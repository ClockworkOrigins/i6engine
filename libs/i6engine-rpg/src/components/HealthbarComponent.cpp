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

#include "i6engine/rpg/components/HealthbarComponent.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/AttributeComponent.h"

namespace i6e {
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

	api::attributeMap HealthbarComponent::synchronize() const {
		return api::attributeMap();
	}

	void HealthbarComponent::show() {
		api::EngineController::GetSingleton().getGUIFacade()->setVisibility("HPBar_" + std::to_string(_id), true);
	}

	void HealthbarComponent::hide() {
		api::EngineController::GetSingleton().getGUIFacade()->setVisibility("HPBar_" + std::to_string(_id), false);
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
