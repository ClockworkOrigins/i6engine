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

#define _GLIBCXX_USE_NANOSLEEP

#include "i6engine/editor/Editor.h"

#include <chrono>
#include <thread>

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/MovableTextComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/WaypointComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/WaynetManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "tinyxml2.h"

namespace i6engine {
namespace editor {

	Editor::Editor() : Application(), _iniParser(), _movementSpeed(1.0), _camera(), _eventMap(), _inLevel(false), _selectedObjectID(-1), _freeFlyMode(false), _moveObject(false), _lastX(), _lastY(), _lastNearWaypoints(), _removeBox(false) {
		_eventMap["forward"] = std::make_pair(boost::bind(&Editor::Forward, this), false);
		_eventMap["backward"] = std::make_pair(boost::bind(&Editor::Backward, this), false);
		_eventMap["left"] = std::make_pair(boost::bind(&Editor::Left, this), false);
		_eventMap["right"] = std::make_pair(boost::bind(&Editor::Right, this), false);
		_eventMap["down"] = std::make_pair(boost::bind(&Editor::Down, this), false);
		_eventMap["up"] = std::make_pair(boost::bind(&Editor::Up, this), false);
		_eventMap["rotateLeft"] = std::make_pair(boost::bind(&Editor::RotateLeft, this), false);
		_eventMap["rotateRight"] = std::make_pair(boost::bind(&Editor::RotateRight, this), false);
		_eventMap["rotateUp"] = std::make_pair(boost::bind(&Editor::RotateUp, this), false);
		_eventMap["rotateDown"] = std::make_pair(boost::bind(&Editor::RotateDown, this), false);
		_eventMap["leanLeft"] = std::make_pair(boost::bind(&Editor::LeanLeft, this), false);
		_eventMap["leanRight"] = std::make_pair(boost::bind(&Editor::LeanRight, this), false);
		_eventMap["scaleUp"] = std::make_pair(boost::bind(&Editor::ScaleUp, this), false);
		_eventMap["scaleDown"] = std::make_pair(boost::bind(&Editor::ScaleDown, this), false);
		_eventMap["scaleXUp"] = std::make_pair(boost::bind(&Editor::ScaleXUp, this), false);
		_eventMap["scaleXDown"] = std::make_pair(boost::bind(&Editor::ScaleXDown, this), false);
		_eventMap["scaleYUp"] = std::make_pair(boost::bind(&Editor::ScaleYUp, this), false);
		_eventMap["scaleYDown"] = std::make_pair(boost::bind(&Editor::ScaleYDown, this), false);
		_eventMap["scaleZUp"] = std::make_pair(boost::bind(&Editor::ScaleZUp, this), false);
		_eventMap["scaleZDown"] = std::make_pair(boost::bind(&Editor::ScaleZDown, this), false);

		if (clockUtils::ClockError::SUCCESS != _iniParser.load("i6engine-editor.ini")) {
			ISIXE_THROW_FAILURE("Editor", "i6engine-editor.ini not found!");
		}

		if (clockUtils::ClockError::SUCCESS != _iniParser.getValue("GENERAL", "movementSpeed", _movementSpeed)) {
			ISIXE_THROW_FAILURE("Editor", "Entry for movement speed does not exist or is corrupted!");
		}
	}

	Editor::~Editor() {
		_iniParser.setValue("GENERAL", "movementSpeed", _movementSpeed);

		if (clockUtils::ClockError::SUCCESS != _iniParser.save("i6engine-editor.ini")) {
			ISIXE_THROW_FAILURE("Editor", "i6engine-editor.ini couldn't be saved!");
		}
	}

