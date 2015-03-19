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

#ifndef __I6ENGINE_API_INPUTFACADE_H__
#define __I6ENGINE_API_INPUTFACADE_H__

#include <map>

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

namespace i6engine {
namespace api {

	enum class KeyCode : uint32_t;
	enum class MouseButtonID : uint32_t;

	/**
	 * \brief defines the current state of a key
	 */
	enum class ISIXE_MODULES_API KeyState : uint16_t {
		KEY_RELEASED = 0,	//! key was released
		KEY_PRESSED = 1,	//! key was pressed
		KEY_HOLD = 2		//! key is still pressed
	};

	class ISIXE_MODULES_API InputFacade {
		/**
		 * \brief Data structure to store mappings of keys and corresponding actions.
		 */
		typedef std::map<const KeyCode, std::string> mapKeymap;

		/**
		 * \brief Data structure to store mappings of mousebuttons and corresponding actions.
		 */
		typedef std::map<const MouseButtonID, std::string> mapMouseButtonMap;

	public:
		InputFacade();
		~InputFacade();

		/**
		 * \brief Sets a mouse button mapping
		 * \param[in] id			mouse button to bind action to
		 * \param[in] strAction		action to be bound
		 */
		void setMouseButtonMapping(const MouseButtonID id, const std::string & strAction);

		/**
		 * \brief Retrieves a mouse button mapping
		 * \param[in] id			mouse button to get action for
		 * \return					action bound to the mouse button
		 */
		std::string getMouseButtonMapping(const MouseButtonID id) const;

		/**
		 * \brief Sets a key mapping
		 * \param[in] id			key code to bind action to
		 * \param[in] strAction		action to be bound
		 */
		void setKeyMapping(const KeyCode id, const std::string & strAction);

		/**
		 * \brief Removes a key mapping
		 * \param[in] id			key code to bind action to
		 */
		void removeKeyMapping(const KeyCode id);

		/**
		 * \brief Retrieves a key mapping
		 * \param[in] id			key code to retrieve action for
		 * \return					action bound to the key
		 */
		std::string getKeyMapping(const KeyCode id) const;

		/**
		 * \brief Subscribes for a keyboard event. (m)
		 * \param name The OIS::KeyCode for the specified Key, equivalent to an int
		 * \param type Type of the keyboard event.
		 * \param ptrEventMethod Everytime the Event occurs on this window, the Method leading to this pointer will be called
		 */
		void subscribeKeyEvent(const KeyCode name, const KeyState type, const boost::function<void(void)> & f) const;

		/**
		 * \brief Unsubscribes for a keyboard event. (m)
		 * \param name The OIS::KeyCode for the specified Key, equivalent to an int
		 * \param type Type of the keyboard event.
		 */
		void unsubscribeKeyEvent(const KeyCode name, const KeyState type) const;

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

	private:
		mapKeymap _keymap;
		mapMouseButtonMap _mbmap;

		mutable boost::mutex _lock;

		/**
		 * \brief forbidden
		 */
		InputFacade(const InputFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		InputFacade & operator=(const InputFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_INPUTFACADE_H__ */

/**
 * @}
 */
