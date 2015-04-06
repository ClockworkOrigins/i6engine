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
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_MODULES_OBJECTCONTROLLER_H__
#define __I6ENGINE_MODULES_OBJECTCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
namespace api {
	class ObjectFacade;
} /* namespace api */

namespace modules {

	class ObjectManager;
	class ObjectMailbox;

	/**
	 * \class ObjectController
	 * \brief It administers all GameObjects and their GOComponents.
	 *
	 * Have a look at \link i6engine here \endlink for an overview of typedef's in namespace %i6engine.
	 */
	class ISIXE_MODULES_API ObjectController : public core::ModuleController {
	public:
		/**
		 * \brief Constructor for ObjectController.
		 */
		ObjectController();

		/**
		 * \brief destructor
		 */
		~ObjectController();

	protected:
		/**
		 * \brief This method will be called when starting the thread of ObjectController.
		 * The ObjectController subscribes itself on the Messaging System on start.
		 */
		void OnThreadStart() override;

		/**
		 * \brief This method will be called in every tick of ObjectController.
		 * It processes all messages for the GOComponents.
		 */
		void Tick() override;

		/**
		 * \brief This method will be called when shutting down ObjectController.
		 * For every GameObject in _GOList DeleteObject will be called.
		 */
		void ShutDown() override;

	private:
		ObjectMailbox * _mailbox;

		ObjectManager * _manager;

		/**
		 * \brief forbidden
		 */
		ObjectController(const ObjectController &) = delete;

		/**
		 * \brief forbidden
		 */
		ObjectController & operator=(const ObjectController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_OBJECTCONTROLLER_H__ */

/**
 * @}
 */
