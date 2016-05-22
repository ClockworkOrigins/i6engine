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

#ifndef __I6ENGINE_MODULES_INPUTMAILBOX_H__
#define __I6ENGINE_MODULES_INPUTMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class InputController;
	class InputManager;

	/**
	 * \class InputMailbox
	 * \brief Dispatches all messages received by input subsystem
	 */
	class InputMailbox {
		friend class InputController;

	private:
		InputManager * _manager;

		/**
		 * \brief constructor taking InputManager
		 */
		explicit InputMailbox(InputManager * manager);

		/**
		 * \brief dispatcher for messages for input subsystem
		 */
		void News(const api::GameMessage::Ptr & msg);

		/**
		 * \brief forbidden
		 */
		InputMailbox(const InputMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		const InputMailbox & operator=(const InputMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_INPUTMAILBOX_H__ */

/**
 * @}
 */
