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

#include "i6engine/modules/input/InputManager.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/input/InputController.h"
#include "i6engine/modules/input/KeyboardListener.h"
#include "i6engine/modules/input/MouseListener.h"

#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"

#include "OIS/OIS.h"

namespace i6e {
namespace modules {

	InputManager::InputManager(InputController * ctrl) : _objRotX(0.0), _objRotY(0.0), _objOisInputManager(nullptr), _objOisKeyboard(nullptr), _objOisMouse(nullptr), _objRenderWindow(nullptr), _keyboardListener(nullptr), _mouseListener(nullptr), _ctrl(ctrl) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	InputManager::~InputManager() {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::WindowEventUtilities::removeWindowEventListener(_objRenderWindow, this);
		removeMouseListener();
		removeKeyboardListener();

		OIS::InputManager::destroyInputSystem(_objOisInputManager);
	}

	void InputManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION

		try {
			captureDevices();
		} catch(utils::exceptions::SubsystemException & e) {
			e.writeLog();
			// check errorcode and repair, if not possible:
			e.PassToMain();
		}
		if (_keyboardListener) {
			_keyboardListener->Tick();
		}
		if (_mouseListener) {
			_mouseListener->Tick();
		}
	}

	void InputManager::captureDevices() {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_objOisMouse) {
			_objOisMouse->capture();
		}

		if (_objOisKeyboard) {
			_objOisKeyboard->capture();
		}
	}

	void InputManager::registerKeyboardListener() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objOisKeyboard = static_cast<OIS::Keyboard *>(_objOisInputManager->createInputObject(OIS::OISKeyboard, true));
		_keyboardListener = new KeyboardListener();
		_objOisKeyboard->setEventCallback(_keyboardListener);
	}

	void InputManager::registerMouseListener() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_objOisMouse = static_cast<OIS::Mouse *>(_objOisInputManager->createInputObject(OIS::OISMouse, true));
		unsigned int intWidth, intHeight, intDepth;
		int intTop, intLeft;
		_objRenderWindow->getMetrics(intWidth, intHeight, intDepth, intLeft, intTop);
		const OIS::MouseState & ms = _objOisMouse->getMouseState();
		ms.width = int(intWidth);
		ms.height = int(intHeight);

		_mouseListener = new MouseListener();
		_objOisMouse->setEventCallback(_mouseListener);
	}

	void InputManager::removeKeyboardListener() {
		ASSERT_THREAD_SAFETY_FUNCTION

		_objOisInputManager->destroyInputObject(_objOisKeyboard);
		_objOisKeyboard = nullptr;
		delete _keyboardListener;
	}

	void InputManager::removeMouseListener() {
		ASSERT_THREAD_SAFETY_FUNCTION

		_objOisInputManager->destroyInputObject(_objOisMouse);
		_objOisMouse = nullptr;
		delete _mouseListener;
	}

	void InputManager::initializeInput(Ogre::RenderWindow * window) {
		ASSERT_THREAD_SAFETY_FUNCTION

		_objRenderWindow = window;

		OIS::ParamList objPl;
		size_t objWindowHnd = 0;
		std::ostringstream objWindowHndStr;

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (!_objRenderWindow->isFullScreen()) {
			objPl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
			objPl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
			objPl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
			objPl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		}
	#else
		objPl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));

		objPl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		objPl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("true")));
		objPl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	#endif

		_objRenderWindow->getCustomAttribute("WINDOW", &objWindowHnd);

		objWindowHndStr << objWindowHnd;
		objPl.insert(std::make_pair(std::string("WINDOW"), objWindowHndStr.str()));

		_objOisInputManager = OIS::InputManager::createInputSystem(objPl);

		registerKeyboardListener();
		registerMouseListener();

		Ogre::WindowEventUtilities::addWindowEventListener(_objRenderWindow, this);
	}

	void InputManager::NewsCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getSubtype() == api::input::InputWindow) {
			initializeInput(reinterpret_cast<Ogre::RenderWindow *>(static_cast<api::input::Input_Window_Create *>(msg->getContent())->window));
		} else if (msg->getSubtype() == api::input::InputSubscribeKeyEvent) {
			if (_keyboardListener == nullptr) {
				ISIXE_THROW_API("InputManager", "No KeyBoardListener available. Have you registered GraphicsSubSystem?")
			}
			_keyboardListener->setKeyFunction(static_cast<api::input::Input_SubscribeKeyEvent_Create *>(msg->getContent())->code, static_cast<api::input::Input_SubscribeKeyEvent_Create *>(msg->getContent())->type, static_cast<api::input::Input_SubscribeKeyEvent_Create *>(msg->getContent())->func);
			if (_mouseListener == nullptr) {
				ISIXE_THROW_API("InputManager", "No KeyBoardListener available. Have you registered GraphicsSubSystem?")
			}
			_mouseListener->setKeyFunction(static_cast<api::input::Input_SubscribeKeyEvent_Create *>(msg->getContent())->code, static_cast<api::input::Input_SubscribeKeyEvent_Create *>(msg->getContent())->type, static_cast<api::input::Input_SubscribeKeyEvent_Create *>(msg->getContent())->func);
		}
	}

	void InputManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getSubtype() == api::input::InputReset) {
			_ctrl->reset();
		} else if (msg->getSubtype() == api::input::InputResolution) {
			removeMouseListener();
			registerMouseListener();
		}
	}

	void InputManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (msg->getSubtype() == api::input::InputSubscribeKeyEvent) {
			_keyboardListener->removeKeyFunction(static_cast<api::input::Input_SubscribeKeyEvent_Delete *>(msg->getContent())->code, static_cast<api::input::Input_SubscribeKeyEvent_Delete *>(msg->getContent())->type);
			_mouseListener->removeKeyFunction(static_cast<api::input::Input_SubscribeKeyEvent_Delete *>(msg->getContent())->code, static_cast<api::input::Input_SubscribeKeyEvent_Delete *>(msg->getContent())->type);
		}
	}

} /* namespace modules */
} /* namespace i6e */
