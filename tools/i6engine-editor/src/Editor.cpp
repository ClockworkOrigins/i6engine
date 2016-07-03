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

#define _GLIBCXX_USE_NANOSLEEP

#include "i6engine/editor/Editor.h"

#include <chrono>

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/utils/AutoUpdater.h"
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

namespace i6e {
namespace editor {

	Editor::Editor() : Application(), _iniParser(), _movementSpeed(1.0), _rotationSpeed(1.0), _camera(), _eventMap(), _inLevel(false), _selectedObjectID(-1), _freeFlyMode(false), _moveObject(false), _lastX(), _lastY(), _lastNearWaypoints(), _removeBox(false), _mouseHold(false) {
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
		if (clockUtils::ClockError::SUCCESS != _iniParser.getValue("GENERAL", "rotationSpeed", _rotationSpeed)) {
			ISIXE_THROW_FAILURE("Editor", "Entry for rotation speed does not exist or is corrupted!");
		}
	}

	Editor::~Editor() {
		_iniParser.setValue("GENERAL", "movementSpeed", _movementSpeed);
		_iniParser.setValue("GENERAL", "rotationSpeed", _rotationSpeed);

		if (clockUtils::ClockError::SUCCESS != _iniParser.save("i6engine-editor.ini")) {
			ISIXE_THROW_FAILURE("Editor", "i6engine-editor.ini couldn't be saved!");
		}
	}

