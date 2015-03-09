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

#ifndef __I6ENGINE_MODULES_INPUTCONTROLLER_H__
#define __I6ENGINE_MODULES_INPUTCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

#include "i6engine/api/GameMessage.h"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_INPUTCONTROLLER_H__ */

/**
 * @}
 */
