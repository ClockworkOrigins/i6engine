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

#include "i6engine/modules/gui/GUIManager.h"

#include "i6engine/utils/EnumClassStream.h"
#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/gui/GUIWidget.h"

#include "i6engine/modules/gui/GUICanvas.h"
#include "i6engine/modules/gui/guiwidgets/GUIBar.h"
#include "i6engine/modules/gui/guiwidgets/GUIChat.h"
#include "i6engine/modules/gui/guiwidgets/GUIComboBox.h"
#include "i6engine/modules/gui/guiwidgets/GUIEditbox.h"
#include "i6engine/modules/gui/guiwidgets/GUIImage.h"
#include "i6engine/modules/gui/guiwidgets/GUIImageButton.h"
#include "i6engine/modules/gui/guiwidgets/GUIMultiColumnList.h"
#include "i6engine/modules/gui/guiwidgets/GUIPrint.h"
#include "i6engine/modules/gui/guiwidgets/GUIStatusList.h"
#include "i6engine/modules/gui/guiwidgets/GUITextButton.h"
#include "i6engine/modules/gui/guiwidgets/GUIToggleButton.h"
#include "i6engine/modules/gui/guiwidgets/GUITooltip.h"

#include "boost/filesystem.hpp"
#include "boost/functional/factory.hpp"

#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/Ogre/Renderer.h"

#include "OGRE/OgreRoot.h"

namespace i6e {
namespace modules {

	GUIManager::GUIManager(GUIController * ctrl) : _objRenderer(), _objGUIFunctions(), _objGUIKeyFunctions(), _objRoot(), _widgets(), _enabledFunctions(), _listIDs(0), _toTick(), _keyStates(), _factory(), _tickerLock(), _ctrl(ctrl), _lastOnWindow(""), _mouseOverWidgets(), _mouseCursorSequence(), _mouseCursorFps(0.0), _mouseCursorAnimationLooping(false), _mouseCursorAnimationStartTime(0) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		i6eGUIFacade->registerAddTickerCallback(std::bind(&GUIManager::addTicker, this, std::placeholders::_1));
		i6eGUIFacade->registerRemoveTickerCallback(std::bind(&GUIManager::removeTicker, this, std::placeholders::_1));

		registerGUIWidgetTemplate("GUIBar", boost::factory<GUIBar *>());
		registerGUIWidgetTemplate("GUIChat", boost::factory<GUIChat *>());
		registerGUIWidgetTemplate("GUIComboBox", boost::factory<GUIComboBox *>());
		registerGUIWidgetTemplate("GUIEditbox", boost::factory<GUIEditbox *>());
		registerGUIWidgetTemplate("GUIImage", boost::factory<GUIImage *>());
		registerGUIWidgetTemplate("GUIImageButton", boost::factory<GUIImageButton *>());
		registerGUIWidgetTemplate("GUIMultiColumnList", boost::factory<GUIMultiColumnList *>());
		registerGUIWidgetTemplate("GUIPrint", boost::factory<GUIPrint *>());
		registerGUIWidgetTemplate("GUIStatusList", boost::factory<GUIStatusList *>());
		registerGUIWidgetTemplate("GUITextButton", boost::factory<GUITextButton *>());
		registerGUIWidgetTemplate("GUIToggleButton", boost::factory<GUIToggleButton *>());
		registerGUIWidgetTemplate("GUITooltip", boost::factory<GUITooltip *>());
	}

	GUIManager::~GUIManager() {
		ASSERT_THREAD_SAFETY_FUNCTION

		while (!_widgets.empty()) {
			std::queue<api::GUIWidget *> deleteW;
			deleteW.push(_widgets.begin()->second);
			while (!deleteW.empty()) {
				api::GUIWidget * widget = deleteW.front();
				deleteW.pop();
				for (api::GUIWidget * gw : widget->_childs) {
					deleteW.push(gw);
				}
				_widgets.erase(widget->_name);
				delete widget;
			}
		}

		// Destroy all windows that have been created to allow a clean shutdown of the gui subsystem
		CEGUI::WindowManager::getSingleton().destroyAllWindows();

		// Destroy the CEGUI rendering system
		CEGUI::System::getSingleton().destroy();
	}

