/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __I6ENGINE_UTILS_RANGEDMAP_H__
#define __I6ENGINE_UTILS_RANGEDMAP_H__

#include "i6engine/utils/Exceptions.h"
#include <fstream>

namespace i6engine {
namespace utils {

	/**
	 * class RangedMap
	 *
	 */
	template<typename Key, typename Value>
	class RangedMap {
	public:
		struct Element;
		struct Block {
			Element * left;
			Element * mid;
			Element * right;
			Key leftSplit, middleSplit;

			Block() : left(nullptr), mid(nullptr), right(nullptr), leftSplit(), middleSplit() {
			}

			~Block() {
				delete left;
				delete mid;
				delete right;
			}

			void dump(std::ostream & of) {
				of << "\"" << this << "\"[label=\"Block\\n" << leftSplit << "|" << middleSplit << "\"];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << left << "\"" << "[color=red,label=\"L\"];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << mid << "\"" << "[color=red,label=\"M\"];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << right << "\"" << "[color=red,label=\"R\"];" << std::endl;
				if (left) left->dump(of);
				if (mid) mid->dump(of);
				if (right) right->dump(of);
			}
		};
		struct Element {
			Element(const Key & s, const Key & e, const Value & v) : _start(s), _end(e), _next(nullptr), _val(v), _leftNext(nullptr), _rightNext(nullptr), _parent(nullptr) {
			}
			Key _start, _end;
			Block * _next;
			Value _val;
			Element * _leftNext;
			Element * _rightNext;
			Element * _parent;

			~Element() {
				delete _next;
			}

			void dump(std::ostream & of) {
				of << "\"" << this << "\"" << "[label=\"" << _start << "-" << _end << ":" << _val << "\"];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << _leftNext << "\"" << "[color=blue];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << _rightNext << "\"" << "[color=blue];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << _parent << "\"" << "[color=black];" << std::endl;
				of << "\"" << this << "\"" << " -> " << "\"" << _next << "\"" << "[color=green];" << std::endl;
				if (_next) _next->dump(of);
			}
			bool isValid() const {
				return _start <= _end;
			}
		};

		struct iterator {
			iterator(Element * e) : _cur(e) {
			}

			bool valid() const {
				return _cur != nullptr;
			}

			Value operator*() {
				return _cur->_val;
			}
		private:
			Element * _cur;
		};

		/**
		 * \brief default constructor
		 */
		RangedMap(const Key & minVal, const Key & maxVal, const Value & def) : _head(nullptr), _merged(false) {
			_head = new Element(minVal, maxVal, def);
		}

		~RangedMap() {
			delete _head;
		}

		void dump(const std::string & fileName) {
			std::ofstream of;
			of.open(fileName.c_str());
			of << "digraph {\n";
			_head->dump(of);
			of << "}";
			of.close();
		}

		Value get(const Key & k) const {
			Element * e = _head;
			if (k < e->_start || k > e->_end) {
				ISIXE_THROW_FAILURE("RangedMap", "Key " << k << " not found");
			}
			while (e->_next != nullptr) { // iterate as long as there is no further level
				Block * b = e->_next;
				if (k > b->middleSplit) {
					e = b->right;
				} else if (k > b->leftSplit) {
					e = b->mid;
				} else {
					e = b->left;
				}
			}
			return e->_val;
		}

		void eraseElement(Element * e) {
			if (e->_leftNext) {
				e->_leftNext->_rightNext = e->_rightNext;
			}
			if (e->_rightNext) {
				e->_rightNext->_leftNext = e->_leftNext;
			}
		}

		/**
		 * \brief updates the start range of e and all its parents until the key isn't on the border anymore
		 * afterwards, updates the splitValue
		 */
		void updateStart(Element * e, const Key k, int range) {
			Element * previous = e;
			while (e && e->_start == k) {
				previous = e;
				e->_start += range;
				e = e->_parent;
			}
			if (isLeft(previous)) {
				ISIXE_THROW_FAILURE("RangedMap", "Corrupted structure");
			} else if (isMid(previous)) {
				e->_next->leftSplit = previous->_start - 1;
			} else if (isRight(previous)) {
				e->_next->middleSplit = previous->_start - 1;
			}
		}

