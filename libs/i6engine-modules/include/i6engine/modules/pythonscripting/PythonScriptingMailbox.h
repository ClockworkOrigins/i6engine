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
 * \addtogroup PythonScripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_PYTHONSCRIPTINGMAILBOX_H__
#define __I6ENGINE_MODULES_PYTHONSCRIPTINGMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class PythonScriptingController;
	class PythonScriptingManager;

	/**
	 * \class PythonScriptingMailbox
	 * \brief Handles the incoming messages on the scripting channel and calls the corresponding methods.
	 */
	class PythonScriptingMailbox {
		friend class PythonScriptingController;

	protected:
		/**
		 * \brief Constructor
		 */
		explicit PythonScriptingMailbox(PythonScriptingManager * m);

		/**
		 * \brief Destructor
		 */
		~PythonScriptingMailbox() {}

		/**
		 * \brief Handles incoming messages.
		 */
		void News(const api::GameMessage::Ptr & msg) const;

	private:
		PythonScriptingManager * _manager;

		/**
		 * \brief forbidden
		 */
		PythonScriptingMailbox(const PythonScriptingMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		PythonScriptingMailbox & operator=(const PythonScriptingMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PYTHONSCRIPTINGMAILBOX_H__ */

/**
 * @}
 */