	void Editor::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, Editor::InputMailbox, this);
	}

	void Editor::AfterInitialize() {
		api::InputFacade * inputFacade = api::EngineController::GetSingleton().getInputFacade();

		inputFacade->setKeyMapping(api::KeyCode::KC_W, "forward");
		inputFacade->setKeyMapping(api::KeyCode::KC_S, "backward");
		inputFacade->setKeyMapping(api::KeyCode::KC_A, "left");
		inputFacade->setKeyMapping(api::KeyCode::KC_D, "right");
		inputFacade->setKeyMapping(api::KeyCode::KC_LCONTROL, "down");
		inputFacade->setKeyMapping(api::KeyCode::KC_SPACE, "up");
		inputFacade->setKeyMapping(api::KeyCode::KC_DELETE, "rotateLeft");
		inputFacade->setKeyMapping(api::KeyCode::KC_INSERT, "rotateRight");
		inputFacade->setKeyMapping(api::KeyCode::KC_HOME, "rotateUp");
		inputFacade->setKeyMapping(api::KeyCode::KC_END, "rotateDown");
		inputFacade->setKeyMapping(api::KeyCode::KC_PGUP, "leanLeft");
		inputFacade->setKeyMapping(api::KeyCode::KC_PGDOWN, "leanRight");
		inputFacade->setKeyMapping(api::KeyCode::KC_ADD, "scaleUp");
		inputFacade->setKeyMapping(api::KeyCode::KC_SUBTRACT, "scaleDown");
		inputFacade->setKeyMapping(api::KeyCode::KC_NUMPAD1, "scaleXDown");
		inputFacade->setKeyMapping(api::KeyCode::KC_NUMPAD2, "scaleXUp");
		inputFacade->setKeyMapping(api::KeyCode::KC_NUMPAD4, "scaleYDown");
		inputFacade->setKeyMapping(api::KeyCode::KC_NUMPAD5, "scaleYUp");
		inputFacade->setKeyMapping(api::KeyCode::KC_NUMPAD7, "scaleZDown");
		inputFacade->setKeyMapping(api::KeyCode::KC_NUMPAD8, "scaleZUp");

		inputFacade->setKeyMapping(api::KeyCode::KC_F3, "freeFly");
		inputFacade->setKeyMapping(api::KeyCode::KC_M, "moveObject");
		inputFacade->setKeyMapping(api::KeyCode::KC_F4, "toggleWaynet");
		inputFacade->setKeyMapping(api::KeyCode::KC_DELETE, "removeObject");
	}

	bool Editor::ShutdownRequest() {
		return true;
	}

	void Editor::Finalize() {
	}

	void Editor::Tick() {
		if (!_inLevel || _camera.get() == nullptr) {
			return;
		}
		for (auto & p : _eventMap) {
			if (p.second.second) {
				p.second.first();
			}
		}
	}

	void Editor::loadLevel(const std::string & file) {
		_camera = api::GOPtr();

		for (auto & p : _eventMap) {
			p.second.second = false;
		}

		std::thread([this, file]() {
			api::EngineController::GetSingletonPtr()->getObjectFacade()->cleanUpAll();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			api::EngineController::GetSingletonPtr()->getGraphicsFacade()->resetSubSystem();
			api::EngineController::GetSingletonPtr()->getGUIFacade()->resetSubSystem();
			api::EngineController::GetSingletonPtr()->getInputFacade()->resetSubSystem();
			api::EngineController::GetSingletonPtr()->getObjectFacade()->resetSubSystem();
			api::EngineController::GetSingletonPtr()->getPhysicsFacade()->resetSubSystem();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			std::string flags = "";

			std::vector<std::string> levelFlags = getLevelFlags();

			for (size_t i = 0; i < levelFlags.size(); i++) {
				flags += levelFlags[i];

				if (i < levelFlags.size() - 1) {
					flags += "|";
				}
			}

			api::EngineController::GetSingletonPtr()->getObjectFacade()->loadLevel(file, flags);

			api::EngineController::GetSingletonPtr()->getGraphicsFacade()->getHighestCoordinate(Vec3::ZERO, [this](Vec3 pos) {
				i6engine::api::objects::GOTemplate tmpl;
				i6engine::api::attributeMap paramsSSC;
				(pos + Vec3(0.0, 2.0, 0.0)).insertInMap("pos", paramsSSC);
				i6engine::api::attributeMap paramsCamera;
				(pos + Vec3(0.0, 2.0, 1.0)).insertInMap("lookAt", paramsCamera);
				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));
				tmpl._components.push_back(i6engine::api::objects::GOTemplateComponent("Camera", paramsCamera, "", false, false));
				api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO("EditorCam", tmpl, api::EngineController::GetSingleton().getUUID(), false, boost::bind(&Editor::setCamera, this, _1));
			});

			_inLevel = true;

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			updateObjectList();
		}).detach();
	}

	void Editor::saveLevel(const std::string & level) {
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLDeclaration * decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		doc.InsertFirstChild(decl);

		tinyxml2::XMLElement * root = doc.NewElement("Map");
		doc.InsertAfterChild(decl, root);

		std::map<std::string, std::vector<api::GOPtr>> sectionMap;

		for (auto & go : api::EngineController::GetSingleton().getObjectFacade()->getGOList()) {
			if (go->getType() == "EditorCam") {
				continue;
			}
			std::string flagString;

			for (size_t i = 0; i < go->getFlags().size(); i++) {
				flagString += go->getFlags()[i];

				if (i < go->getFlags().size() - 1) {
					flagString += "|";
				}
			}

			if (sectionMap.find(flagString) == sectionMap.end()) {
				sectionMap.insert(std::make_pair(flagString, std::vector<api::GOPtr>()));
			}

			sectionMap[flagString].push_back(go);
		}

		for (auto & p : sectionMap) {
			tinyxml2::XMLElement * section = doc.NewElement("Section");

			section->SetAttribute("flags", p.first.c_str());

			for (auto & go : p.second) {
				tinyxml2::XMLElement * gameObject = doc.NewElement("GameObject");
				gameObject->SetAttribute("name", go->getType().c_str());

				if (go->getSend()) {
					gameObject->SetAttribute("send", "true");
				} else {
					gameObject->SetAttribute("send", "false");
				}

				for (auto & c : go->getGOCList()) {
					if (go->getType() == "Waypoint" && (c->getTemplateName() == "MovableText" || c->getTemplateName() == "MeshAppearance")) {
						continue;
					}
					tinyxml2::XMLElement * component = doc.NewElement("Component");
					component->SetAttribute("template", c->getTemplateName().c_str());

					for (auto am : c->synchronize()) {
						tinyxml2::XMLElement * attribute = doc.NewElement("Attribute");
						attribute->SetAttribute("name", am.first.c_str());

						tinyxml2::XMLText * attributeValue = doc.NewText(am.second.c_str());

						attribute->LinkEndChild(attributeValue);

						component->LinkEndChild(attribute);
					}

					gameObject->LinkEndChild(component);
				}

				section->LinkEndChild(gameObject);

				root->LinkEndChild(section);
			}
		}

		doc.SaveFile(level.c_str());
	}

	void Editor::InputMailbox(const api::GameMessage::Ptr & msg) {
		if (msg->getSubtype() == api::keyboard::KeyKeyboard) {
			api::input::Input_Keyboard_Update * iku = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent());
			if (!api::EngineController::GetSingleton().getGUIFacade()->getInputCaptured()) {
				std::string key = api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(iku->code);

				if (_eventMap.find(key) != _eventMap.end()) {
					_eventMap[key].second = iku->pressed != api::KeyState::KEY_RELEASED;
				} else if (key == "freeFly") {
					if (iku->pressed == api::KeyState::KEY_PRESSED) {
						_freeFlyMode = !_freeFlyMode;
					}
				} else if (key == "moveObject") {
					if (iku->pressed == api::KeyState::KEY_PRESSED) {
						if (_moveObject) {
							_moveObject = false;
						} else {
							if (_selectedObjectID != -1) {
								_moveObject = true;
							}
						}
					}
				} else if (key == "removeObject") {
					if (iku->pressed == api::KeyState::KEY_PRESSED) {
						removeObject();
					}
				} else if (iku->pressed == api::KeyState::KEY_PRESSED && iku->code == api::KeyCode::KC_MBLeft && !api::EngineController::GetSingleton().getGUIFacade()->getOnWindow()) {
					auto targetList = api::EngineController::GetSingleton().getGraphicsFacade()->getSelectables();
					for (auto & p : targetList) {
						auto go = api::EngineController::GetSingleton().getObjectFacade()->getObject(p.first);
						if (go != nullptr && go->getType() != "EditorCam") {
							selectObject(p.first);
							break;
						}
					}
				} else if (iku->pressed == api::KeyState::KEY_PRESSED && iku->code == api::KeyCode::KC_MBMiddle && !api::EngineController::GetSingleton().getGUIFacade()->getOnWindow()) {
					auto targetList = api::EngineController::GetSingleton().getGraphicsFacade()->getSelectables();

					for (auto & p : targetList) {
						auto go = api::EngineController::GetSingleton().getObjectFacade()->getObject(p.first);

						if (go != nullptr && go->getType() != "EditorCam") {
							auto selected = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
							if (go->getType() == "Waypoint" && selected != nullptr && selected->getType() == "Waypoint") {
								auto newWC = go->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent);
								auto selectedWC = selected->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent);
								if (newWC->isConnected(selectedWC->getName())) {
									newWC->removeConnection(selectedWC->getName());
									selectedWC->removeConnection(newWC->getName());
								} else {
									newWC->addConnection(selectedWC->getName());
									selectedWC->addConnection(newWC->getName());
								}
								selectObject(_selectedObjectID);
								api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
							}
							break;
						}
					}
				} else if (iku->pressed == api::KeyState::KEY_PRESSED && iku->code == api::KeyCode::KC_MBRight) {
					selectObject(-1);
				}
			}
		} else if (msg->getSubtype() == api::mouse::MouMouse) {
			api::input::Input_Mouse_Update * imu = dynamic_cast<api::input::Input_Mouse_Update *>(msg->getContent());
			if (_freeFlyMode) {
				if (_lastX < imu->intNewX) {
					RotateLeft();
				} else if (_lastX > imu->intNewX) {
					RotateRight();
				}
				if (_lastY < imu->intNewY) {
					RotateDown();
				} else if (_lastY > imu->intNewY) {
					RotateUp();
				}
				_lastX = imu->intNewX;
				_lastY = imu->intNewY;
			} else {
				_lastX = imu->intNewX;
				_lastY = imu->intNewY;
			}
		}
	}

	void Editor::Forward() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, _movementSpeed), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setPosition(objPSC->getPosition() + math::rotateVector(Vec3(0.0, 0.0, _movementSpeed), ssc->getRotation()), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setPosition(objSSC->getPosition() + math::rotateVector(Vec3(0.0, 0.0, _movementSpeed), ssc->getRotation()));
				}
			}
			selectObject(_selectedObjectID);
			if (go->getType() == "Waypoint") {
				api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
			}
		}
	}

	void Editor::Backward() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, -_movementSpeed), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setPosition(objPSC->getPosition() + math::rotateVector(Vec3(0.0, 0.0, -_movementSpeed), ssc->getRotation()), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setPosition(objSSC->getPosition() + math::rotateVector(Vec3(0.0, 0.0, -_movementSpeed), ssc->getRotation()));
				}
			}
			selectObject(_selectedObjectID);
			if (go->getType() == "Waypoint") {
				api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
			}
		}
	}

	void Editor::Left() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(_movementSpeed, 0.0, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setPosition(objPSC->getPosition() + math::rotateVector(Vec3(_movementSpeed, 0.0, 0.0), ssc->getRotation()), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setPosition(objSSC->getPosition() + math::rotateVector(Vec3(_movementSpeed, 0.0, 0.0), ssc->getRotation()));
				}
			}
			selectObject(_selectedObjectID);
			if (go->getType() == "Waypoint") {
				api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
			}
		}
	}

	void Editor::Right() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(-_movementSpeed, 0.0, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setPosition(objPSC->getPosition() + math::rotateVector(Vec3(-_movementSpeed, 0.0, 0.0), ssc->getRotation()), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setPosition(objSSC->getPosition() + math::rotateVector(Vec3(-_movementSpeed, 0.0, 0.0), ssc->getRotation()));
				}
			}
			selectObject(_selectedObjectID);
			if (go->getType() == "Waypoint") {
				api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
			}
		}
	}

	void Editor::Down() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, -_movementSpeed, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setPosition(objPSC->getPosition() + math::rotateVector(Vec3(0.0, -_movementSpeed, 0.0), ssc->getRotation()), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setPosition(objSSC->getPosition() + math::rotateVector(Vec3(0.0, -_movementSpeed, 0.0), ssc->getRotation()));
				}
			}
			selectObject(_selectedObjectID);
			if (go->getType() == "Waypoint") {
				api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
			}
		}
	}

	void Editor::Up() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, _movementSpeed, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setPosition(objPSC->getPosition() + math::rotateVector(Vec3(0.0, _movementSpeed, 0.0), ssc->getRotation()), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setPosition(objSSC->getPosition() + math::rotateVector(Vec3(0.0, _movementSpeed, 0.0), ssc->getRotation()));
				}
			}
			selectObject(_selectedObjectID);
			if (go->getType() == "Waypoint") {
				api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
			}
		}
	}

	void Editor::RotateLeft() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::RotateRight() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::RotateUp() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::RotateDown() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::LeanLeft() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::LeanRight() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
		} else {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleUp() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() + Vec3(0.01, 0.01, 0.01), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() + Vec3(0.01, 0.01, 0.01));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleDown() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() - Vec3(0.01, 0.01, 0.01), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() - Vec3(0.01, 0.01, 0.01));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleXUp() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() + Vec3(0.01, 0.0, 0.0), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() + Vec3(0.01, 0.0, 0.0));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleXDown() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() - Vec3(0.01, 0.0, 0.0), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() - Vec3(0.01, 0.0, 0.0));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleYUp() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() + Vec3(0.0, 0.01, 0.0), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() + Vec3(0.0, 0.01, 0.0));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleYDown() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() - Vec3(0.0, 0.01, 0.0), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() - Vec3(0.0, 0.01, 0.0));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleZUp() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() + Vec3(0.0, 0.0, 0.01), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() + Vec3(0.0, 0.0, 0.01));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::ScaleZDown() {
		if (_moveObject) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setScale(objPSC->getScale() - Vec3(0.0, 0.0, 0.01), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setScale(objSSC->getScale() - Vec3(0.0, 0.0, 0.01));
				}
			}
			selectObject(_selectedObjectID);
		}
	}

	void Editor::updateWaypointNames() {
		std::set<api::WeakGOPtr> removables;
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		for (auto g : _lastNearWaypoints) {
			auto go = g.get();
			if (go == nullptr) {
				removables.insert(go);
			} else if ((ssc->getPosition() - go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent)->getPosition()).length() > 50.0) {
				auto mac = go->getGOC(api::components::MeshAppearanceComponent);
				if (mac != nullptr) {
					mac->setDie();
				}
				auto mtc = go->getGOC(api::components::MovableTextComponent);
				if (mtc != nullptr) {
					mtc->setDie();
				}
				removables.insert(go);
			}
		}
		for (auto g : removables) {
			_lastNearWaypoints.erase(g);
		}
		for (auto go : api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint")) {
			auto wpSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
			if ((ssc->getPosition() - wpSSC->getPosition()).length() <= 50.0 && _lastNearWaypoints.find(go) == _lastNearWaypoints.end()) {
				{
					api::attributeMap params;
					params.insert(std::make_pair("mesh", "Waypoint.MESH"));
					params.insert(std::make_pair("pos", "0.0 0.0 0.0"));
					params.insert(std::make_pair("rot", "1.0 0.0 0.0 0.0"));
					params.insert(std::make_pair("scale", "1.0 1.0 1.0"));
					params.insert(std::make_pair("visibility", "1"));
					api::EngineController::GetSingleton().getObjectFacade()->createComponent(go->getID(), api::EngineController::GetSingleton().getIDManager()->getID(), "MeshAppearance", params);
				}
				{
					api::attributeMap params;
					params.insert(std::make_pair("font", "DejaVuSans"));
					params.insert(std::make_pair("size", "16"));
					params.insert(std::make_pair("colour", "1.0 1.0 1.0"));
					params.insert(std::make_pair("text", go->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent)->getName()));
					api::EngineController::GetSingleton().getObjectFacade()->createComponent(go->getID(), api::EngineController::GetSingleton().getIDManager()->getID(), "MovableText", params);
				}
				_lastNearWaypoints.insert(go);
			}
		}
	}

} /* namespace editor */
} /* namespace i6engine */
