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
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/audio/AudioController.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/input/InputController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#include "i6engine/editor/EditorMessageTypes.h"
#include "i6engine/editor/gui/FileDialogWidget.h"
#include "i6engine/editor/gui/ListboxWidget.h"
#include "i6engine/editor/gui/MenuBarWidget.h"
#include "i6engine/editor/gui/ObjectInfoWidget.h"

#include "boost/functional/factory.hpp"

#include "tinyxml2.h"

namespace i6engine {
namespace editor {

	Editor::Editor(const std::string & name) : Application(), _camera(), _eventMap(), _inLevel(false), _selectedObjectID(-1), _freeFlyMode(), _lastX(), _lastY() {
		setName(name);

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
	}

	Editor::~Editor() {
	}

	void Editor::start() {
		api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new modules::GraphicsController(), LNG_GRAPHICS_FRAME_TIME);
		api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new modules::ObjectController(), LNG_OBJECT_FRAME_TIME);
		api::EngineController::GetSingletonPtr()->registerSubSystem("Input", new modules::InputController(), LNG_INPUT_FRAME_TIME);
		api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#ifdef ISIXE_WITH_AUDIO
		api::EngineController::GetSingletonPtr()->registerSubSystem("Audio", new modules::AudioController(), LNG_SCRIPTING_FRAME_TIME);
#endif
		api::EngineController::GetSingletonPtr()->registerApplication(*this);

