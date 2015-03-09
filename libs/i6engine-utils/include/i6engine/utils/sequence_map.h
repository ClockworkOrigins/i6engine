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

/**
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_SEQUENCEMAP_H__
#define __I6ENGINE_UTILS_SEQUENCEMAP_H__

#include <cassert>
#include <list>
#include <unordered_map>

namespace i6engine {
namespace utils {

	/**
	 * \class sequence_map
	 * \brief A map with linear access time and an iterator iterating through the elements in creation time.
	 * This map is intended for fast accessing complexity with preserving insertion order
	 * | Operation | Time | Desc. |
	 * |-----------|:----:|-------|
	 * | Insert | O(1) | |
	 * | Access | O(1) | |
	 * | erase(key) | O(n) | maybe improvable when storing iterators not pointers |
	 * | erase(iterator) | O(n) | O(1) as soon as stdlib++ allows const_iterator in erase (bug) |
	 */
	template<typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
	class sequence_map {
	public:
		typedef typename std::list<std::pair<KeyType, ValueType>>::const_iterator const_iterator;
		typedef typename std::list<std::pair<KeyType, ValueType>>::iterator iterator;

		/**
		 * \brief constructor
		 */
		sequence_map() : _map(), _list() {
		}

		/**
		 * \brief acess operator with [] using key, returns
		 */
		ValueType & operator[](const KeyType & key) {
			if (_map.find(key) == _map.end()) {
				_list.push_back(std::make_pair(key, ValueType()));
				_map[key] = &(_list.back().second);
			}
			return *_map[key];
		}

		/**
		 * \brief returns size of the map
		 */
		std::size_t size() const {
			return _map.size();
		}

		/**
		 * \brief removes value for given key, returns 1 if successful, otherwise 0
		 */
		std::size_t erase(const KeyType & key) {
			_map.erase(key);
			for (auto it = _list.begin(); it != _list.end(); it++) {
				if (it->first == key) {
					_list.erase(it);
					return 1;
				}
			}
			return 0;
		}

		/**
		 * \brief remove value contained by iterator
		 */
		const_iterator erase(const_iterator position) {
			_map.erase(position->first);
			// This is O(n) but as of a bug in libstdc++, there is no list::erase for const_iterator
			// See http://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011 Section 23.3.5 for current support
			for (iterator it = _list.begin(); it != _list.end(); ++it) {
				if (it->first == position->first) {
					return _list.erase(it);
				}
			}
			return _list.end();
		}

		/**
		 * \brief returns iterator to found entry for key, otherwise end()
		 */
		typename std::list<std::pair<KeyType, ValueType>>::iterator find(const KeyType & key) {
			if (_map.find(key) == _map.end()) {
				return _list.end();
			}
			for (typename std::list<std::pair<KeyType, ValueType>>::iterator it = _list.begin(); it != _list.end(); ++it) {
				if (it->first == key) {
					return it;
				}
			}
			assert(false); // key appears in map but not in list
			return _list.end();
		}

		typename std::list<std::pair<KeyType, ValueType>>::const_iterator find(const KeyType & key) const {
			if (_map.find(key) == _map.end()) {
				return _list.end();
			}
			for (typename std::list<std::pair<KeyType, ValueType>>::const_iterator it = _list.begin(); it != _list.end(); ++it) {
				if (it->first == key) {
					return it;
				}
			}
			assert(false); // key appears in map but not in list
			return _list.end();
		}

		typename std::list<std::pair<KeyType, ValueType>>::iterator begin() {
			return _list.begin();
		}

		typename std::list<std::pair<KeyType, ValueType>>::const_iterator begin() const {
			return _list.begin();
		}

		typename std::list<std::pair<KeyType, ValueType>>::iterator end() {
			return _list.end();
		}

		typename std::list<std::pair<KeyType, ValueType>>::const_iterator end() const {
			return _list.end();
		}

		typename std::list<std::pair<KeyType, ValueType>>::const_iterator cbegin() const {
			return _list.cbegin();
		}

		typename std::list<std::pair<KeyType, ValueType>>::const_iterator cend() const {
			return _list.cend();
		}

		/**
		 * \brief clears map
		 */
		void clear() {
			_map.clear();
			_list.clear();
		}

	private:
		std::unordered_map<KeyType, ValueType *, Hash> _map;

		std::list<std::pair<KeyType, ValueType>> _list;
	};

} /* namespace utils */
} /* namespace i6engine */

#endif /* __I6ENGINE_UTILS_SEQUENCEMAP_H__ */

/**
 * @}
 */
