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

#ifndef __I6ENGINE_MODULES_KEYBOARDLISTENER_H__
#define __I6ENGINE_MODULES_KEYBOARDLISTENER_H__

#include <array>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/facades/InputFacade.h"

#include "OIS/OISKeyboard.h"

namespace OIS {
	class KeyEvent;
} /* namespace OIS */

namespace i6engine {
namespace modules {

	class InputController;

	/**
	 * \class KeyboardListener
	 * \brief Monitors keypresses and sends the corresponding messages to the "Input" message channel.
	 */
	class KeyboardListener : public OIS::KeyListener {
		friend class InputManager;

		typedef std::map<std::pair<api::KeyCode, api::KeyState>, boost::function<void(void)>> InputKeyFunctions;

	private:
		std::array<api::KeyState, 238> _keyStates;
		std::array<char, 238> _keyTexts;

		InputKeyFunctions _objInputKeyFunctions;

		/**
		 * \brief constructor
		 */
		KeyboardListener();

		/**
		 * \brief destructor
		 */
		~KeyboardListener() {}

		/**
		 * \brief Sends a message to the "Keyboard" message channel to notify subscribers a button was pressed.
		 * \param[in] objKeyEvent Event describing the relevant key.
		 * \return This function currently always returns true.
		 */
		bool keyPressed(const OIS::KeyEvent & objKeyEvent);

		/**
		 * \brief Sends a message to the "Keyboard" message channel to notify subscribers a button was pressed.
		 * \param[in] objKeyEvent Event describing the relevant key.
		 * \return This function currently always returns true.
		 */
		bool keyReleased(const OIS::KeyEvent & objKeyEvent);

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
		 * \brief forbidden
		 */
		KeyboardListener(const KeyboardListener &) = delete;

		/**
		 * \brief forbidden
		 */
		KeyboardListener & operator=(const KeyboardListener &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_KEYBOARDLISTENER_H__ */

/**
 * @}
 */
