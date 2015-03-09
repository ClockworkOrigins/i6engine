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

#include "i6engine/modules/manager/IDManager.h"

#include "i6engine/utils/Exceptions.h"

namespace i6engine {
namespace modules {

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
		// if (_idNext == _idMax - 10) {
			// Feature:
			// TODO: (Michael) request new range from server
		// }
		return _idNext++;
	}

	int64_t IDManager::getID(const uint32_t amount) {
		boost::mutex::scoped_lock lock(_idMutex);

		if (_idMax != -1 && _idNext + amount - 1 > _idMax) {
			ISIXE_THROW_FAILURE("IDManager", "no more ids available.");
		}
		// if (_idNext == _idMax - 10) {
			// Feature:
			// TODO: (Michael) request new range from server
		// }
		_idNext += amount;
		return _idNext - amount;
	}

} /* namespace modules */
} /* namespace i6engine */
