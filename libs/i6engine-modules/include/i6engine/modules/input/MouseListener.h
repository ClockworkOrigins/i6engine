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

		typedef std::map<std::pair<api::KeyCode, api::KeyState>, boost::function<void(void)>> InputKeyFunctions;

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
		void setKeyFunction(const api::KeyCode name, const api::KeyState type, const boost::function<void(void)> & ptrEventMethod);

		/**
		 * \brief removes the function specified for the given pair of KeyCode and type
		 *
		 * \param name OIS::api::KeyCode of the designated Key
		 * \param type must either be "Pressed" or "Released"
		 */
		void removeKeyFunction(const api::KeyCode name, const api::KeyState type);

		/**
		 * \brief Triggers the previously subscribed event for a key "Pressed" or "Released" event.
		 *
		 *    seems to invoke a function call to function in _objGUIFunctions specified by name and type
		 *    merely checks existance of name and type value pair in _objGUIFunctions and calls boost::thread(iter->second) on success
		 *
		 * \return   bool true if enabled, false otherwise
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
