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

#include "i6engine/modules/object/ComponentFactory.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace modules {

	ComponentFactory::ComponentFactory() : _callbacks(), _tickingAllowed(true) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	ComponentFactory::~ComponentFactory() {
		// If the map is empty, there is nothing to be done
		_callbacks.clear();
	}

	void ComponentFactory::registerCTemplate(const std::string & GOCType, const api::createGOCCallback & fn) {
		ASSERT_THREAD_SAFETY_FUNCTION
		// Register GOCType and its callback function at the factory
		if (_callbacks.find(GOCType) != _callbacks.end()) {
			ISIXE_THROW_FAILURE("ComponentFactory", "Component type '" << GOCType << "' is already registered!");
		}
		_callbacks[GOCType] = fn;
	}

	api::ComPtr ComponentFactory::createGOC(int64_t id, const std::string & GOCType, const api::attributeMap & params, const api::WeakGOPtr & owner) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		// Check, if the GOCType is registered at the factory
		callbackMap::const_iterator it = _callbacks.find(GOCType);

		// Not registered
		if (it == _callbacks.end()) {
			ISIXE_THROW_FAILURE("ComponentFactory", "GOComponent '" << GOCType << "' not registered");
			return api::ComPtr();
		}

		if (id == -1) {
			id = i6eEngineController->getIDManager()->getID();
		}

		// Return the new GOComponent created by the registered method
		api::ComPtr co = (it->second)(id, params);
		std::pair<bool, int64_t> addedOrReplaced = owner.get()->setGOC(co);
		if (addedOrReplaced.first) {
			co->setSelf(co);
			co->enableTicking(_tickingAllowed);

			i6eEngineController->getObjectFacade()->notifyNewID(id);

			if (addedOrReplaced.second != -1) {
				i6eEngineController->getObjectFacade()->notifyDeletedID(id);
			}

			return co;
		} else {
			return api::ComPtr();
		}
	}

} /* namespace modules */
} /* namespace i6e */
