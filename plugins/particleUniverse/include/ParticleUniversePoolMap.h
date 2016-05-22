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

#ifndef __PU_POOLMAP_H__
#define __PU_POOLMAP_H__

#include <map>

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	/** The PoolMap is an extention of the regular Pool. The PoolMap contains objects that can be found by name.
	*/
	template<typename T>
	class _ParticleUniverseExport PoolMap {
	public:
		typedef std::multimap<String, T *> PoolMapMap;
		typedef typename PoolMapMap::iterator PoolMapIterator; // The 'typename' MUST be added, since T is not a fixed type
		PoolMapIterator mPoolMapIterator;

		PoolMap(bool managed = false) : mManaged(managed) {
			if (mManaged) {
				// Unused.
			}
		}

		virtual ~PoolMap() {
			if (mManaged) {
				// Unused.
			}
		}

		/** Returns true if the number of released elements is 0.
		*/
		inline bool isEmpty() const {
			return mReleased.empty();
		}

		/** Todo
		*/
		inline size_t getSize() const {
			return mReleased.size();
		}

		/** Todo
		*/
		inline void resetIterator() {
			mPoolMapIterator = mReleased.begin();
		}

		/** Todo
		*/
		inline T * getFirst() {
			resetIterator();
			if (end()) {
				return nullptr;
			}

			return mPoolMapIterator->second;
		}

		/** Todo
		*/
		inline T * getNext() {
			if (end()) {
				return nullptr;
			}

			mPoolMapIterator++;
			if (end()) {
				return nullptr;
			}
				
			return mPoolMapIterator->second;
		}

		/** Todo
		*/
		inline bool end() {
			return mPoolMapIterator == mReleased.end();
		}

		/** Todo
		*/
		inline void clear() {
			mLocked.clear();
			mReleased.clear();
		}

		/** Add an element to the PoolMap.
		@remarks
			Can only be done if the pool doesn't manage the creation and desctruction of the elements.
		*/
		inline void addElement(const String & key, T * element) {
			if (mManaged) {
				EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "It is not allowed to add elements, because the pool manages the creation/destruction.", "PoolMap::addElement");
			}

			mLocked.insert(make_pair(key, element));
		}

		/** Get an element from the pool.
		@remarks
			This function does not only retrieve an object, but the object is also marked as ´released´. This means
			that the object is exposed to the outside world. If objects in pool are traversed, only the released objects
			are taken into account.
		*/
		inline T * releaseElement(const String & key) {
			// Return with 0 if no elements left
			if (mLocked.empty()) {
				return nullptr;
			}

			// Return the first element that is encountered
			T * t = nullptr;
			PoolMapIterator it = mLocked.find(key);
			if (it != mLocked.end()) {
				// Get the element and move it to the released elements list
				t = it->second;
				mReleased.insert(make_pair(key, t));
				mLocked.erase(it);
			}

			return t;
		}

		/** Todo
		*/
		inline void releaseAllElements() {
			// Move all elements from locked elements to released elements
			for (PoolMapIterator it = mLocked.begin(); it != mLocked.end(); ++it) {
				mReleased.insert(make_pair(it->first, it->second));
			}
			mLocked.clear();
			resetIterator();
		}

		/** Todo
		*/
		inline void lockLatestElement() {
			// Move element pointed by iterator from released elements to locked elements
			mLocked.insert(make_pair(mPoolMapIterator->first, mPoolMapIterator->second));
			mReleased.erase(mPoolMapIterator++); // Watch the ++ at the end to set mPoolMapIterator to the next element
			if (mPoolMapIterator != mReleased.begin() && mPoolMapIterator != mReleased.end()) {
				mPoolMapIterator--;
			}
		}

		/** Lock all elements in the pool.
		@remarks
			All released objects are locked.
		*/
		inline void lockAllElements() {
			// Move all elements from release elements to locked elements
			for (PoolMapIterator it = mReleased.begin(); it != mReleased.end(); ++it) {
				mLocked.insert(make_pair(it->first, it->second));
			}
			mReleased.clear();
			resetIterator();
		}

	protected:
		bool mManaged; // Determine whether the pool is responsible for creation and destruction of the elements
		PoolMapMap mReleased;
		PoolMapMap mLocked;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_POOLMAP_H__ */
