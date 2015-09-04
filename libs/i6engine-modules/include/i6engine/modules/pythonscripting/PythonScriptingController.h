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

#ifndef __I6ENGINE_MODULES_PYTHONSCRIPTINGCONTROLLER_H__
#define __I6ENGINE_MODULES_PYTHONSCRIPTINGCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
namespace modules {

	class PythonScriptingMailbox;
	class PythonScriptingManager;

	/**
	 * \class PythonScriptingController
	 * \brief Controls the scripting
	 *
	 * This class creates the scripting and manages its objects.
	 */
	class ISIXE_MODULES_API PythonScriptingController : public core::ModuleController {
	public:
		/**
		 * \brief constructor
		 */
		PythonScriptingController();

		/**
		 * \brief destructor
		 */
		~PythonScriptingController();

	private:
		PythonScriptingMailbox * _mailbox;
		PythonScriptingManager * _manager;

		/**
		 * \brief This method will be called when starting the thread of the subsystem.
		 */
		void OnThreadStart() override;

		/**
		 * \brief This method will be called when shutting down the subsystem.
		 */
		void ShutDown() override;

		/**
		 * \brief This method will be called in every tick of the subsystem.
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		PythonScriptingController(const PythonScriptingController &) = delete;

		/**
		 * \brief forbidden
		 */
		const PythonScriptingController & operator=(const PythonScriptingController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_PYTHONSCRIPTINGCONTROLLER_H__ */

/**
 * @}
 */
