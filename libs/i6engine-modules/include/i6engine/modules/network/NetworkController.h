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
 * \addtogroup Network
 * @{
 */

#ifndef __I6ENGINE_MODULES_NETWORKCONTROLLER_H__
#define __I6ENGINE_MODULES_NETWORKCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace modules {

	class NetworkMailbox;
	class NetworkManager;

	/**
	 * \class NetworkController
	 * \brief This class performs and provides various methods to the engine for the network
	 */
	class ISIXE_MODULES_API NetworkController : public core::ModuleController {
	public:
		/**
		 * \brief Nothing to do yet
		 * This method has nothing to to do yet
		 */
		NetworkController();

		/**
		 * \brief Nothing to do yet
		 * This method has nothing to do yet
		 */
		~NetworkController();

	private:
		NetworkMailbox * _mailbox;
		NetworkManager * _manager;

		/**
		 * \brief Called when starting the thread of the graphics subsystem.
		 * \details Registers with the messaging subsystem
		 */
		void OnThreadStart() override;

		/**
		 * \brief Called when shutting down the network subsystem.
		 * \details Cleans up all resources of the network subsystem.
		 */
		void ShutDown() override;

		/**
		 * \brief Called on every tick of the network subsystem.
		 * \details Updates the network
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		const NetworkController & operator=(const NetworkController &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkController(const NetworkController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_NETWORKCONTROLLER_H__ */

/**
 * @}
 */
