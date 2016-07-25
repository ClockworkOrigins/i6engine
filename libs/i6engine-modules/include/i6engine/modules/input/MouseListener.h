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

#ifndef __I6ENGINE_MODULES_MOUSELISTENER_H__
#define __I6ENGINE_MODULES_MOUSELISTENER_H__

#include <array>
#include <cstdint>
#include <map>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/facades/InputFacade.h"

#include "OIS/OISMouse.h"

namespace OIS {
	class MouseEvent;
} /* namespace OIS */

namespace i6e {
namespace modules {

	class InputManager;

	/**
	 * \class MouseListener
	 * \brief Monitors buttonpresses and sends the corresponding messages to the "Mouse" message channel.
	 */
	class MouseListener : public OIS::MouseListener {
		friend class InputManager;

		typedef std::map<std::pair<api::KeyCode, api::KeyState>, std::function<void(void)>> InputKeyFunctions;

	private:
		std::array<api::KeyState, size_t(api::KeyCode::COUNT)> _keyStates;
		std::array<char, size_t(api::KeyCode::COUNT)> _keyTexts;

		InputKeyFunctions _objInputKeyFunctions;

		/**
		 * \brief last state of the mouseWheel
		 */
		int32_t _lastWheelPos;

		/**
		 * \brief constructor
		 */
		MouseListener();

		/**
		 * \brief destructor
		 */
		~MouseListener() {}

		/**
		 * \brief Sends a message to the "Mouse" message channel to notify subscribers of a change in mouse position.
		 * \param[in] objMouseEvent Event describing a change in the mouse state
		 */
		bool mouseMoved(const OIS::MouseEvent & objMouseEvent);

		/**
		 * \brief Sends a message to the "Mouse" message channel to notify subscribers a button was pressed.
		 * \param[in] objMouseEvent Event describing a change in the mouse state
		 * \param[in] id ID of the button whose state changed
		 */
		bool mousePressed(const OIS::MouseEvent & objMouseEvent, OIS::MouseButtonID id);

		/**
		 * \brief Sends a message to the "Mouse" message channel to notify subscribers a button was released.
		 * \param[in] objMouseEvent Event describing a change in the mouse state
		 * \param[in] id ID of the button whose state changed
		 */
		bool mouseReleased(const OIS::MouseEvent & objMouseEvent, OIS::MouseButtonID id);

		/**
		 * \brief Adds the given function associated with specified keyevent to the _objInputKeyFunctions map
		 *
		 * \param name OIS::api::KeyCode of the designated Key
		 * \param type must either be "Pressed" or "Released"
		 * \param ptrEventMethod Pointer to the designated function
		 */
		void setKeyFunction(const api::KeyCode name, const api::KeyState type, const std::function<void(void)> & ptrEventMethod);

		/**
		 * \brief removes the function specified for the given pair of KeyCode and type
		 *
		 * \param name OIS::api::KeyCode of the designated Key
		 * \param type must either be "Pressed" or "Released"
		 */
		void removeKeyFunction(const api::KeyCode name, const api::KeyState type);

		/**
		 * \brief Triggers the previously subscribed event for a key "Pressed" or "Released" event.
		 */
		void triggerKeyFunction(const api::KeyCode keyCode, const api::KeyState type);

		/**
		 * \brief checks whether keys are hold
		 */
		void Tick();

		/**
		 * \brief Data structure to store mappings of mousebuttons and corresponding actions.
		 */
		typedef std::map<const OIS::MouseButtonID, std::string> mapMouseButtonMap;

		/**
		 * \brief forbidden
		 */
		MouseListener(const MouseListener &) = delete;

		/**
		 * \brief forbidden
		 */
		MouseListener & operator=(const MouseListener &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_MOUSELISTENER_H__ */

/**
 * @}
 */
