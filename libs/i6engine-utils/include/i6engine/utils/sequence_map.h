/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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

namespace i6e {
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
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
			return _list.erase(position);
#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
			// This is O(n) but as of a bug in libstdc++, there is no list::erase for const_iterator
			// See http://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2011 Section 23.3.5 for current support
			for (iterator it = _list.begin(); it != _list.end(); ++it) {
				if (it->first == position->first) {
					return _list.erase(it);
				}
			}
			return _list.end();
#endif
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

		/**
		 * \brief returns iterator to the begin of the list
		 */
		typename std::list<std::pair<KeyType, ValueType>>::iterator begin() {
			return _list.begin();
		}
		
		/**
		 * \brief returns const_iterator to the begin of the list
		 */
		typename std::list<std::pair<KeyType, ValueType>>::const_iterator begin() const {
			return _list.begin();
		}
		
		/**
		 * \brief returns iterator to the end of the list
		 */
		typename std::list<std::pair<KeyType, ValueType>>::iterator end() {
			return _list.end();
		}
		
		/**
		 * \brief returns const_iterator to the end of the list
		 */
		typename std::list<std::pair<KeyType, ValueType>>::const_iterator end() const {
			return _list.end();
		}
		
		/**
		 * \brief returns const_iterator to the begin of the list
		 */
		typename std::list<std::pair<KeyType, ValueType>>::const_iterator cbegin() const {
			return _list.cbegin();
		}
		
		/**
		 * \brief returns const_iterator to the end of the list
		 */
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
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_SEQUENCEMAP_H__ */

/**
 * @}
 */
