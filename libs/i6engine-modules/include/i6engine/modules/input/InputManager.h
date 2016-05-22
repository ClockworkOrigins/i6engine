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

/**
 * \addtogroup Input
 * @{
 */

#ifndef __I6ENGINE_MODULES_INPUTMANAGER_H__
#define __I6ENGINE_MODULES_INPUTMANAGER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

#include "OGRE/OgreMath.h"
#include "OGRE/OgreWindowEventUtilities.h"

namespace OGRE {
	class RenderWindow;
} /* namespace OGRE */

namespace OIS {
	class InputManager;
	class Keyboard;
	class Mouse;
} /* namespace OIS */

namespace i6e {
namespace modules {

	class InputController;
	class InputMailbox;
	class KeyboardListener;
	class MouseListener;

	class InputManager : public Ogre::WindowEventListener {
		friend class InputController;
		friend class InputMailbox;

	private:
		Ogre::Radian _objRotX, _objRotY;
		OIS::InputManager * _objOisInputManager;
		OIS::Keyboard * _objOisKeyboard;
		OIS::Mouse * _objOisMouse;
		Ogre::RenderWindow * _objRenderWindow;
		KeyboardListener * _keyboardListener;
		MouseListener * _mouseListener;

		InputController * _ctrl;

		/**
		 * \brief constructor, initializes all OIS stuff
		 */
		InputManager(InputController * ctrl);

		/**
		 * \brief destructor
		 */
		~InputManager();

		/**
		 * \brief Ticks Input Subsystem
		 */
		void Tick();

		/**
		 * \brief Returns a handle for the keyboard device.
		 * \return Input library pointer to the keyboard
		 */
		inline OIS::Keyboard * getKeyboard() const { return _objOisKeyboard; }

		/**
		 * \brief Returns a handle for the mouse device.
		 * \return Input library pointer to the mouse
		 */
		inline OIS::Mouse * getMouse() const { return _objOisMouse; }

		/**
		 * \brief Captures events of the input devices and sends corresponding messages to all subscribers of the "Input" message channel.
		 */
		void captureDevices();

		/**
		 * \brief Initializes a keyboard listener
		 */
		void registerKeyboardListener();

		/**
		 * \brief Initializes a mouse listener
		 */
		void registerMouseListener();

		/**
		 * \brief Removes a keyboard listener
		 */
		void removeKeyboardListener();

		/**
		 * \brief Removes a mouse listener
		 */
		void removeMouseListener();

		/**
		 * \brief initialized all internal input stuff
		 */
		void initializeInput(Ogre::RenderWindow * root);

		/**
		 * \brief handles create messages for Input Subsystem
		 */
		void NewsCreate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles update messages for Input Subsystem
		 */
		void NewsUpdate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles delete messages for Input Subsystem
		 */
		void NewsDelete(const api::GameMessage::Ptr & msg);

		/**
		 * \brief forbidden
		 */
		InputManager(const InputManager &) = delete;

		/**
		 * \brief forbidden
		 */
		InputManager & operator=(const InputManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_INPUTMANAGER_H__ */

/**
 * @}
 */
