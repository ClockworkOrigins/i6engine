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

#include "CompositorApplication.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/CameraComponent.h"
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

	CompositorApplication::CompositorApplication() : i6engine::api::Application(), _showFPS(false), _camera(), _eventMap() {
		_eventMap["forward"] = std::make_pair(boost::bind(&CompositorApplication::Forward, this), false);
		_eventMap["backward"] = std::make_pair(boost::bind(&CompositorApplication::Backward, this), false);
		_eventMap["left"] = std::make_pair(boost::bind(&CompositorApplication::Left, this), false);
		_eventMap["right"] = std::make_pair(boost::bind(&CompositorApplication::Right, this), false);
		_eventMap["down"] = std::make_pair(boost::bind(&CompositorApplication::Down, this), false);
		_eventMap["up"] = std::make_pair(boost::bind(&CompositorApplication::Up, this), false);
		_eventMap["rotateLeft"] = std::make_pair(boost::bind(&CompositorApplication::RotateLeft, this), false);
		_eventMap["rotateRight"] = std::make_pair(boost::bind(&CompositorApplication::RotateRight, this), false);
		_eventMap["rotateUp"] = std::make_pair(boost::bind(&CompositorApplication::RotateUp, this), false);
		_eventMap["rotateDown"] = std::make_pair(boost::bind(&CompositorApplication::RotateDown, this), false);
		_eventMap["leanLeft"] = std::make_pair(boost::bind(&CompositorApplication::LeanLeft, this), false);
		_eventMap["leanRight"] = std::make_pair(boost::bind(&CompositorApplication::LeanRight, this), false);
	}

	CompositorApplication::~CompositorApplication() {
	}

	void CompositorApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(i6engine::api::messages::InputMessageType, CompositorApplication::InputMailbox, this);
	}

	void CompositorApplication::AfterInitialize() {
		i6engine::api::GUIFacade * gf = i6engine::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// sets gravity for the game... here like on earth
		i6engine::api::EngineController::GetSingletonPtr()->getPhysicsFacade()->setGravity(Vec3(0, -9.81, 0));

		// ambient light for the scene
		i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->setAmbientLight(0.0, 0.0, 0.0);

		// register ESC to close the application
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_ESCAPE, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::EngineController::stop, i6engine::api::EngineController::GetSingletonPtr()));

		// register F12 to take screenshot
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F12, i6engine::api::KeyState::KEY_PRESSED, boost::bind(&i6engine::api::GraphicsFacade::takeScreenshot, i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade(), "TerrainScreen_", ".jpg"));

		// shows fps (activate/deactive using F1)
		i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6engine::api::KeyCode::KC_F1, i6engine::api::KeyState::KEY_PRESSED, [this]() {
			if (!_showFPS) {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->showFPS(0.0, 0.0, "RPG/StaticImage", "RPG/Blanko", "RPG", "TbM_Filling");
			} else {
				i6engine::api::EngineController::GetSingletonPtr()->getGraphicsFacade()->hideFPS();
			}
			_showFPS = !_showFPS;
		});

		// add ToggleButtons for different compositors
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addToggleButton("Bloom", "RPG/ToggleButton", 0.85, 0.0, 0.02, 0.02, false, [this](bool b) {
			setCompositor("Bloom", b);
		});
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint("BloomText", "RPG/Blanko", 0.87, 0.0, "Bloom", i6engine::api::gui::Alignment::Left, -1);
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addToggleButton("GaussianBlur", "RPG/ToggleButton", 0.85, 0.03, 0.02, 0.02, false, [this](bool b) {
			setCompositor("GaussianBlur", b);
		});
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint("GaussianBlurText", "RPG/Blanko", 0.87, 0.03, "Gaussian Blur", i6engine::api::gui::Alignment::Left, -1);
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addToggleButton("HeatVision", "RPG/ToggleButton", 0.85, 0.06, 0.02, 0.02, false, [this](bool b) {
			setCompositor("HeatVision", b);
		});
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint("HeatVisionText", "RPG/Blanko", 0.87, 0.06, "Heat Vision", i6engine::api::gui::Alignment::Left, -1);
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addToggleButton("HDR", "RPG/ToggleButton", 0.85, 0.09, 0.02, 0.02, false, [this](bool b) {
			setCompositor("HDR", b);
		});
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint("HDRText", "RPG/Blanko", 0.87, 0.09, "HDR", i6engine::api::gui::Alignment::Left, -1);
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addToggleButton("MotionBlur", "RPG/ToggleButton", 0.85, 0.12, 0.02, 0.02, false, [this](bool b) {
			setCompositor("MotionBlur", b);
		});
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint("MotionBlurText", "RPG/Blanko", 0.87, 0.12, "Motion Blur", i6engine::api::gui::Alignment::Left, -1);

		i6engine::api::ObjectFacade * of = i6engine::api::EngineController::GetSingleton().getObjectFacade();
		// a camera to see the scene and move around
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
		// a ground so we can see shadows on it
		{
			i6engine::api::objects::GOTemplate tmpl;
			of->createObject("Floor", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		// one static spotlight
		{
			i6engine::api::objects::GOTemplate tmpl;
			i6engine::api::attributeMap params;
			params.insert(std::make_pair("lightType", "2"));
			params.insert(std::make_pair("spotLightRangeInner", "30.0"));
			params.insert(std::make_pair("spotLightRangeOuter", "50.0"));
			params.insert(std::make_pair("pos", "1500 1750 1300"));
			params.insert(std::make_pair("diffuseColour", "0.35 0.35 0.38"));
			params.insert(std::make_pair("specularColour", "0.9 0.9 1"));
			params.insert(std::make_pair("attenuation", "100000 1 0 0"));
			Vec3(-1500, -1750, -1300).normalize().insertInMap("direction", params);

			tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("LuminousAppearance", params, "", false, false));

			of->createObject("Light", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		// one dynamic point light being moved around by a mover
		{
			i6engine::api::objects::GOTemplate tmpl;
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("lightType", "0"));
				params.insert(std::make_pair("diffuseColour", "0.35 0.35 0.38"));
				params.insert(std::make_pair("specularColour", "1 1 1"));
				params.insert(std::make_pair("attenuation", "80 1 0.0005 0"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("LuminousAppearance", params, "", false, false));
			}
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("pos", "0 0 0"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
			}
			{
				i6engine::api::attributeMap params;
				params.insert(std::make_pair("keyframes", "11"));
				params.insert(std::make_pair("keyframe_0_pos", "3 7.5 -7"));
				params.insert(std::make_pair("keyframe_0_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_1_pos", "1.5 8 -2.5"));
				params.insert(std::make_pair("keyframe_1_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_2_pos", "-1.5 8.5 -1"));
				params.insert(std::make_pair("keyframe_2_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_3_pos", "-4 7 -2"));
				params.insert(std::make_pair("keyframe_3_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_4_pos", "-2 7 -4"));
				params.insert(std::make_pair("keyframe_4_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_5_pos", "-1 8.5 -2"));
				params.insert(std::make_pair("keyframe_5_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_6_pos", "-1 5.75 1.8"));
				params.insert(std::make_pair("keyframe_6_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_7_pos", "0 7.5 3"));
				params.insert(std::make_pair("keyframe_7_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_8_pos", "1 8.5 1.0"));
				params.insert(std::make_pair("keyframe_8_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_9_pos", "2.5 8 0"));
				params.insert(std::make_pair("keyframe_9_rot", "1 0 0 0"));
				params.insert(std::make_pair("keyframe_10_pos", "3 7.5 -7"));
				params.insert(std::make_pair("keyframe_10_rot", "1 0 0 0"));

				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("MoverInterpolate", params, "", false, false));
			}

			of->createObject("MoverLight", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
		}
		// some columns casting shadows
		for (int x = -2; x <= 2; ++x) {
			for (int z = -2; z <= 2; ++z) {
				if (x != 0 || z != 0) {
					i6engine::api::objects::GOTemplate tmpl;
					i6engine::api::attributeMap params;
					Vec3 pos(x * 3.0, 1.5, z * 3.0);
					pos.insertInMap("pos", params);

					tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("PhysicalState", params, "", false, false));
					of->createObject("Column", tmpl, i6engine::api::EngineController::GetSingleton().getUUID(), false);
				}
			}

		}

		// registration of movements
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

	void CompositorApplication::Tick() {
		for (auto & p : _eventMap) {
			if (p.second.second) {
				p.second.first();
			}
		}
	}

	bool CompositorApplication::ShutdownRequest() {
		return true;
	}

	void CompositorApplication::Finalize() {
	}

	void CompositorApplication::ShutDown() {
	}

	void CompositorApplication::InputMailbox(const i6engine::api::GameMessage::Ptr & msg) {
		if (msg->getSubtype() == i6engine::api::keyboard::KeyKeyboard) { // for movement of the camera
			i6engine::api::input::Input_Keyboard_Update * iku = dynamic_cast<i6engine::api::input::Input_Keyboard_Update *>(msg->getContent());
			if (!i6engine::api::EngineController::GetSingleton().getGUIFacade()->getInputCaptured()) {
				std::string key = i6engine::api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(iku->code);

				if (_eventMap.find(key) != _eventMap.end()) {
					_eventMap[key].second = iku->pressed != i6engine::api::KeyState::KEY_RELEASED;
				}
			}
		}
	}

	void CompositorApplication::Forward() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 0.0, 2.0), ssc->getRotation()));
	}

	void CompositorApplication::Backward() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 0.0, -2.0), ssc->getRotation()));
	}

	void CompositorApplication::Left() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(2.0, 0.0, 0.0), ssc->getRotation()));
	}

	void CompositorApplication::Right() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(-2.0, 0.0, 0.0), ssc->getRotation()));
	}

	void CompositorApplication::Down() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, -2.0, 0.0), ssc->getRotation()));
	}

	void CompositorApplication::Up() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setPosition(ssc->getPosition() + i6engine::math::rotateVector(Vec3(0.0, 2.0, 0.0), ssc->getRotation()));
	}

	void CompositorApplication::RotateLeft() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
	}

	void CompositorApplication::RotateRight() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
	}

	void CompositorApplication::RotateUp() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
	}

	void CompositorApplication::RotateDown() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
	}

	void CompositorApplication::LeanLeft() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
	}

	void CompositorApplication::LeanRight() {
		i6engine::utils::sharedPtr<i6engine::api::StaticStateComponent, i6engine::api::Component> ssc = _camera->getGOC<i6engine::api::StaticStateComponent>(i6engine::api::components::StaticStateComponent);
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
	}

	void CompositorApplication::setCompositor(const std::string & compositor, bool enabled) const {
		_camera->getGOC<i6engine::api::CameraComponent>(i6engine::api::components::ComponentTypes::CameraComponent)->enableCompositor(compositor, enabled);
	}

} /* namespace sample */
