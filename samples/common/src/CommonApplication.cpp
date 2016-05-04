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

#include "CommonApplication.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/CameraComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/bind.hpp"

namespace sample {

	CommonApplication::CommonApplication(bool move, bool shootBall) : i6e::api::Application(), _showFPS(false), _camera(), _eventMap(), _move(move), _shootBall(shootBall) {
		_eventMap["forward"] = std::make_pair(boost::bind(&CommonApplication::Forward, this), false);
		_eventMap["backward"] = std::make_pair(boost::bind(&CommonApplication::Backward, this), false);
		_eventMap["left"] = std::make_pair(boost::bind(&CommonApplication::Left, this), false);
		_eventMap["right"] = std::make_pair(boost::bind(&CommonApplication::Right, this), false);
		_eventMap["down"] = std::make_pair(boost::bind(&CommonApplication::Down, this), false);
		_eventMap["up"] = std::make_pair(boost::bind(&CommonApplication::Up, this), false);
		_eventMap["rotateLeft"] = std::make_pair(boost::bind(&CommonApplication::RotateLeft, this), false);
		_eventMap["rotateRight"] = std::make_pair(boost::bind(&CommonApplication::RotateRight, this), false);
		_eventMap["rotateUp"] = std::make_pair(boost::bind(&CommonApplication::RotateUp, this), false);
		_eventMap["rotateDown"] = std::make_pair(boost::bind(&CommonApplication::RotateDown, this), false);
		_eventMap["leanLeft"] = std::make_pair(boost::bind(&CommonApplication::LeanLeft, this), false);
		_eventMap["leanRight"] = std::make_pair(boost::bind(&CommonApplication::LeanRight, this), false);
	}

	CommonApplication::~CommonApplication() {
	}

	void CommonApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6e::api::messages::InputMessageType, CommonApplication::InputMailbox, this);
	}

	void CommonApplication::AfterInitialize() {
		i6e::api::GUIFacade * gf = i6e::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// sets gravity for the game... here like on earth
		i6e::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(0, -9.81, 0));

		// ambient light for the scene
		i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(0.0, 0.0, 0.0);

		// register ESC to close the application
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, boost::bind(&i6e::api::EngineController::stop, i6e::api::EngineController::GetSingletonPtr()));

		// register F12 to take screenshot
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_F12, i6e::api::KeyState::KEY_PRESSED, boost::bind(&i6e::api::GraphicsFacade::takeScreenshot, i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade(), "Screen_", ".jpg"));

		// shows fps (activate/deactive using F1)
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_F1, i6e::api::KeyState::KEY_PRESSED, [this]() {
			if (!_showFPS) {
				i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->showFPS(0.0, 0.0, "RPG/StaticImage", "RPG/Blanko", "RPG", "TbM_Filling");
			} else {
				i6e::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->hideFPS();
			}
			_showFPS = !_showFPS;
		});

		// registration of movements
		i6e::api::InputFacade * inputFacade = i6e::api::EngineController::GetSingleton().getInputFacade();

		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_W, "forward");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_S, "backward");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_A, "left");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_D, "right");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_LCONTROL, "down");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_SPACE, "up");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_Q, "rotateRight");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_E, "rotateLeft");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_DELETE, "rotateLeft");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_INSERT, "rotateRight");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_HOME, "rotateUp");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_END, "rotateDown");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_PGUP, "leanLeft");
		inputFacade->setKeyMapping(i6e::api::KeyCode::KC_PGDOWN, "leanRight");
	}

	void CommonApplication::Tick() {
		for (auto & p : _eventMap) {
			if (p.second.second) {
				p.second.first();
			}
		}
	}

	bool CommonApplication::ShutdownRequest() {
		return true;
	}

	void CommonApplication::Finalize() {
	}

	void CommonApplication::ShutDown() {
	}

	void CommonApplication::InputMailbox(const i6e::api::GameMessage::Ptr & msg) {
		if (_move && msg->getSubtype() == i6e::api::keyboard::KeyKeyboard) { // for movement of the camera
			i6e::api::input::Input_Keyboard_Update * iku = dynamic_cast<i6e::api::input::Input_Keyboard_Update *>(msg->getContent());
			if (!i6e::api::EngineController::GetSingleton().getGUIFacade()->getInputCaptured()) {
				std::string key = i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(iku->code);

				if (_eventMap.find(key) != _eventMap.end()) {
					_eventMap[key].second = iku->pressed != i6e::api::KeyState::KEY_RELEASED;
				} else {
					if (iku->code == i6e::api::KeyCode::KC_MBLeft && !i6e::api::EngineController::GetSingleton().getGUIFacade()->getOnWindow() && iku->pressed == i6e::api::KeyState::KEY_PRESSED && _shootBall) {
						i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
						Vec3 pos = ssc->getPosition();

						i6e::api::attributeMap paramsPSC;
						pos.insertInMap("pos", paramsPSC);
						i6e::api::objects::GOTemplate tmpl;
						tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));
						i6e::api::EngineController::GetSingleton().getObjectFacade()->createGO("Ball", tmpl, i6e::api::EngineController::GetSingleton().getUUID(), false, [this, ssc](i6e::api::GOPtr go) {
							auto cc = _camera.get()->getGOC<i6e::api::CameraComponent>(i6e::api::components::ComponentTypes::CameraComponent);
							go->getGOC<i6e::api::PhysicalStateComponent>(i6e::api::components::ComponentTypes::PhysicalStateComponent)->setLinearVelocity(i6e::math::rotateVector((cc->getLookAt() - cc->getPosition() + Vec3(0.0, 0.0, 2.0)).normalize() * 100.0, ssc->getRotation()), 2);
						});
					}
				}
			}
		}
	}

	void CommonApplication::Forward() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6e::math::rotateVector(Vec3(0.0, 0.0, 2.0), ssc->getRotation()));
	}

	void CommonApplication::Backward() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6e::math::rotateVector(Vec3(0.0, 0.0, -2.0), ssc->getRotation()));
	}

	void CommonApplication::Left() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6e::math::rotateVector(Vec3(2.0, 0.0, 0.0), ssc->getRotation()));
	}

	void CommonApplication::Right() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6e::math::rotateVector(Vec3(-2.0, 0.0, 0.0), ssc->getRotation()));
	}

	void CommonApplication::Down() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6e::math::rotateVector(Vec3(0.0, -2.0, 0.0), ssc->getRotation()));
	}

	void CommonApplication::Up() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6e::math::rotateVector(Vec3(0.0, 2.0, 0.0), ssc->getRotation()));
	}

	void CommonApplication::RotateLeft() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
	}

	void CommonApplication::RotateRight() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
	}

	void CommonApplication::RotateUp() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
	}

	void CommonApplication::RotateDown() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
	}

	void CommonApplication::LeanLeft() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
	}

	void CommonApplication::LeanRight() {
		i6e::utils::sharedPtr<i6e::api::StaticStateComponent, i6e::api::Component> ssc = _camera.get()->getGOC<i6e::api::StaticStateComponent>(i6e::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
	}

} /* namespace sample */
