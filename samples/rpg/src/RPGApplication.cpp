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

#include "RPGApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
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

#include "boost/bind.hpp"

namespace sample {

	RPGApplication::RPGApplication() : i6engine::api::Application(), _showFPS(false), _iniParser() {
		_iniParser.load("RPG.ini");
	}

	RPGApplication::~RPGApplication() {
	}

	void RPGApplication::Initialize() {
	}

	void RPGApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// sets gravity for the game... here like on earth
		std::string gravityString;
		_iniParser.getValue("PHYSIC", "gravity", gravityString);
		i6engine::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(gravityString));

		// ambient light for the scene
		std::string ambientLightString;
		_iniParser.getValue("GRAPHIC", "ambientLight", ambientLightString);
		auto vec = i6engine::utils::split(ambientLightString, " ");
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(std::stod(vec[0]), std::stod(vec[1]), std::stod(vec[2]));

		// setting shadow technique... currently only additive stencil possible
		uint16_t shadowTechnique;
		_iniParser.getValue("GRAPHIC", "shadowTechnique", shadowTechnique);
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6engine::api::graphics::ShadowTechnique(shadowTechnique));

		// setting distance fog
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setExponentialFog(Vec3(0.9, 0.9, 0.9), 0.005);

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));

		// register F12 to take screenshot
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F12, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::GraphicsFacade::takeScreenshot, i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade(), "RPGScreen_", ".jpg"));

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

		// loads the RPG demo level
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel("../media/maps/RPGLevel.xml", "Singleplayer");

		// a hack to load rpg library
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player").front()->getGOC<i6engine::rpg::components::NameComponent>(i6engine::rpg::components::config::ComponentTypes::NameComponent)->getName();

		// shows fps (activate/deactive using F1)
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F1, i6engine::api::KeyState::KEY_PRESSED, [this]() {
			if (!_showFPS) {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->showFPS(0.0, 0.0, "RPG/StaticImage", "RPG/Blanko", "RPG", "TbM_Filling");
			} else {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->hideFPS();
			}
			_showFPS = !_showFPS;
		});

		// register keys
		uint16_t key;
		// action key, default: E
		_iniParser.getValue("INPUT", "action", key);
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(i6engine::api::KeyCode(key), "action");
		// inventory key, default: TAB
		_iniParser.getValue("INPUT", "inventory", key);
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(i6engine::api::KeyCode(key), "inventory");

		// forward key, default: W
		_iniParser.getValue("INPUT", "forward", key);
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(i6engine::api::KeyCode(key), "forward");
		// backward key, default: S
		_iniParser.getValue("INPUT", "backward", key);
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(i6engine::api::KeyCode(key), "backward");
		// left key, default: A
		_iniParser.getValue("INPUT", "left", key);
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(i6engine::api::KeyCode(key), "left");
		// right key, default: D
		_iniParser.getValue("INPUT", "right", key);
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->setKeyMapping(i6engine::api::KeyCode(key), "right");
	}

	void RPGApplication::Tick() {
	}

	bool RPGApplication::ShutdownRequest() {
		return true;
	}

	void RPGApplication::Finalize() {
	}

	void RPGApplication::ShutDown() {
	}

} /* namespace sample */
