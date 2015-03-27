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

#include "i6engine/api/components/ShatterComponent.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	ShatterComponent::ShatterComponent(const int64_t id, const attributeMap & params) : Component(id, params), _respawn(0) {
		Component::_objFamilyID = components::ShatterComponent;
		Component::_objComponentID = components::ShatterComponent;
	}

	ShatterComponent::~ShatterComponent() {
	}

	void ShatterComponent::Tick() {
	}

	void ShatterComponent::Init() {
	}

	void ShatterComponent::News(const GameMessage::Ptr & msg) {
		uint16_t type = msg->getSubtype();

		if (type == components::ComShatter) {
			int64_t b = static_cast<components::Component_Shatter_Update *>(msg->getContent())->other;
			GOPtr objB = EngineController::GetSingletonPtr()->getObjectFacade()->getObject(b);

			if (objB == nullptr) {
				ISIXE_LOG_WARN("ObjectMailbox", "ComShatter message for a nonexistent GameObject");
				return;
			}

#ifdef ISIXE_PROFILING
			msg->insertTimestamp("ShatterComponent: " + _objComponentID);
#endif /* ISIXE_PROFILING */

			shatter(objB);
		} else {
			ISIXE_THROW_FAILURE("ShatterComponent", "Unknown message type");
		}
	}

	attributeMap ShatterComponent::synchronize() const {
		return attributeMap();
	}

} /* namespace api */
} /* namespace i6engine */
