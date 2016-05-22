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

#ifndef __I6ENGINE_API_INPUTFACADE_H__
#define __I6ENGINE_API_INPUTFACADE_H__

#include <map>

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

namespace i6e {
namespace api {

	enum class KeyCode : uint32_t;

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

	public:
		InputFacade();
		~InputFacade();

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_INPUTFACADE_H__ */

/**
 * @}
 */