	void Editor::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, Editor::InputMailbox, this);
	}

	void Editor::AfterInitialize() {
		api::InputFacade * inputFacade = i6eInputFacade;

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

	void Editor::loadLevel(const std::string & file, const std::function<void(uint16_t)> & callback) {
		_camera = api::GOPtr();

		for (auto & p : _eventMap) {
			p.second.second = false;
		}

		i6eEngineController->registerTimer(0, [this, file, callback]() {
			clearLevel();

			startLoadLevel(file);

			std::string flags = "";

			std::vector<std::string> levelFlags = getLevelFlags();

			for (size_t i = 0; i < levelFlags.size(); i++) {
				flags += levelFlags[i];

				if (i < levelFlags.size() - 1) {
					flags += "|";
				}
			}

			i6eObjectFacade->loadLevel(file, flags, callback);

			for (auto p : i6eObjectFacade->getGOMap()) {
				api::ComPtr c = p.second->getGOC(api::components::ComponentTypes::PhysicalStateComponent);
				if (c != nullptr) {
					c->enableTicking(true);
				}
			}

			api::EngineController::GetSingletonPtr()->getGraphicsFacade()->getHighestCoordinate(Vec3::ZERO, [this](Vec3 pos) {
				api::objects::GOTemplate tmpl;
				api::attributeMap paramsSSC;
				(pos + Vec3(0.0, 2.0, 0.0)).insertInMap("pos", paramsSSC);
				api::attributeMap paramsCamera;
				Vec3(0.0, 0.0, 1.0).insertInMap("lookAt", paramsCamera);
				tmpl._components.push_back(api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));
				tmpl._components.push_back(api::objects::GOTemplateComponent("Camera", paramsCamera, "", false, false));
				i6eObjectFacade->createGO("EditorCam", tmpl, i6eEngineController->getUUID(), false, boost::bind(&Editor::setCamera, this, _1));
			});

			_inLevel = true;

			finishLoadLevel(file);

			finishedProgress();

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			updateObjectList();
			return false;
		}, false, core::JobPriorities::Prio_Medium);
	}

	void Editor::saveLevel(const std::string & level) {
		startLevelWithPlugin(level);
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLDeclaration * decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		doc.InsertFirstChild(decl);

		tinyxml2::XMLElement * root = doc.NewElement("Map");
		doc.InsertAfterChild(decl, root);

		std::map<std::string, std::vector<api::GOPtr>> sectionMap;

		utils::AutoUpdater<int> currentValue = 0;
		currentValue.registerUpdate(std::bind(&Editor::setProgressValue, this, std::placeholders::_1));
		utils::AutoUpdater<int> maxValue = 1;
		maxValue.registerUpdate(std::bind(&Editor::setProgressMaximum, this, std::placeholders::_1));
		maxValue += int(i6eObjectFacade->getGOMap().size());

		for (auto & p : i6eObjectFacade->getGOMap()) {
			if (p.second->getType() == "EditorCam") {
				continue;
			}
			std::string flagString;

			for (size_t i = 0; i < p.second->getFlags().size(); i++) {
				flagString += p.second->getFlags()[i];

				if (i < p.second->getFlags().size() - 1) {
					flagString += "|";
				}
			}

			if (sectionMap.find(flagString) == sectionMap.end()) {
				sectionMap.insert(std::make_pair(flagString, std::vector<api::GOPtr>()));
			}

			sectionMap[flagString].push_back(p.second);
			currentValue++;
			maxValue++;
		}

		for (auto & p : sectionMap) {
			tinyxml2::XMLElement * section = doc.NewElement("Section");

			section->SetAttribute("flags", p.first.c_str());

			for (auto & go : p.second) {
				if (!saveObjectWithPlugin(go, section)) {
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
				}

				currentValue++;
			}
			root->LinkEndChild(section);
		}

		doc.SaveFile(level.c_str());
		finishLevelWithPlugin();
		currentValue++;
		finishedProgress();
	}

	void Editor::clearLevel() {
		i6eObjectFacade->cleanUpAll();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		api::EngineController::GetSingletonPtr()->getGraphicsFacade()->resetSubSystem();
		api::EngineController::GetSingletonPtr()->getGUIFacade()->resetSubSystem();
		api::EngineController::GetSingletonPtr()->getInputFacade()->resetSubSystem();
		i6eObjectFacade->resetSubSystem();
		api::EngineController::GetSingletonPtr()->getPhysicsFacade()->resetSubSystem();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	void Editor::InputMailbox(const api::GameMessage::Ptr & msg) {
		if (msg->getSubtype() == api::keyboard::KeyKeyboard) {
			api::input::Input_Keyboard_Update * iku = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent());
			if (!i6eGUIFacade->getInputCaptured()) {
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
				} else if (iku->pressed == api::KeyState::KEY_PRESSED && iku->code == api::KeyCode::KC_MBLeft && !i6eGUIFacade->getOnWindow() && !_mouseHold) {
					auto targetList = i6eGraphicsFacade->getSelectables();
					for (auto & p : targetList) {
						auto go = i6eObjectFacade->getObject(p.first);
						if (go != nullptr && go->getType() != "EditorCam") {
							selectObject(p.first);
							break;
						}
					}
					_mouseHold = true;
				} else if (iku->pressed == api::KeyState::KEY_RELEASED && iku->code == api::KeyCode::KC_MBLeft && _mouseHold) {
					_mouseHold = false;
				} else if (iku->pressed == api::KeyState::KEY_PRESSED && iku->code == api::KeyCode::KC_MBMiddle && !i6eGUIFacade->getOnWindow()) {
					auto targetList = i6eGraphicsFacade->getSelectables();

					for (auto & p : targetList) {
						auto go = i6eObjectFacade->getObject(p.first);

						if (go != nullptr && go->getType() != "EditorCam") {
							auto selected = i6eObjectFacade->getObject(_selectedObjectID);
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
								i6eEngineController->getWaynetManager()->createWaynet();
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
			} else if (_selectedObjectID != -1 && _mouseHold) {
				if (_lastX < imu->intNewX) {
					Right();
				} else if (_lastX > imu->intNewX) {
					Left();
				}
				if (_lastY < imu->intNewY) {
					Backward();
				} else if (_lastY > imu->intNewY) {
					Forward();
				}
			}
			_lastX = imu->intNewX;
			_lastY = imu->intNewY;
		}
	}

	void Editor::Forward() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, _movementSpeed), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
				i6eEngineController->getWaynetManager()->createWaynet();
			}
			triggerChangedLevel();
		}
	}

	void Editor::Backward() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, -_movementSpeed), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
				i6eEngineController->getWaynetManager()->createWaynet();
			}
			triggerChangedLevel();
		}
	}

	void Editor::Left() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(_movementSpeed, 0.0, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
				i6eEngineController->getWaynetManager()->createWaynet();
			}
			triggerChangedLevel();
		}
	}

	void Editor::Right() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(-_movementSpeed, 0.0, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
				i6eEngineController->getWaynetManager()->createWaynet();
			}
			triggerChangedLevel();
		}
	}

	void Editor::Down() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, -_movementSpeed, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
				i6eEngineController->getWaynetManager()->createWaynet();
			}
			triggerChangedLevel();
		}
	}

	void Editor::Up() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, _movementSpeed, 0.0), ssc->getRotation()));
			updateWaypointNames();
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
				i6eEngineController->getWaynetManager()->createWaynet();
			}
			triggerChangedLevel();
		}
	}

	void Editor::RotateLeft() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), _rotationSpeed  * -(PI / 48)));
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), _rotationSpeed  * -(PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), _rotationSpeed  * -(PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
			triggerChangedLevel();
		}
	}

	void Editor::RotateRight() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), _rotationSpeed * (PI / 48)));
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), _rotationSpeed * (PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), _rotationSpeed * (PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
			triggerChangedLevel();
		}
	}

	void Editor::RotateUp() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), _rotationSpeed  * -(PI / 48)));
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), _rotationSpeed  * -(PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), _rotationSpeed  * -(PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
			triggerChangedLevel();
		}
	}

	void Editor::RotateDown() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), _rotationSpeed * (PI / 48)));
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), _rotationSpeed * (PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), _rotationSpeed * (PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
			triggerChangedLevel();
		}
	}

	void Editor::LeanLeft() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), _rotationSpeed  * -(PI / 48)));
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), _rotationSpeed  * -(PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), _rotationSpeed  * -(PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
			triggerChangedLevel();
		}
	}

	void Editor::LeanRight() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		if (!_moveObject && !_mouseHold) {
			ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), _rotationSpeed * (PI / 48)));
		} else {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
			auto objPSC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);
			if (objPSC != nullptr) {
				objPSC->setRotation(objPSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), _rotationSpeed * (PI / 48)), 2);
			} else {
				auto objSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				if (objSSC != nullptr) {
					objSSC->setRotation(objSSC->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), _rotationSpeed * (PI / 48)));
				}
			}
			selectObject(_selectedObjectID);
			triggerChangedLevel();
		}
	}

	void Editor::ScaleUp() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleDown() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleXUp() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleXDown() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleYUp() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleYDown() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleZUp() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
		}
	}

	void Editor::ScaleZDown() {
		if (_moveObject) {
			api::GOPtr go = i6eObjectFacade->getObject(_selectedObjectID);
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
			triggerChangedLevel();
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
		for (auto go : i6eObjectFacade->getAllObjectsOfType("Waypoint")) {
			auto wpSSC = go->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
			if ((ssc->getPosition() - wpSSC->getPosition()).length() <= 50.0 && _lastNearWaypoints.find(go) == _lastNearWaypoints.end()) {
				{
					api::attributeMap params;
					api::writeAttribute(params, "mesh", "Waypoint.MESH");
					api::writeAttribute(params, "pos", Vec3::ZERO);
					api::writeAttribute(params, "rot", Quaternion::IDENTITY);
					api::writeAttribute(params, "scale", Vec3(1.0, 1.0, 1.0));
					api::writeAttribute(params, "visibility", true);
					i6eObjectFacade->createComponentCallback(go->getID(), i6eEngineController->getIDManager()->getID(), "MeshAppearance", params, [this](api::ComPtr c) {
						if (_selectedObjectID == c->getOwnerGO()->getID()) {
							auto mac = utils::dynamic_pointer_cast<api::MeshAppearanceComponent>(c);
							mac->drawBoundingBox(Vec3(1.0, 0.0, 0.0));
						}
					});
				}
				{
					api::attributeMap params;
					api::writeAttribute(params, "font", "DejaVuSans");
					api::writeAttribute(params, "size", 0.5);
					api::writeAttribute(params, "colour", Vec3(1.0, 1.0, 1.0));
					api::writeAttribute(params, "text", go->getGOC<api::WaypointComponent>(api::components::ComponentTypes::WaypointComponent)->getName());
					api::writeAttribute(params, "pos", Vec3(0.0, 1.0, 0.0));
					i6eObjectFacade->createComponentCallback(go->getID(), i6eEngineController->getIDManager()->getID(), "MovableText", params, [](api::ComPtr c) {
						auto mtc = utils::dynamic_pointer_cast<api::MovableTextComponent>(c);
						mtc->setAutoScaleCallback([](const Vec3 & cameraPos, const Vec3 & textPos) {
							return 0.5 * ((cameraPos - textPos).length() - ((cameraPos - textPos).length() - 10.0) / 1.5) / 10.0;
						});
					});
				}
				_lastNearWaypoints.insert(go);
			}
		}
	}

} /* namespace editor */
} /* namespace i6e */