		api::EngineController::GetSingletonPtr()->start(false);
	}

	void Editor::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, Editor::InputMailbox);
	}

	void Editor::AfterInitialize() {
		api::GUIFacade * gf = api::EngineController::GetSingletonPtr()->getGUIFacade();

		// register GUIWidgets
		gf->registerWidgetTemplate("FileDialog", boost::factory<gui::FileDialogWidget *>());
		gf->registerWidgetTemplate("Listbox", boost::factory<gui::ListboxWidget *>());
		gf->registerWidgetTemplate("MenuBar", boost::factory<gui::MenuBarWidget *>());
		gf->registerWidgetTemplate("ObjectInfo", boost::factory<gui::ObjectInfoWidget *>());

		// initialize GUI
		gf->startGUI("Editor.scheme", "", "", "Editor", "MouseArrow");

		// add menu bar
		gf->createWidget("MenuBar", "MenuBar", "");
		gf->setVisibility("MenuBar", true);

		// register submenus
		registerMenu("File", "Load Level", boost::bind(&Editor::chooseLoadLevel, this));
		registerMenu("File", "Save Level", boost::bind(&Editor::chooseSaveLevel, this));
		registerMenu("File", "Exit", boost::bind(&api::EngineController::stop, api::EngineController::GetSingletonPtr()));

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

		inputFacade->setKeyMapping(api::KeyCode::KC_F3, "freeFly");
	}

	bool Editor::ShutdownRequest() {
		return true;
	}

	void Editor::Finalize() {
	}

	void Editor::Tick() {
		for (auto p : _eventMap) {
			if (p.second.second) {
				p.second.first();
			}
		}
	}

	void Editor::registerMenu(const std::string & menu, const std::string & submenu, const boost::function<void(void)> & func) {
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddMenuBarEntry, core::Method::Update, new messages::GUI_MenuBarEntry("MenuBar", menu, submenu, func), i6engine::core::Subsystem::Unknown));
	}

	void Editor::chooseLoadLevel() {
		api::GUIFacade * gf = api::EngineController::GetSingletonPtr()->getGUIFacade();

		gf->createWidget("FileDialog", "FileDialog", "");
		gf->setPosition("FileDialog", 0.2, 0.2);
		gf->setSize("FileDialog", 0.6, 0.6);
		gf->setText("FileDialog", "Load File");

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::SetFileDialogBaseDir, core::Method::Update, new messages::GUI_SetFileDialogBaseDir("FileDialog", getBasePath()), i6engine::core::Subsystem::Unknown));
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddStringCallback, core::Method::Update, new messages::GUI_AddStringCallback("FileDialog", [this](std::string s) {
			loadLevel(s);
		}), i6engine::core::Subsystem::Unknown));
	}

	void Editor::chooseSaveLevel() {
		api::GUIFacade * gf = api::EngineController::GetSingletonPtr()->getGUIFacade();

		gf->createWidget("FileDialog", "FileDialog", "");
		gf->setPosition("FileDialog", 0.2, 0.2);
		gf->setSize("FileDialog", 0.6, 0.6);
		gf->setText("FileDialog", "Save File");

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::SetFileDialogBaseDir, core::Method::Update, new messages::GUI_SetFileDialogBaseDir("FileDialog", getBasePath()), i6engine::core::Subsystem::Unknown));
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddStringCallback, core::Method::Update, new messages::GUI_AddStringCallback("FileDialog", [this](std::string s) {
			saveLevel(s);
		}), i6engine::core::Subsystem::Unknown));
	}

	void Editor::loadLevel(const std::string & file) {
		_camera = api::GOPtr();

		std::thread([this, file]() {
			api::EngineController::GetSingletonPtr()->getGUIFacade()->deleteWidget("FileDialog");

			if (_inLevel) {
				api::EngineController::GetSingletonPtr()->getGUIFacade()->deleteWidget("ObjectList");
			}

			if (_selectedObjectID != -1) {
				api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::RemoveEntries, core::Method::Update, new messages::GUI_RemoveEntries("ObjectList"), i6engine::core::Subsystem::Unknown));
				_selectedObjectID = -1;
			}

			api::EngineController::GetSingletonPtr()->getObjectFacade()->cleanUpAll();
			api::EngineController::GetSingletonPtr()->getPhysicsFacade()->clean();

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

			api::EngineController::GetSingletonPtr()->getObjectFacade()->createGO("EditorCam", api::objects::GOTemplate(), api::EngineController::GetSingleton().getUUID(), false, boost::bind(&Editor::setCamera, this, _1));

			_inLevel = true;

			api::EngineController::GetSingleton().getGUIFacade()->createWidget("ObjectList", "Listbox", "");
			api::EngineController::GetSingleton().getGUIFacade()->setPosition("ObjectList", 0.85, 0.0);
			api::EngineController::GetSingleton().getGUIFacade()->setSize("ObjectList", 0.15, 0.5);
			api::EngineController::GetSingleton().getGUIFacade()->setVisibility("ObjectList", true);

			std::this_thread::sleep_for(std::chrono::milliseconds(500));

			updateObjectList();
		}).detach();
	}

	void Editor::saveLevel(const std::string & level) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->deleteWidget("FileDialog");

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLDeclaration * decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		doc.InsertFirstChild(decl);

		tinyxml2::XMLElement * root = doc.NewElement("Map");
		doc.InsertAfterChild(decl, root);

		std::map<std::string, std::vector<api::GOPtr>> sectionMap;

		for (auto go : api::EngineController::GetSingleton().getObjectFacade()->getGOList()) {
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

		for (auto p : sectionMap) {
			tinyxml2::XMLElement * section = doc.NewElement("Section");

			section->SetAttribute("flags", p.first.c_str());

			for (auto go : p.second) {
				tinyxml2::XMLElement * gameObject = doc.NewElement("GameObject");
				gameObject->SetAttribute("name", go->getType().c_str());

				if (go->getSend()) {
					gameObject->SetAttribute("send", "true");
				} else {
					gameObject->SetAttribute("send", "false");
				}

				for (auto c : go->getGOCList()) {
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
				std::string key = api::EngineController::GetSingletonPtr()->getInputFacade()->getKeyMapping(api::KeyCode(iku->code));

				if (_eventMap.find(key) != _eventMap.end()) {
					_eventMap[key].second = iku->pressed != api::KeyState::KEY_RELEASED;
				} else if (key == "freeFly") {
					if (iku->pressed == api::KeyState::KEY_PRESSED) {
						_freeFlyMode = !_freeFlyMode;
					}
				} else {
					std::cout << iku->code << std::endl;
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
		} else if (msg->getSubtype() == api::mouse::MouButton) {
			api::input::Input_Button_Update * ibu = dynamic_cast<api::input::Input_Button_Update *>(msg->getContent());

			if (ibu->pressed && api::MouseButtonID(ibu->code) == api::MouseButtonID::MB_Left) {
				auto targetList = i6engine::api::EngineController::GetSingleton().getGraphicsFacade()->getSelectables();

				for (auto & p : targetList) {
					auto go = i6engine::api::EngineController::GetSingleton().getObjectFacade()->getObject(p.first);

					if (go != nullptr && go->getType() != "EditorCam") {
						selectObject(p.first);
						break;
					}
				}
			}
		}
	}

	void Editor::updateObjectList() {
		api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::RemoveEntries, core::Method::Update, new messages::GUI_RemoveEntries("ObjectList"), i6engine::core::Subsystem::Unknown));

		for (auto go : api::EngineController::GetSingleton().getObjectFacade()->getGOList()) {
			if (go->getType() != "EditorCam") {
				api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddEntry, core::Method::Update, new messages::GUI_AddEntry("ObjectList", go->getType(), boost::bind(&Editor::selectObject, this, go->getID())), i6engine::core::Subsystem::Unknown));
			}
		}
	}

	void Editor::selectObject(int64_t id) {
		if (_selectedObjectID != -1) {
			api::EngineController::GetSingletonPtr()->getGUIFacade()->deleteWidget("ObjectInfo");
		}
		_selectedObjectID = id;

		api::EngineController::GetSingleton().getGUIFacade()->setSize("ObjectList", 0.15, 0.5);

		api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(id);

		api::EngineController::GetSingleton().getGUIFacade()->createWidget("ObjectInfo", "ObjectInfo", "");
		api::EngineController::GetSingleton().getGUIFacade()->setPosition("ObjectInfo", 0.75, 0.5);
		api::EngineController::GetSingleton().getGUIFacade()->setSize("ObjectInfo", 0.25, 0.5);
		api::EngineController::GetSingleton().getGUIFacade()->setVisibility("ObjectInfo", true);
		api::EngineController::GetSingleton().getGUIFacade()->setText("ObjectInfo", go->getType());
		api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddComponentOption, core::Method::Update, new messages::GUI_AddComponentOption("ObjectInfo", true, "Flags", [go]() {
			std::vector<std::string> flags = go->getFlags();
			std::string flagString;
			for (size_t i = 0; i < flags.size(); i++) {
				flagString += flags[i];

				if (i < flags.size() - 1) {
					flagString += "|";
				}
			}
			return flagString;
		}, [go](std::string s) {
			go->setFlags(utils::split(s, "|"));
			return true;
		}), i6engine::core::Subsystem::Unknown));

		for (auto c : go->getGOCList()) {
			api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddComponent, core::Method::Update, new messages::GUI_AddComponent("ObjectInfo", c->getTemplateName(), c->getIdentifier()), i6engine::core::Subsystem::Unknown));

			for (auto option : c->getComponentOptions()) {
				api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, messages::GUIMessageTypes::AddComponentOption, core::Method::Update, new messages::GUI_AddComponentOption("ObjectInfo", std::get<api::ComponentOptionsParameter::ACCESSSTATE>(option) == api::AccessState::READWRITE, std::get<api::ComponentOptionsParameter::NAME>(option), std::get<api::ComponentOptionsParameter::READFUNC>(option), std::get<api::ComponentOptionsParameter::WRITEFUNC>(option)), i6engine::core::Subsystem::Unknown));
			}
		}

		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));

		utils::sharedPtr<api::PhysicalStateComponent, api::Component> psc = utils::dynamic_pointer_cast<api::PhysicalStateComponent>(go->getGOC(api::components::PhysicalStateComponent));
		if (psc != nullptr) {
			ssc->setPosition(psc->getPosition() - Vec3(0.0, 0.0, -2.0));
			return;
		}

		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc2 = utils::dynamic_pointer_cast<api::StaticStateComponent>(go->getGOC(api::components::StaticStateComponent));
		if (ssc2 != nullptr) {
			ssc->setPosition(ssc2->getPosition() - Vec3(0.0, 0.0, -2.0));
			return;
		}
	}

	void Editor::Forward() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, 1.0), ssc->getRotation()));
	}

	void Editor::Backward() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, -1.0), ssc->getRotation()));
	}

	void Editor::Left() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(1.0, 0.0, 0.0), ssc->getRotation()));
	}

	void Editor::Right() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(-1.0, 0.0, 0.0), ssc->getRotation()));
	}

	void Editor::Down() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, -1.0, 0.0), ssc->getRotation()));
	}

	void Editor::Up() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setPosition(ssc->getPosition() + math::rotateVector(Vec3(0.0, 1.0, 0.0), ssc->getRotation()));
	}

	void Editor::RotateLeft() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), -(PI / 48)));
	}

	void Editor::RotateRight() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (PI / 48)));
	}

	void Editor::RotateUp() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), -(PI / 48)));
	}

	void Editor::RotateDown() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(1.0, 0.0, 0.0), (PI / 48)));
	}

	void Editor::LeanLeft() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), -(PI / 48)));
	}

	void Editor::LeanRight() {
		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = utils::dynamic_pointer_cast<api::StaticStateComponent>(_camera->getGOC(api::components::StaticStateComponent));
		ssc->setRotation(ssc->getRotation() * Quaternion(Vec3(0.0, 0.0, 1.0), (PI / 48)));
	}

} /* namespace editor */
} /* namespace i6engine */
