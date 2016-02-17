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

#include "i6engine/modules/object/ComponentFactory.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
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
			id = api::EngineController::GetSingletonPtr()->getIDManager()->getID();
		}

		// Return the new GOComponent created by the registered method
		api::ComPtr co = (it->second)(id, params);
		owner.get()->setGOC(co);
		co->setSelf(co);
		co->enableTicking(_tickingAllowed);

		api::EngineController::GetSingletonPtr()->getObjectFacade()->notifyNewID(id);

		return co;
	}

} /* namespace modules */
} /* namespace i6engine */
