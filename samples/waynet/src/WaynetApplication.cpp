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

#include "WaynetApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/MoveComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/ToggleWaynetComponent.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/manager/WaynetManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/components/AttributeComponent.h"
#include "i6engine/rpg/components/HealthbarComponent.h"
#include "i6engine/rpg/components/HumanMovementComponent.h"
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

	WaynetApplication::WaynetApplication() : i6engine::api::Application(), _showFPS(false), _iniParser(), _camera(), _eventMap() {
		_iniParser.load("RPG.ini");
		_eventMap["forward"] = std::make_pair(boost::bind(&WaynetApplication::Forward, this), false);
		_eventMap["backward"] = std::make_pair(boost::bind(&WaynetApplication::Backward, this), false);
		_eventMap["left"] = std::make_pair(boost::bind(&WaynetApplication::Left, this), false);
		_eventMap["right"] = std::make_pair(boost::bind(&WaynetApplication::Right, this), false);
		_eventMap["down"] = std::make_pair(boost::bind(&WaynetApplication::Down, this), false);
		_eventMap["up"] = std::make_pair(boost::bind(&WaynetApplication::Up, this), false);
		_eventMap["rotateLeft"] = std::make_pair(boost::bind(&WaynetApplication::RotateLeft, this), false);
		_eventMap["rotateRight"] = std::make_pair(boost::bind(&WaynetApplication::RotateRight, this), false);
		_eventMap["rotateUp"] = std::make_pair(boost::bind(&WaynetApplication::RotateUp, this), false);
		_eventMap["rotateDown"] = std::make_pair(boost::bind(&WaynetApplication::RotateDown, this), false);
		_eventMap["leanLeft"] = std::make_pair(boost::bind(&WaynetApplication::LeanLeft, this), false);
		_eventMap["leanRight"] = std::make_pair(boost::bind(&WaynetApplication::LeanRight, this), false);
	}

	WaynetApplication::~WaynetApplication() {
	}

	void WaynetApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType, WaynetApplication::News, this);
	}

	void WaynetApplication::AfterInitialize() {
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
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("HumanMovement", boost::bind(&i6engine::rpg::components::HumanMovementComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ListInventory", boost::bind(&i6engine::rpg::components::ListInventoryComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Name", boost::bind(&i6engine::rpg::components::NameComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Quickslot", boost::bind(&i6engine::rpg::components::QuickslotComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("Slot", boost::bind(&i6engine::rpg::components::SlotComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("SlotInventory", boost::bind(&i6engine::rpg::components::SlotInventoryComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("ThirdPersonControl", boost::bind(&i6engine::rpg::components::ThirdPersonControlComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("UsableItem", boost::bind(&i6engine::rpg::components::UsableItemComponent::createC, _1, _2));
		i6engine::api::EngineController::GetSingleton().getObjectFacade()->registerCTemplate("WeightInventory", boost::bind(&i6engine::rpg::components::WeightInventoryComponent::createC, _1, _2));

		// loads the RPG demo level
		i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel("../media/maps/SampleLevel.xml", "Singleplayer");

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6engine::api::objects::GOTemplate tmpl;
			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("ToggleWaynet", i6engine::api::attributeMap(), "", false, false));
			of->createGO("SpectatorCam", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this](i6engine::api::GOPtr go) {
				_camera = go;
				go->getGOC<i6engine::api::ToggleWaynetComponent>(i6engine::api::components::ComponentTypes::ToggleWaynetComponent)->enable(true);
			});
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			i6engine::api::attributeMap params;
			params.insert(std::make_pair("name", "NPCName01")); // size of the terrain in world units
			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Name", params, "", false, false));
			of->createObject("NPC", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}

		// shows fps (activate/deactive using F1)
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F1, i6engine::api::KeyState::KEY_PRESSED, [this]() {
			if (!_showFPS) {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->showFPS(0.0, 0.0, "RPG/StaticImage", "RPG/Blanko", "RPG", "TbM_Filling");
			} else {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->hideFPS();
			}
			_showFPS = !_showFPS;
		});

		i6engine::api::InputFacade * inputFacade = i6engine::api::EngineController::GetSingleton().getInputFacade();

		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_W, "forward");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_S, "backward");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_A, "left");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_D, "right");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_LCONTROL, "down");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_SPACE, "up");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_Q, "rotateRight");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_E, "rotateLeft");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_DELETE, "rotateLeft");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_INSERT, "rotateRight");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_HOME, "rotateUp");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_END, "rotateDown");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_PGUP, "leanLeft");
		inputFacade->setKeyMapping(i6engine::api::KeyCode::KC_PGDOWN, "leanRight");
	}

	void WaynetApplication::Tick() {
		for (auto & p : _eventMap) {
			if (p.second.second) {
				p.second.first();
			}
		}
	}

	bool WaynetApplication::ShutdownRequest() {
		return true;
	}

	void WaynetApplication::Finalize() {
		ISIXE_UNREGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType);
	}

	void WaynetApplication::ShutDown() {
	}

	void WaynetApplication::News(const i6engine::api::GameMessage::Ptr & msg) {
		if (msg->getSubtype() == i6engine::api::keyboard::KeyKeyboard) { // for movement of the camera
			i6engine::api::input::Input_Keyboard_Update * iku = dynamic_cast<i6engine::api::input::Input_Keyboard_Update *>(msg->getContent());
			if (!i6engine::api::EngineController::GetSingleton().getGUIFacade()->getInputCaptured()) {
				std::string key = i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(iku->code);

				if (_eventMap.find(key) != _eventMap.end()) {
					_eventMap[key].second = iku->pressed != i6engine::api::KeyState::KEY_RELEASED;
				}
			}
		} else if (msg->getMessageType() == i6engine::api::messages::InputMessageType) {
			if (msg->getSubtype() == i6engine::api::mouse::MouseMessageTypes::MouButton) {
				i6engine::api::input::Input_Button_Update * ibu = dynamic_cast<i6engine::api::input::Input_Button_Update *>(msg->getContent());
				if (ibu->code == i6engine::api::MouseButtonID::MB_Left && ibu->pressed) {
					auto vec = i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->getSelectables();
					if (vec.size() > 0) {
						auto go = i6engine::api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("NPC").front();
						go->getGOC<i6engine::api::MoveComponent>(i6engine::api::components::ComponentTypes::MoveComponent)->navigate(vec.front().second);
					}
				}
			}
		}
	}

	void WaynetApplication::Forward() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 0.0, 2.0), ssc->getRotation()));
	}

	void WaynetApplication::Backward() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 0.0, -2.0), ssc->getRotation()));
	}

	void WaynetApplication::Left() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(2.0, 0.0, 0.0), ssc->getRotation()));
	}

	void WaynetApplication::Right() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(-2.0, 0.0, 0.0), ssc->getRotation()));
	}

	void WaynetApplication::Down() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, -2.0, 0.0), ssc->getRotation()));
	}

	void WaynetApplication::Up() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 2.0, 0.0), ssc->getRotation()));
	}

	void WaynetApplication::RotateLeft() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
	}

	void WaynetApplication::RotateRight() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
	}

	void WaynetApplication::RotateUp() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
	}

	void WaynetApplication::RotateDown() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
	}

	void WaynetApplication::LeanLeft() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
	}

	void WaynetApplication::LeanRight() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
	}

} /* namespace sample */
