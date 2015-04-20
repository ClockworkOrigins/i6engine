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

#include "LeveleditorApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/InputFacade.h"

#ifdef ISIXE_WITH_RPG
	#include "i6engine/api/facades/ObjectFacade.h"

	#include "i6engine/rpg/components/AttributeComponent.h"
	#include "i6engine/rpg/components/HealthbarComponent.h"
	#include "i6engine/rpg/components/ListInventoryComponent.h"
	#include "i6engine/rpg/components/NameComponent.h"
	#include "i6engine/rpg/components/QuickslotComponent.h"
	#include "i6engine/rpg/components/SlotComponent.h"
	#include "i6engine/rpg/components/SlotInventoryComponent.h"
	#include "i6engine/rpg/components/ThirdPersonControlComponent.h"
	#include "i6engine/rpg/components/UsableItemComponent.h"
	#include "i6engine/rpg/components/WeightInventoryComponent.h"
#endif

#include "boost/bind.hpp"

namespace sample {

	LeveleditorApplication::LeveleditorApplication() : i6engine::editor::Editor("Leveleditor Sample") {
	}

	LeveleditorApplication::~LeveleditorApplication() {
	}

	void LeveleditorApplication::AfterInitialize() {
		Editor::AfterInitialize();

#ifdef ISIXE_WITH_RPG
		// register rpg components we want to use
		// do this befor loading the level
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Attribute", boost::bind(&i6engine::rpg::components::AttributeComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Healthbar", boost::bind(&i6engine::rpg::components::HealthbarComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ListInventory", boost::bind(&i6engine::rpg::components::ListInventoryComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Name", boost::bind(&i6engine::rpg::components::NameComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Quickslot", boost::bind(&i6engine::rpg::components::QuickslotComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Slot", boost::bind(&i6engine::rpg::components::SlotComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("SlotInventory", boost::bind(&i6engine::rpg::components::SlotInventoryComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ThirdPersonControl", boost::bind(&i6engine::rpg::components::ThirdPersonControlComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("UsableItem", boost::bind(&i6engine::rpg::components::UsableItemComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("WeightInventory", boost::bind(&i6engine::rpg::components::WeightInventoryComponent::createC, _1, _2));
#endif

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));
	}

} /* namespace sample */
