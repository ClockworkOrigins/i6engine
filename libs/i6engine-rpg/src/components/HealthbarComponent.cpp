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

#include "i6engine/rpg/components/HealthbarComponent.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/AttributeComponent.h"

namespace i6engine {
namespace rpg {
namespace components {

	HealthbarComponent::HealthbarComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _currentHP(), _maxHP() {
		_objFamilyID = config::ComponentTypes::HealthbarComponent;
		_objComponentID = config::ComponentTypes::HealthbarComponent;
	}

	api::ComPtr HealthbarComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<HealthbarComponent, api::Component>(id, params);
	}

	void HealthbarComponent::Init() {
		auto ac = getOwnerGO()->getGOC<AttributeComponent>(config::ComponentTypes::AttributeComponent);
		_currentHP = ac->getAttributeValue(Attribute::HP);
		_maxHP = ac->getAttributeValue(Attribute::HP_MAX);
		api::EngineController::GetSingleton().getGUIFacade()->addProgressBar("HPBar_" + std::to_string(_id), "RPG/ProgressBar", 0.01, 0.95, 0.2, 0.05);
		api::EngineController::GetSingleton().getGUIFacade()->setProgress("HPBar_" + std::to_string(_id), _currentHP / double(_maxHP));
		ac->registerListener(Attribute::HP, [this](int32_t hp) {
			_currentHP = hp;
			api::EngineController::GetSingleton().getGUIFacade()->setProgress("HPBar_" + std::to_string(_id), _currentHP / double(_maxHP));
		});
		ac->registerListener(Attribute::HP_MAX, [this](int32_t hp) {
			_maxHP = hp;
			api::EngineController::GetSingleton().getGUIFacade()->setProgress("HPBar_" + std::to_string(_id), _currentHP / double(_maxHP));
		});
	}

	void HealthbarComponent::Finalize() {
		api::EngineController::GetSingleton().getGUIFacade()->deleteWidget("HPBar_" + std::to_string(_id));
	}

	api::attributeMap HealthbarComponent::synchronize() {
		return api::attributeMap();
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
