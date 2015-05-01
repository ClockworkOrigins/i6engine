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

#include "FloatingApplication.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/CameraComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
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

	FloatingApplication::FloatingApplication() : i6engine::api::Application(), _showFPS(false), _camera(), _eventMap() {
		_eventMap["forward"] = std::make_pair(boost::bind(&FloatingApplication::Forward, this), false);
		_eventMap["backward"] = std::make_pair(boost::bind(&FloatingApplication::Backward, this), false);
		_eventMap["left"] = std::make_pair(boost::bind(&FloatingApplication::Left, this), false);
		_eventMap["right"] = std::make_pair(boost::bind(&FloatingApplication::Right, this), false);
		_eventMap["down"] = std::make_pair(boost::bind(&FloatingApplication::Down, this), false);
		_eventMap["up"] = std::make_pair(boost::bind(&FloatingApplication::Up, this), false);
		_eventMap["rotateLeft"] = std::make_pair(boost::bind(&FloatingApplication::RotateLeft, this), false);
		_eventMap["rotateRight"] = std::make_pair(boost::bind(&FloatingApplication::RotateRight, this), false);
		_eventMap["rotateUp"] = std::make_pair(boost::bind(&FloatingApplication::RotateUp, this), false);
		_eventMap["rotateDown"] = std::make_pair(boost::bind(&FloatingApplication::RotateDown, this), false);
		_eventMap["leanLeft"] = std::make_pair(boost::bind(&FloatingApplication::LeanLeft, this), false);
		_eventMap["leanRight"] = std::make_pair(boost::bind(&FloatingApplication::LeanRight, this), false);
	}

	FloatingApplication::~FloatingApplication() {
	}

	void FloatingApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType, FloatingApplication::InputMailbox, this);
	}

	void FloatingApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// sets gravity for the game... here like on earth
		i6engine::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(0, -9.81, 0));

		// ambient light for the scene
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(1.0, 1.0, 1.0);

		// setting shadow technique... currently only additive stencil possible
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setShadowTechnique(i6engine::api::graphics::ShadowTechnique::Stencil_Additive);

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));

		// register F12 to take screenshot
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F12, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::GraphicsFacade::takeScreenshot, i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade(), "JengaScreen_", ".jpg"));

		// shows fps (activate/deactive using F1)
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F1, i6engine::api::KeyState::KEY_PRESSED, [this]() {
			if (!_showFPS) {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->showFPS(0.0, 0.0, "RPG/StaticImage", "RPG/Blanko", "RPG", "TbM_Filling");
			} else {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->hideFPS();
			}
			_showFPS = !_showFPS;
		});

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("pos", "0 5 -20"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("StaticState", params, "", false, false));
			}
			of->createGO("SpectatorCam", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this](i6engine::api::GOPtr go) {
				_camera = go;
			});
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Floor", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Sun", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		resetScene();

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
		inputFacade->subscribeKeyEvent(i6engine::api::KeyCode::KC_RETURN, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&FloatingApplication::resetScene, this));
	}

	void FloatingApplication::Tick() {
		for (auto & p : _eventMap) {
			if (p.second.second) {
				p.second.first();
			}
		}
	}

	bool FloatingApplication::ShutdownRequest() {
		return true;
	}

	void FloatingApplication::Finalize() {
	}

	void FloatingApplication::ShutDown() {
	}

	void FloatingApplication::InputMailbox(const i6engine::api::GameMessage::Ptr & msg) {
		if (msg->getSubtype() == i6engine::api::keyboard::KeyKeyboard) {
			i6engine::api::input::Input_Keyboard_Update * iku = dynamic_cast<i6engine::api::input::Input_Keyboard_Update *>(msg->getContent());
			if (!i6engine::api::EngineController::GetSingleton().getGUIFacade()->getInputCaptured()) {
				std::string key = i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(iku->code);

				if (_eventMap.find(key) != _eventMap.end()) {
					_eventMap[key].second = iku->pressed != i6engine::api::KeyState::KEY_RELEASED;
				}
			}
		} else if (msg->getSubtype() == i6engine::api::mouse::MouButton) {
			i6engine::api::input::Input_Button_Update * ibu = dynamic_cast<i6engine::api::input::Input_Button_Update *>(msg->getContent());
			if (ibu->pressed && ibu->code == i6engine::api::MouseButtonID::MB_Left && !i6engine::api::EngineController::GetSingleton().getGUIFacade()->getOnWindow()) {
				i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
				Vec3 pos = ssc->getPosition();

				i6engine::api::attributeMap paramsPSC;
				pos.insertInMap("pos", paramsPSC);
				i6engine::api::objects::GOTemplate tmpl;
				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));
				i6engine::api::EngineController::GetSingleton().getObjectFacade()->createGO("Ball", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false, [this, ssc](i6engine::api::GOPtr go) {
					auto cc = _camera->getGOC<i6engine::api::CameraComponent>(i6engine::api::components::ComponentTypes::CameraComponent);
					go->getGOC<i6engine::api::PhysicalStateComponent>(i6engine::api::components::ComponentTypes::PhysicalStateComponent)->setLinearVelocity(i6engine::math::rotateVector((cc->getLookAt() - cc->getPosition() + Vec3(0.0, 0.0, 2.0)).normalize() * 100.0, ssc->getRotation()), 2);
				});
			}
		}
	}

	void FloatingApplication::Forward() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 0.0, 1.0), ssc->getRotation()));
	}

	void FloatingApplication::Backward() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 0.0, -1.0), ssc->getRotation()));
	}

	void FloatingApplication::Left() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(1.0, 0.0, 0.0), ssc->getRotation()));
	}

	void FloatingApplication::Right() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(-1.0, 0.0, 0.0), ssc->getRotation()));
	}

	void FloatingApplication::Down() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, -1.0, 0.0), ssc->getRotation()));
	}

	void FloatingApplication::Up() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 1.0, 0.0), ssc->getRotation()));
	}

	void FloatingApplication::RotateLeft() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
	}

	void FloatingApplication::RotateRight() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
	}

	void FloatingApplication::RotateUp() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
	}

	void FloatingApplication::RotateDown() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
	}

	void FloatingApplication::LeanLeft() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
	}

	void FloatingApplication::LeanRight() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
	}

	void FloatingApplication::resetScene() {
		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		of->deleteAllObjectsOfType("Ballon");
		of->deleteAllObjectsOfType("Plank");
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(0.0, 0.05, 0.0);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank01";
				paramsConstraint["targetIdentifier"] = "Ballon01";
				paramsConstraint["selfOffset"] = "-1.5 0.05 -0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank01", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank02";
				paramsConstraint["targetIdentifier"] = "Ballon02";
				paramsConstraint["selfOffset"] = "-1.5 0.05 0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank02", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank03";
				paramsConstraint["targetIdentifier"] = "Ballon03";
				paramsConstraint["selfOffset"] = "1.5 0.05 -0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank03", false, false));
			}
			{
				i6engine::api::attributeMap paramsConstraint;
				paramsConstraint["selfIdentifier"] = "Plank04";
				paramsConstraint["targetIdentifier"] = "Ballon04";
				paramsConstraint["selfOffset"] = "1.5 0.05 0.75";
				paramsConstraint["targetOffset"] = "0.0 -0.4 0.0";
				paramsConstraint["breakingImpulse"] = "10.0";

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "Plank04", false, false));
			}

			of->createObject("Plank", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(-1.5, 0.45, -0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon01";
			paramsConstraint["targetIdentifier"] = "Plank01";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "-1.5 0.05 -0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(-1.5, 0.45, 0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon02";
			paramsConstraint["targetIdentifier"] = "Plank02";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "-1.5 0.05 0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(1.5, 0.45, -0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon03";
			paramsConstraint["targetIdentifier"] = "Plank03";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "1.5 0.05 -0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		{
			i6engine::api::objects::GOTemplate tmpl;

			Vec3 pos(1.5, 0.45, 0.75);

			i6engine::api::attributeMap paramsPSC;
			pos.insertInMap("pos", paramsPSC);
			paramsPSC.insert(std::make_pair("gravity", "0.0 26.0 0.0"));

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", paramsPSC, "", false, false));

			i6engine::api::attributeMap paramsConstraint;
			paramsConstraint["selfIdentifier"] = "Ballon04";
			paramsConstraint["targetIdentifier"] = "Plank04";
			paramsConstraint["selfOffset"] = "0.0 -0.4 0.0";
			paramsConstraint["targetOffset"] = "1.5 0.05 0.75";
			paramsConstraint["breakingImpulse"] = "10.0";

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Point2PointConstraint", paramsConstraint, "", false, false));

			of->createObject("Ballon", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
	}

} /* namespace sample */