		/**
		 * \brief updates the end range of e and all its parents until the key isn't on the border anymore
		 * afterwards, updates the splitValue
		 */
		void updateEnd(Element * e, const Key k, int range) {
			Element * previous = e;
			while (e && e->_end == k) {
				previous = e;
				e->_end += range;
				e = e->_parent;
			}
			if (isLeft(previous)) {
				e->_next->leftSplit = previous->_end;
			} else if (isMid(previous)) {
				e->_next->middleSplit = previous->_end;
			} else if (isRight(previous)) {
				ISIXE_THROW_FAILURE("RangedMap", "Corrupted structure");
			}
		}

		Element * combineBlockOneValid(Element * e) {
			Element * par = e->_parent;

			// ultra merge
			Element * ne = nullptr;
			Block * nex = par->_next;

			if (!nex->left->isValid()) {
				eraseElement(par->_next->left);
			} else {
				ne = nex->left;
			}
			if (!nex->mid->isValid()) {
				eraseElement(nex->mid);
			} else {
				ne = nex->mid;
			}
			if (!nex->right->isValid()) {
				eraseElement(nex->right);
			} else {
				ne = nex->right;
			}
			par->_start = ne->_start;
			par->_end = ne->_end;
			par->_val = ne->_val;
			par->_next = ne->_next;
			if (par->_next) {
				par->_next->left->_parent = par;
				par->_next->mid->_parent = par;
				par->_next->right->_parent = par;
			} else {
				par->_leftNext = ne->_leftNext;
				if (ne->_leftNext) {
					ne->_leftNext->_rightNext = par;
				}
				par->_rightNext = ne->_rightNext;
				if (ne->_rightNext) {
					ne->_rightNext->_leftNext = par;
				}
			}

			ne->_next = nullptr;
			delete nex;

			return par;
		}

		bool isLeft(const Element * e) const {
			return e->_parent && e == e->_parent->_next->left;
		}

		bool isMid(const Element * e) const {
			return e->_parent && e == e->_parent->_next->mid;
		}

		bool isRight(const Element * e) const {
			return e->_parent && e == e->_parent->_next->right;
		}

		void swapLeftMid(Element * el) {
			Element * em = el->_parent->_next->mid;
			Element * right = el->_rightNext;
			Element * r2 = em;
			while (r2->_next) {
				r2 = r2->_next->right;
			}

			// swap left/mid pointer
			std::swap(el->_parent->_next->left, el->_parent->_next->mid);

			// modify linked list pointer
			right->_leftNext = el->_leftNext;
			if (el->_leftNext) {
				el->_leftNext->_rightNext = right;
			}

			el->_rightNext = r2->_rightNext;
			r2->_rightNext->_leftNext = el;

			r2->_rightNext = el;
			el->_leftNext = r2;

			// update inValidMid ranges
			el->_start = r2->_end + 1;
			el->_end = r2->_end;

			// update splits
			el->_parent->_next->leftSplit = r2->_end;
			el->_parent->_next->middleSplit = r2->_end;
		}

		void swapRightMid(Element * er) {
			Element * em = er->_parent->_next->mid;
			Element * left = er->_leftNext;
			Element * l2 = em;
			while (l2->_next) {
				l2 = l2->_next->left;
			}

			// swap right/mid pointer
			std::swap(er->_parent->_next->right, er->_parent->_next->mid);

			// modify linked list pointer
			left->_rightNext = er->_rightNext;
			if (er->_rightNext) {
				er->_rightNext->_leftNext = left;
			}

			er->_leftNext = l2->_leftNext;
			l2->_leftNext->_rightNext = er;

			l2->_leftNext = er;
			er->_rightNext = l2;

			// update inValidMid ranges
			er->_start = l2->_start - 1;
			er->_end = l2->_start - 2;

			// update splits
			er->_parent->_next->leftSplit = l2->_start - 1;
			er->_parent->_next->middleSplit = l2->_start - 1;
		}

