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

#ifndef __I6ENGINE_MODULES_SCRIPTINGCONTROLLER_H__
#define __I6ENGINE_MODULES_SCRIPTINGCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
namespace modules {

	class ScriptingMailbox;
	class ScriptingManager;

	/**
	 * \class ScriptingController
	 * \brief Controls the scripting
	 *
	 * This class creates the scripting and manages its objects.
	 */
	class ISIXE_MODULES_API ScriptingController : public core::ModuleController {
	public:
		/**
		 * \brief constructor
		 */
		ScriptingController();

		/**
		 * \brief destructor
		 */
		~ScriptingController();

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

	private:
		ScriptingMailbox * _mailbox;
		ScriptingManager * _manager;

		/**
		 * \brief forbidden
		 */
		ScriptingController(const ScriptingController &) = delete;

		/**
		 * \brief forbidden
		 */
		const ScriptingController & operator=(const ScriptingController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_SCRIPTINGCONTROLLER_H__ */

/**
 * @}
 */
