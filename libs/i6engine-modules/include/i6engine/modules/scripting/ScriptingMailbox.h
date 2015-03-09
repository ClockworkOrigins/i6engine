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
 * \addtogroup Scripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_SCRIPTINGMAILBOX_H__
#define __I6ENGINE_MODULES_SCRIPTINGMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6engine {
namespace modules {

	class ScriptingController;
	class ScriptingManager;

	/**
	 * \class ScriptingMailbox
	 * \brief Handles the incoming messages on the scripting channel and calls the corresponding methods.
	 */
	class ScriptingMailbox {
		friend class ScriptingController;

	protected:
		/**
		 * \brief Constructor
		 */
		explicit ScriptingMailbox(ScriptingManager * m);

		/**
		 * \brief Destructor
		 */
		~ScriptingMailbox() {}

		/**
		 * \brief Handles incoming messages.
		 */
		void News(const api::GameMessage::Ptr & msg) const;

	private:
		ScriptingManager * _manager;

		/**
		 * \brief forbidden
		 */
		ScriptingMailbox(const ScriptingMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		ScriptingMailbox & operator=(const ScriptingMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_SCRIPTINGMAILBOX_H__ */

/**
 * @}
 */