		std::pair<Element *, bool> tryMergeLeftToMid(Element * e, const Key k, const Value v, int range) {
			if (k == e->_end && /* e->_rightNext && */ e->_rightNext->_val == v && e->_rightNext->isValid()) {
				_merged = true;
				updateStart(e->_rightNext, e->_rightNext->_start, -range);
				e->_end -= range;
				if (!e->isValid()) { // range is now empty
					swapLeftMid(e);
				} else {
					e = e->_rightNext;
				}
				return std::make_pair(e, true);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeRightToMid(Element * e, const Key k, const Value v, int range) {
			if (k == e->_start && /* e->_leftNext && */ e->_leftNext->_val == v && e->_leftNext->isValid()) {
				_merged = true;
				updateEnd(e->_leftNext, e->_leftNext->_end, range);
				e->_start += range;
				if (!e->isValid()) { // range is now empty
					swapRightMid(e);
				} else {
					e = e->_leftNext;
				}
				return std::make_pair(e, true);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeMidToLeft(Element * e, const Key k, const Value v, int range) {
			if (k == e->_start && /* e->_leftNext && */ e->_leftNext->_val == v) {
				_merged = true;
				updateEnd(e->_leftNext, e->_leftNext->_end, range);
				e->_start += range;
				return std::make_pair(e->_leftNext, true);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeMidToRight(Element * e, const Key k, const Value v, int range) {
			if (k == e->_end && /* e->_rightNext && */ e->_rightNext->_val == v) {
				_merged = true;
				updateStart(e->_rightNext, e->_rightNext->_start, -range);
				e->_end -= range;
				return std::make_pair(e->_rightNext, true);
			}
			return std::make_pair(e, false);
		}

		Element * tryMergeToLeft(Element * e, const Key k, const Value v, int range) {
			std::pair<Element *, bool> res;
			if (isLeft(e)) {
				res = tryMergeLeftToCrossLeftMid(e, k, v, range);
				if (!res.second) {
					res = tryMergeLeftToCrossLeftLeft(e, k, v, range);
				}
			} else if (isMid(e)) {
				res = tryMergeMidToLeft(e, k, v, range);
			} else if (isRight(e)) {
				res = tryMergeRightToMid(e, k, v, range);
				if (!res.second) {
					res = tryMergeRightToLeft(e, k, v, range);
				}
			}
			return res.first;
		}

		Element * tryMergeToRight(Element * e, const Key k, const Value v, int range) {
			std::pair<Element *, bool> res;
			if (isLeft(e)) {
				res = tryMergeLeftToMid(e, k, v, range);
				if (!res.second) {
					res = tryMergeLeftToRight(e, k, v, range);
				}
			} else if (isMid(e)) {
				res = tryMergeMidToRight(e, k, v, range);
			} else if (isRight(e)) {
				res = tryMergeRightToCrossRightMid(e, k, v, range);
				if (!res.second) {
					res = tryMergeRightToCrossRightRight(e, k, v, range);
				}
			}
			return res.first;
		}

		std::pair<Element *, bool> tryMergeLeftToCrossLeftLeft(Element * e, const Key k, const Value v, int range) {
			// iff
			//  *	we change the left border
			//  *	we have a left neighbour
			//  *	the left neighbour is invalid (-> we have a left left neighbour)
			//  *	the values match
			if (k == e->_start && e->_leftNext && !e->_leftNext->isValid() && e->_leftNext->_leftNext->_val == v) {
				_merged = true;
				Element * target = e->_leftNext->_leftNext;
				updateEnd(target, target->_end, range);
				updateStart(e, e->_start, range);
				// update skipped node
				e->_leftNext->_start += range;
				e->_leftNext->_end += range;

				if (!e->isValid()) {
					// e->_rightNext can be in subtree
					if (e->_rightNext->isValid()) {
						// left is invalid, mid is valid => just swap (swap takes care of subtree)
						swapLeftMid(e);
					} else {
						e = combineBlockOneValid(e->_rightNext);
					}
				}

				return std::make_pair(target, false);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeRightToCrossRightRight(Element * e, const Key k, const Value v, int range) {
			// iff
			//  *	we change the right border
			//  *	we have a right neighbour
			//  *	the right neighbour is invalid (-> we have a right right neighbour)
			//  *	the values match
			if (k == e->_end && e->_rightNext && !e->_rightNext->isValid() && e->_rightNext->_rightNext->_val == v) {
				_merged = true;
				Element * target = e->_rightNext->_rightNext;
				updateStart(target, target->_start, -range);
				updateEnd(e, e->_end, -range);
				// update skipped node
				e->_rightNext->_start -= range;
				e->_rightNext->_end -= range;

				if (!e->isValid()) {
					// e->_leftNext can be in subtree
					if (e->_leftNext->isValid()) {
						// right is invalid, mid is valid => just swap (swap takes care of subtree)
						swapRightMid(e);
					} else {
						e = combineBlockOneValid(e->_leftNext);
					}
				}

				return std::make_pair(target, false);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeLeftToCrossLeftMid(Element * e, const Key k, const Value v, int range) {
			if (k == e->_start && e->_leftNext && e->_leftNext->_val == v && e->_leftNext->isValid()) {
				_merged = true;
				// merge cross to left
				// update ranges
				Element * newE = e->_leftNext;

				updateEnd(e->_leftNext, e->_leftNext->_end, range);
				updateStart(e, e->_start, range);
				if (!e->isValid()) {
					// e->_rightNext can be in subtree
					if (e->_rightNext->isValid()) {
						// left is invalid, mid is valid => just swap (swap takes care of subtree)
						swapLeftMid(e);
					} else {
						e = combineBlockOneValid(e->_rightNext);
					}
				}
				return std::make_pair(newE, true);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeRightToCrossRightMid(Element * e, const Key k, const Value v, int range) {
			if (k == e->_end && e->_rightNext && e->_rightNext->_val == v && e->_rightNext->isValid()) {
				_merged = true;
				// merge cross to right
				// update ranges
				Element * newE = e->_rightNext;

				updateStart(e->_rightNext, e->_rightNext->_start, -range);
				updateEnd(e, e->_end, -range);
				if (!e->isValid()) {
					// e->_leftNext can be in subtree
					if (e->_leftNext->isValid()) {
						// right is invalid, mid is valid => just swap (swap takes care of subtree)
						swapRightMid(e);
					} else {
						e = combineBlockOneValid(e->_leftNext);
					}
				}
				return std::make_pair(newE, true);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeLeftToRight(Element * e, const Key k, const Value v, int range) {
			if (k == e->_end && /* e->_rightNext && */ !e->_rightNext->isValid() && e->_rightNext->_rightNext->_val == v) {
				_merged = true;
				// merge over invalid range
				updateStart(e->_rightNext->_rightNext, e->_rightNext->_rightNext->_start, -range);
				// update left and middle node
				e->_end -= range;
				e->_rightNext->_start -= range;
				e->_rightNext->_end -= range;
				// update the leftSplit
				e->_parent->_next->leftSplit -= range;

				Element * newE = e->_rightNext->_rightNext;
				if (!e->isValid()) {
					e = combineBlockOneValid(e->_rightNext);
					// combined was only a leaf node -> we need this node to return
					// because newE is now a deleted node (combine* copies all data from the valid child to the parent)
					if (e->_next == nullptr) {
						newE = e;
					}
				} else {
					// left border can't be merged
				}
				return std::make_pair(newE, true);
			}
			return std::make_pair(e, false);
		}

		std::pair<Element *, bool> tryMergeRightToLeft(Element * e, const Key k, const Value v, int range) {
			if (k == e->_start && /* e->_leftNext && */ !e->_leftNext->isValid() && e->_leftNext->_leftNext->_val == v) {
				_merged = true;
				// merge over invalid range
				updateEnd(e->_leftNext->_leftNext, e->_leftNext->_leftNext->_end, range);
				// update right and middle node
				e->_start += range;
				e->_leftNext->_start += range;
				e->_leftNext->_end += range;

				// update the middleSplit
				e->_parent->_next->middleSplit += range;

				Element * newE = e->_leftNext->_leftNext;

				if (!e->isValid()) {
					e = combineBlockOneValid(e->_leftNext);
					// combined was only a leaf node -> we need this node to return
					// because newE is now a deleted node (combine* copies all data from the valid child to the parent)
					if (e->_next == nullptr) {
						newE = e;
					}
				} else {
					// right border can't be merged
				}
				return std::make_pair(newE, true);
			}
			return std::make_pair(e, false);
		}

		void set(const Key k, const Value v) {
			Element * e = _head;
			Block * bLast;
			if (k < e->_start || k > e->_end) {
				ISIXE_THROW_FAILURE("RangedMap", "Key " << k << " not found");
			}
			while(e->_next != nullptr) { // iterate as long as there is no further level
				bLast = e->_next;
				if (k > bLast->middleSplit) {
					e = bLast->right;
				} else if (k > bLast->leftSplit) {
					e = bLast->mid;
				} else {
					e = bLast->left;
				}
			}
			if (e->_val == v) {
				return;
			}
			_merged = false;
			if (isMid(e)) {
				// mid
				e = tryMergeToLeft(e, k, v, 1);
				int r = 1;
				if (_merged) {
					r = e->_end - e->_start + 1;
				}
				e = tryMergeToRight(e, k, v, r);
			} else if (isLeft(e)) {
				// left
				e = tryMergeToRight(e, k, v, 1);
				int r = 1;
				if (_merged) {
					r = e->_end - e->_start + 1;
				}
				e = tryMergeToLeft(e, k, v, r);
			} else if (isRight(e)) {
				// right
				e = tryMergeToLeft(e, k, v, 1);
				int r = 1;
				if (_merged) {
					r = e->_end - e->_start + 1;
				}
				e = tryMergeToRight(e, k, v, r);
			}

			if (!_merged) {
				if (k == e->_start && k == e->_end) {
					e->_val = v;
				} else if (isRight(e) && k == e->_start && !e->_leftNext->isValid()) {
					e->_leftNext->_start = k;
					e->_leftNext->_end = k;
					e->_start++;
					e->_parent->_next->middleSplit++;
					e->_leftNext->_val = v;
				} else if (isLeft(e) && k == e->_end && !e->_rightNext->isValid()) {
					e->_rightNext->_start = k;
					e->_rightNext->_end = k;
					e->_end--;
					e->_parent->_next->leftSplit--;
					e->_rightNext->_val = v;
				} else {
					Block * b = new Block();
					if (k == e->_start) {
						b->left = new Element(k, k, v);
						b->mid = new Element(k + 1, k, v);
						b->right = new Element(k + 1, e->_end, e->_val);
						b->leftSplit = k;
						b->middleSplit = k;
					} else if (k == e->_end) {
						b->left = new Element(e->_start, k - 1, e->_val);
						b->mid = new Element(k, k - 1, v);
						b->right = new Element(k, k, v);
						b->leftSplit = k - 1;
						b->middleSplit = k - 1;
					} else {
						b->left = new Element(e->_start, k - 1, e->_val);
						b->mid = new Element(k, k, v);
						b->right = new Element(k + 1, e->_end, e->_val);
						b->leftSplit = k - 1;
						b->middleSplit = k;
					}
					b->left->_parent = e;
					b->mid->_parent = e;
					b->right->_parent = e;

					// Element neighbours
					b->left->_leftNext = e->_leftNext;
					if (e->_leftNext != nullptr) {
						e->_leftNext->_rightNext = b->left;
					}

					b->mid->_leftNext = b->left;
					b->left->_rightNext = b->mid;
					b->mid->_rightNext = b->right;
					b->right->_leftNext = b->mid;

					b->right->_rightNext = e->_rightNext;
					if (e->_rightNext != nullptr) {
						e->_rightNext->_leftNext = b->right;
					}

					e->_leftNext = nullptr;
					e->_rightNext = nullptr;

					e->_next = b;
				}
			}
			return;
		}

	private:
		Element * _head;
		bool _merged;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_RANGEDMAP_H__ */
