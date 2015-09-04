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

#include <cstdint>
#include <map>

#include "i6engine/utils/i6eThreadSafety.h"

#include "OIS/OISMouse.h"

namespace OIS {
	class MouseEvent;
} /* namespace OIS */

namespace i6engine {
namespace modules {

	class InputManager;

	/**
	 * \class MouseListener
	 * \brief Monitors buttonpresses and sends the corresponding messages to the "Mouse" message channel.
	 */
	class MouseListener : public OIS::MouseListener {
		friend class InputManager;

	private:
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_MOUSELISTENER_H__ */

/**
 * @}
 */
