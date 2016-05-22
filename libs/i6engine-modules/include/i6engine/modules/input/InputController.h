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

#ifndef __I6ENGINE_MODULES_INPUTCONTROLLER_H__
#define __I6ENGINE_MODULES_INPUTCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class InputMailbox;
	class InputManager;

	/**
	 * \class InputController
	 * \brief Initializes the event listeners for keyboard and mouse input.
	 * \n Responsible for sending input events to other subsystems via messages.
	 */
	class ISIXE_MODULES_API InputController : public core::ModuleController {
	public:
		/**
		 * \brief constructor
		 */
		InputController();

	protected:
		/**
		 * \brief Called when starting the thread of the input subsystem.
		 * \details Initializes the input library and registers input devices.
		 */
		void OnThreadStart() override;

		/**
		 * \brief Called when shutting down the input subsystem.
		 * \details Unregisters input devices and cleans up memory.
		 */
		void ShutDown() override;

		/**
		 * \brief Called on every tick of the input subsystem.
		 * \details Wrapper for local method captureDevices() .
		 */
		void Tick() override;

	private:
		InputMailbox * _mailbox;
		InputManager * _manager;

		/**
		 * \brief dispatcher for messages for input subsystem
		 */
		void Mailbox(const api::GameMessage::Ptr & msg);

		/**
		 * \brief forbidden
		 */
		InputController(const InputController &) = delete;

		/**
		 * \brief forbidden
		 */
		const InputController & operator=(const InputController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_INPUTCONTROLLER_H__ */

/**
 * @}
 */
