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

#include "i6engine/api/manager/IDManager.h"

#include "i6engine/utils/Exceptions.h"

namespace i6e {
namespace api {

	IDManager::IDManager() : _idNext(0), _idMax(INT64_MAX), _idMutex() {
	}

	void IDManager::setBounds(const int64_t from, const int64_t to) {
		_idNext = from;
		_idMax = to;
	}

	int64_t IDManager::getID() {
		boost::mutex::scoped_lock lock(_idMutex);

		if (_idMax != -1 && _idNext > _idMax) {
			ISIXE_THROW_FAILURE("IDManager", "no more ids available.");
		}
		return _idNext++;
	}

	int64_t IDManager::getID(const uint32_t amount) {
		boost::mutex::scoped_lock lock(_idMutex);

		if (_idMax != -1 && _idNext + amount - 1 > _idMax) {
			ISIXE_THROW_FAILURE("IDManager", "no more ids available.");
		}
		_idNext += amount;
		return _idNext - amount;
	}

} /* namespace api */
} /* namespace i6e */
