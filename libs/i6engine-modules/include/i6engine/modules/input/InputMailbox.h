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

#ifndef __I6ENGINE_MODULES_INPUTMAILBOX_H__
#define __I6ENGINE_MODULES_INPUTMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6engine {
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
		/**
		 * \brief constructor taking InputManager
		 */
		explicit InputMailbox(InputManager * manager);

		/**
		 * \brief dispatcher for messages for input subsystem
		 */
		void News(const api::GameMessage::Ptr & msg);

		InputManager * _manager;

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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_INPUTMAILBOX_H__ */

/**
 * @}
 */