	void GUIManager::createGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName) {
		ASSERT_THREAD_SAFETY_FUNCTION

		try {
			// Set skin for GUI elements
			CEGUI::SchemeManager::getSingleton().createFromFile(strScheme);

			// Initialize an empty root window
			_objRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Root.layout");
			_objRoot->setArea(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0), CEGUI::UDim(1, 0), CEGUI::UDim(1, 0));
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(strDefaultMouseImageSet + "/" + strDefaultMouseImageName);
			CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(_objRoot);

			std::string guiAnimationPath;
			if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue<std::string>("GRAPHIC", "guiAnimationPath", guiAnimationPath)) {
				ISIXE_LOG_ERROR("GUIManager", "An exception has occurred: GRAPHIC.guiAnimationPath not set in i6engine.ini!");
				return;
			}
			try {
				boost::filesystem::directory_iterator iter(guiAnimationPath), dirEnd;
				while (iter != dirEnd) {
					if (is_regular_file(*iter)) {
						std::string file = iter->path().string();
						CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML(utils::split(utils::split(file, "/").back(), "\\").back());
					}
					iter++;
				}
			} catch (const boost::filesystem::filesystem_error &) {
			}
		} catch (CEGUI::Exception & e) {
			ISIXE_THROW_API("GUI", e.getMessage());
			std::cout << e.getMessage() << std::endl;
		} catch (Ogre::Exception & e) {
			ISIXE_THROW_API("GUI", e.what());
			std::cout << e.what() << std::endl;
		}
	}

	void GUIManager::MouseMove(const double fltDeltaX, const double fltDeltaY, const double fltDeltaZ) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(float(fltDeltaX), float(fltDeltaY));
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(float(fltDeltaZ));
	}

	void GUIManager::MousePos(const double fltPosX, const double fltPosY) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(float(fltPosX), float(fltPosY));
	}

	void GUIManager::MouseDown(const api::KeyCode enuButton) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		switch (enuButton) {
		case api::KeyCode::KC_MBLeft: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
			break;
		}
		case api::KeyCode::KC_MBRight: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);
			break;
		}
		case api::KeyCode::KC_MBMiddle: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton);
			break;
		}
		case api::KeyCode::KC_MBButton3: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::X1Button);
			break;
		}
		case api::KeyCode::KC_MBButton4: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::X2Button);
			break;
		}
		case api::KeyCode::KC_MBButton5: { break; }
		case api::KeyCode::KC_MBButton6: { break; }
		case api::KeyCode::KC_MBButton7: { break; }
		default: {
			ISIXE_THROW_API("GUI", "Button doesn't exist");
			break;
		}
		}
	}

	void GUIManager::MouseUp(const api::KeyCode enuButton) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		switch (enuButton) {
		case api::KeyCode::KC_MBLeft: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
			break;
		}
		case api::KeyCode::KC_MBRight: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);
			break;
		}
		case api::KeyCode::KC_MBMiddle: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton);
			break;
		}
		case api::KeyCode::KC_MBButton3: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::X1Button);
			break;
		}
		case api::KeyCode::KC_MBButton4: {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::X2Button);
			break;
		}
		case api::KeyCode::KC_MBButton5: { break; }
		case api::KeyCode::KC_MBButton6: { break; }
		case api::KeyCode::KC_MBButton7: { break; }
		default: {
			ISIXE_THROW_API("GUI", "Button doesn't exist");
			break;
		}
		}
	}

	void GUIManager::KeyDown(const api::KeyCode intKeyCode, uint32_t text) {
		ASSERT_THREAD_SAFETY_FUNCTION

		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(CEGUI::Key::Scan(intKeyCode));
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(CEGUI::uint(text));
	}

	void GUIManager::KeyHold(const api::KeyCode intKeyCode) {
		ASSERT_THREAD_SAFETY_FUNCTION
			
		//CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(CEGUI::Key::Scan(intKeyCode));
		//CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(CEGUI::uint(intKeyCode));

		std::stringstream ss;
		ss << uint32_t(intKeyCode);

		if (allowedEvent(ss.str())) {
			triggerKeyFunction(std::stoul(ss.str()), "Hold");
		}
	}

	void GUIManager::KeyUp(const api::KeyCode intKeyCode) {
		ASSERT_THREAD_SAFETY_FUNCTION

		CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(CEGUI::Key::Scan(intKeyCode));
	}

	void GUIManager::MouseLeaves() const {
		ASSERT_THREAD_SAFETY_FUNCTION

		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseLeaves();
	}

	void GUIManager::processCreateMessage(uint16_t messageType, api::gui::GUIUpdateMessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (data->_name == GUIMAN) {
			handleCreateMessage(messageType, data);
			return;
		}
		api::gui::GUI_Widget_Create * crt = static_cast<api::gui::GUI_Widget_Create *>(data);
		api::GUIWidget * widget = _factory.createGUIWidget(crt->_name, crt->widgetType, crt->style);
		widget->setMouseOverCallback(std::bind(&GUIManager::getMouseoverWidgets, this));
		_widgets[crt->_name] = widget;
		addToRootWindow(widget->_window);
	}

	void GUIManager::processUpdateMessage(uint16_t messageType, api::gui::GUIUpdateMessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (data->_name == GUIMAN) {
			handleUpdateMessage(messageType, data);
			return;
		}
		getWidgetByName(data->_name)->update(messageType, data);
	}

	void GUIManager::processDeleteMessage(uint16_t messageType, api::gui::GUIUpdateMessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (messageType == api::gui::GuiReset) {
			ISIXE_LOG_WARN("GUIManager", "GUI reset not implemented yet");
		} else {
			if (data->_name == GUIMAN) {
				handleDeleteMessage(messageType, data);
				return;
			}
			api::GUIWidget * w = getWidgetByName(data->_name);
			std::queue<api::GUIWidget *> deleteW;
			deleteW.push(w);
			while (!deleteW.empty()) {
				api::GUIWidget * widget = deleteW.front();
				deleteW.pop();
				for (api::GUIWidget * gw : widget->_childs) {
					deleteW.push(gw);
				}
				_widgets.erase(widget->_name);
				delete widget;
			}
		}
	}

	void GUIManager::processInputUpdateMessage(uint16_t type, core::MessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (type == api::mouse::MouMouse) {
			int32_t intNewX = static_cast<api::input::Input_Mouse_Update *>(data)->intNewX;
			int32_t intNewY = static_cast<api::input::Input_Mouse_Update *>(data)->intNewY;

			MousePos(intNewX, intNewY);
		} else if (type == api::keyboard::KeyKeyboard) {
			api::KeyCode keyCode = static_cast<api::input::Input_Keyboard_Update *>(data)->code;
			api::KeyState pressed = static_cast<api::input::Input_Keyboard_Update *>(data)->pressed;

			if (pressed == api::KeyState::KEY_PRESSED) {
				if (keyCode < api::KeyCode::MOUSEBUTTONS) {
					KeyDown(keyCode, static_cast<api::input::Input_Keyboard_Update *>(data)->text);
				} else {
					MouseDown(keyCode);
				}
			} else if (pressed == api::KeyState::KEY_RELEASED) {
				if (keyCode < api::KeyCode::MOUSEBUTTONS) {
					KeyUp(keyCode);
				} else {
					MouseUp(keyCode);
				}
				KeyUp(keyCode);
			} else if (pressed == api::KeyState::KEY_HOLD) {
				if (keyCode < api::KeyCode::MOUSEBUTTONS) {
					KeyHold(keyCode);
				} else {
					// CEGUI doesn't handle hold of mouse buttons separatly
				}
			}
		}
	}

	void GUIManager::addTicker(api::GUIWidget * widget) {
		ASSERT_THREAD_SAFETY_FUNCTION

		std::lock_guard<std::mutex> sl(_tickerLock);
		_toTick.push_back(widget);
	}

	void GUIManager::removeTicker(api::GUIWidget * widget) {
		ASSERT_THREAD_SAFETY_FUNCTION
		std::lock_guard<std::mutex> sl(_tickerLock);
		_toTick.remove(widget);
	}

	void GUIManager::tickWidgets() {
		ASSERT_THREAD_SAFETY_FUNCTION
		std::lock_guard<std::mutex> sl(_tickerLock);
		for (api::GUIWidget * widget : _toTick) {
			widget->tick();
		}
	}

	void GUIManager::registerGUIWidgetTemplate(const std::string & name, GUIWidget_factory createFunc) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_factory.registerGUIWidgetTemplate(name, createFunc);
	}

	void GUIManager::addChildWindow(const std::string & parent_widget, const std::string & child_widget) {
		ASSERT_THREAD_SAFETY_FUNCTION
		getWidgetByName(parent_widget)->_window->addChild(getWidgetByName(child_widget)->_window);
	}

	void GUIManager::addToRootWindow(CEGUI::Window * window) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objRoot->addChild(window);
	}

	void GUIManager::setMouseVisibility(const bool visibility) {
		ASSERT_THREAD_SAFETY_FUNCTION
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(visibility);
	}

	void GUIManager::cleanUpAllWindows() {
		ASSERT_THREAD_SAFETY_FUNCTION

		for (std::map<std::string, api::GUIWidget *>::iterator it = _widgets.begin(); it != _widgets.end(); ++it) {
			delete it->second;
		}

		_widgets.clear();
	}

	void GUIManager::subscribeEvent(const std::string & name, api::gui::SubscribeEvent type) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (type == api::gui::SubscribeEvent::Clicked) {
			try {
				CEGUI::PushButton * objButton = static_cast<CEGUI::PushButton *>(_objRoot->getChildRecursive(name));
				objButton->CEGUI::Window::subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GUIManager::ClickedEventHandler, this));
				_enabledFunctions.insert(std::make_pair(name, true));
			} catch(CEGUI::Exception & e) {
				ISIXE_THROW_API("GUIManager", e.getMessage());
			}
		} else {
			ISIXE_THROW_API("GUIManager", "Unknown type: " << type);
		}
	}

	bool GUIManager::ClickedEventHandler(const CEGUI::EventArgs & e) {
		ASSERT_THREAD_SAFETY_FUNCTION

		const CEGUI::WindowEventArgs * objArgument = dynamic_cast<const CEGUI::WindowEventArgs *>(&e);
		std::string strName = objArgument->window->getName().c_str();

		if (allowedEvent(strName)) {
			triggerFunction(strName);
		}

		return true;
	}

	bool GUIManager::PressedEventHandler(const CEGUI::EventArgs & e) {
		ASSERT_THREAD_SAFETY_FUNCTION

		const CEGUI::KeyEventArgs * objArgument = dynamic_cast<const CEGUI::KeyEventArgs *>(&e);
		uint32_t scan = uint32_t(objArgument->scancode);
		std::stringstream ss;
		ss << scan;
		if (_keyStates[scan] == api::KeyState::KEY_RELEASED) {
			_keyStates[scan] = api::KeyState::KEY_PRESSED;
		} else if (_keyStates[scan] == api::KeyState::KEY_PRESSED) {
			_keyStates[scan] = api::KeyState::KEY_HOLD;
			return false;
		} else if (_keyStates[scan] == api::KeyState::KEY_HOLD) {
			return false;
		} else {
			_keyStates[scan] = api::KeyState::KEY_PRESSED;
		}

		if (allowedEvent(ss.str())) {
			triggerKeyFunction(std::stoul(ss.str()), "Pressed");
		}

		return true;
	}

	bool GUIManager::ReleasedEventHandler(const CEGUI::EventArgs & e) {
		ASSERT_THREAD_SAFETY_FUNCTION

		const CEGUI::KeyEventArgs * objArgument = dynamic_cast<const CEGUI::KeyEventArgs *>(&e);
		uint32_t scan = uint32_t(objArgument->scancode);
		std::stringstream ss;
		ss << scan;
		if (_keyStates[scan] == api::KeyState::KEY_RELEASED) {
			return false;
		} else if (_keyStates[scan] == api::KeyState::KEY_PRESSED) {
			_keyStates[scan] = api::KeyState::KEY_RELEASED;
		} else if (_keyStates[scan] == api::KeyState::KEY_HOLD) {
			_keyStates[scan] = api::KeyState::KEY_RELEASED;
		} else {
			_keyStates[scan] = api::KeyState::KEY_RELEASED;
		}

		if (allowedEvent(ss.str())) {
			triggerKeyFunction(std::stoul(ss.str()), "Released");
		}

		return true;
	}

	void GUIManager::setFunction(const std::string & name, const std::function<void(void)> & ptrEventMethod) {
		ASSERT_THREAD_SAFETY_FUNCTION

		_objGUIFunctions[name] = ptrEventMethod;
	}

	bool GUIManager::allowedEvent(const std::string & name) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_enabledFunctions.count(name) == 0) {
			return false;
		}
		return _enabledFunctions[name];
	}

	void GUIManager::triggerFunction(const std::string & name) {
		ASSERT_THREAD_SAFETY_FUNCTION

		GUIFunctions::const_iterator iter = _objGUIFunctions.find(name);

		if (iter == _objGUIFunctions.end()) {
			ISIXE_THROW_FAILURE("GUIManager", "A window with name " << name << " does not exist!");
		} else {
			std::function<void(void)> callback = iter->second;
			i6eEngineController->registerTimer(0, [callback]() {
				callback();
				return false;
			}, false, core::JobPriorities::Prio_Medium);
		}
	}

	void GUIManager::triggerKeyFunction(const uint32_t keyCode, const std::string & type) {
		ASSERT_THREAD_SAFETY_FUNCTION

		GUIKeyFunctions::const_iterator iter = _objGUIKeyFunctions.find(std::make_pair(api::KeyCode(keyCode), type));
		if (iter == _objGUIKeyFunctions.end()) {
			// keyCode is register for another event
			return;
		}
		std::function<void(void)> callback = iter->second;
		i6eEngineController->registerTimer(0, [callback]() {
			callback();
			return false;
		}, false, core::JobPriorities::Prio_Medium);
	}

	bool GUIManager::getVisibility(const std::string & windowname) {
		ASSERT_THREAD_SAFETY_FUNCTION

		return getWidgetByName(windowname)->_window->CEGUI::Window::isVisible();
	}

	api::GUIWidget * GUIManager::getWidgetByName(const std::string & name) {
		ASSERT_THREAD_SAFETY_FUNCTION

		std::map<std::string, api::GUIWidget *>::iterator it = _widgets.find(name);
		if (it == _widgets.end()) {
			ISIXE_THROW_API("GUI", "Can't find widget \"" << name << "\"");
		}
		return it->second;
	}

	void GUIManager::handleCreateMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (type == api::gui::GuiWindow) {
			initializeGUI(reinterpret_cast<Ogre::Root *>(static_cast<api::gui::GUI_Window_Create *>(data)->window));
		} else if (type == api::gui::GuiAdd) {
			const api::gui::GUI_Add_Create * msg = static_cast<api::gui::GUI_Add_Create *>(data);
			std::string strScheme = msg->scheme;
			std::string strFont = msg->font;
			std::string strDefaultFont = msg->defFont;
			std::string strDefaultMouseImageSet = msg->defMouseImageSet;
			std::string strDefaultMouseImageName = msg->defMouseImageName;

			createGUI(strScheme, strFont, strDefaultFont, strDefaultMouseImageSet, strDefaultMouseImageName);
		} else if (type == api::gui::GuiWidgetTemplate) {
			registerGUIWidgetTemplate(static_cast<api::gui::GUI_WidgetTemplate_Create *>(data)->name, static_cast<api::gui::GUI_WidgetTemplate_Create *>(data)->func);
		} else if (type == api::gui::GuiAddImageset) {
			CEGUI::ImageManager::getSingleton().loadImageset(dynamic_cast<api::gui::GUI_AddImageset_Create *>(data)->imageset);
		} else if (type == api::gui::GuiLoadCanvas) {
			GUICanvas::load(dynamic_cast<api::gui::GUI_LoadCanvas_Create *>(data)->name, dynamic_cast<api::gui::GUI_LoadCanvas_Create *>(data)->file, this, _factory, _widgets);
		} else {
			ISIXE_THROW_API("GUIManager", "Can't handle create message " << type);
		}
	}

	void GUIManager::handleUpdateMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (type == api::gui::GuiAddToRoot) {
			std::string child_window = static_cast<api::gui::GUI_AddToRoot_Update *>(data)->child;
			addToRootWindow(getWidgetByName(child_window)->_window);
		} else if (type == api::gui::GuiMouseVisible) {
			bool visibility = static_cast<api::gui::GUI_MouseVisible_Update *>(data)->visible;

			setMouseVisibility(visibility);
		} else if (type == api::gui::GuiEvent) {
			changeEvent(static_cast<api::gui::GUI_Event_Update *>(data)->name, static_cast<api::gui::GUI_Event_Update *>(data)->enabled);
		} else if (type == api::gui::GuiMouseCursorImage) {
			setAnimatedMouseCursor({ static_cast<api::gui::GUI_MouseCursorImage_Update *>(data)->image }, 0.0, false);
		} else if (type == api::gui::GuiMouseCursorSequence) {
			api::gui::GUI_MouseCursorSequence_Update * mcsu = dynamic_cast<api::gui::GUI_MouseCursorSequence_Update *>(data);
			setAnimatedMouseCursor(mcsu->sequence, mcsu->fps, mcsu->looping);
		} else if (type == api::gui::GuiResolution) {
			const api::gui::GUI_Resolution_Update * ru = dynamic_cast<api::gui::GUI_Resolution_Update *>(data);
			_objRenderer->setDisplaySize(CEGUI::Sizef(float(ru->resolution.width), float(ru->resolution.height)));
		} else if (type == api::gui::GuiSubscribeEvent) {
			std::map<std::string, api::GUIWidget *>::iterator it = _widgets.find(static_cast<api::gui::GUI_SubscribeEvent_Update *>(data)->name);
			if (it != _widgets.end()) {
				it->second->update(type, data);
				return;
			}
			subscribeEvent(static_cast<api::gui::GUI_SubscribeEvent_Update *>(data)->name, static_cast<api::gui::GUI_SubscribeEvent_Update *>(data)->event);
			setFunction(static_cast<api::gui::GUI_SubscribeEvent_Update *>(data)->name, static_cast<api::gui::GUI_SubscribeEvent_Update *>(data)->func);
		} else if (type == api::gui::GuiDefaultFont) {
			const api::gui::GUI_SetDefaultFont * sdf = dynamic_cast<api::gui::GUI_SetDefaultFont *>(data);
			CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(sdf->font);
		} else if (type == api::gui::GuiSetParent) {
			const api::gui::GUI_SetParent * sp = dynamic_cast<api::gui::GUI_SetParent *>(data);
			addChildWindow(sp->parent, sp->child);
		} else {
			ISIXE_THROW_API("GUIManager", "Can't handle update message " << type);
		}
	}

	void GUIManager::handleDeleteMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (type == api::gui::GuiCleanUp) {
			 cleanUpAllWindows();
		} else {
			ISIXE_THROW_API("GUIManager", "Can't handle delete message " << type);
		}
	}

	void GUIManager::initializeGUI(Ogre::Root * root) {
		ASSERT_THREAD_SAFETY_FUNCTION
		// Bootstrap CEGUI::System with an OgreRenderer object that uses the
		// default Ogre rendering window as the default output surface, an Ogre based
		// ResourceProvider, and an Ogre based ImageCodec.
		Ogre::RenderTarget & _target = *root->getRenderTarget(api::EngineController::GetSingletonPtr()->getAppl()->getName());
		_objRenderer = &CEGUI::OgreRenderer::bootstrapSystem(_target);

		// Add default resource groups (Paths to these groups are defined in /bin/resources.cfg)
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
		CEGUI::Font::setDefaultResourceGroup("Fonts");
		CEGUI::Scheme::setDefaultResourceGroup("Schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
		CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	}

	void GUIManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_objRenderer) {
			tickWidgets();

			// get mouse position to find underlying widgets
			double x = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x;
			double y = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y;
			// search for underlying widgets
			_mouseOverWidgets.clear();
			for (auto widget : _widgets) {
				double posX = widget.second->_window->getPosition().d_x.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width;
				double posY = widget.second->_window->getPosition().d_y.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height;
				double sizeX = widget.second->_window->getSize().d_width.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width;
				double sizeY = widget.second->_window->getSize().d_height.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height;
				if (x >= posX && x <= posX + sizeX && y >= posY && y <= posY + sizeY && widget.second->_window->isVisible() && widget.second->isHit()) {
					_mouseOverWidgets.push_back(widget.second);
				}
			}
			// sort them to get upper widget as required to get the main window
			std::sort(_mouseOverWidgets.begin(), _mouseOverWidgets.end(), [](api::GUIWidget * a, api::GUIWidget * b) {
				return a->_window->getZIndex() > b->_window->getZIndex();
			});
			// if there are widgets below the mouse at this point they are visible and not transparent
			i6eGUIFacade->setOnWindow(!_mouseOverWidgets.empty());

			// check if there is a widget capturing input
			i6eGUIFacade->setInputCaptured(!_mouseOverWidgets.empty() && _mouseOverWidgets.front()->_window->isActive());

			static uint64_t lastTime = i6eEngineController->getCurrentTime();
			uint64_t cT = i6eEngineController->getCurrentTime();
			// animate mouse cursor
			if (std::abs(_mouseCursorFps) > DBL_EPSILON && _mouseCursorSequence.size() > 1) {
				uint64_t timeDiff = cT - _mouseCursorAnimationStartTime;
				if (timeDiff > (_mouseCursorSequence.size() - 1) * 1000000.0 / _mouseCursorFps) {
					if (_mouseCursorAnimationLooping) {
						_mouseCursorAnimationStartTime += uint64_t((_mouseCursorSequence.size() - 1) * 1000000.0 / _mouseCursorFps);
					} else {
						_mouseCursorFps = 0.0;
					}
				}
				if (std::abs(_mouseCursorFps) > DBL_EPSILON) {
					size_t index = size_t(std::round((timeDiff * _mouseCursorFps) / 1000000.0));
					CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage(_mouseCursorSequence[index]);
					CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().invalidate();
					CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().draw();
				}
			}

			// Inject the time since the last tick into CEGUI to allow calculations based on the frame time (fading etc.).
			// Inject the frame time of the graphics subsystem in seconds.
			CEGUI::System::getSingleton().injectTimePulse((cT - lastTime) / 1000000.0f);
			lastTime = cT;
		}
	}

	void GUIManager::setAnimatedMouseCursor(const std::vector<std::string> & sequence, double fps, bool looping) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mouseCursorSequence = sequence;
		_mouseCursorFps = fps;
		_mouseCursorAnimationLooping = looping;
		_mouseCursorAnimationStartTime = i6eEngineController->getCurrentTime();
		if (!sequence.empty()) {
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage(sequence.front());
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().invalidate();
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().draw();
		}
	}

} /* namespace modules */
} /* namespace i6e */

