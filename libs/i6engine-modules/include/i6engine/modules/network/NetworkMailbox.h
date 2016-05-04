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

#ifndef __I6ENGINE_MODULES_NETWORKMAILBOX_H__
#define __I6ENGINE_MODULES_NETWORKMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class NetworkController;
	class NetworkManager;

	/**
	 * \class NetworkMailbox
	 * \brief Extracts the contents of incoming messages on the "Network" channel and calls the corresponding methods
	 * \details Please consult the messaging documentation found in the "Related Pages" on top of this page for in-depth usage details.
	 */
	class NetworkMailbox {
		friend class NetworkController;
		friend class NetworkManager;

	protected:
		/**
		 * \brief constructor
		 */
		NetworkMailbox();

		/**
		 * \brief destructor
		 */
		~NetworkMailbox();

		/**
		 *\brief See Class-Description
		 *\param objMessage Any message on the channel
		 */
		void News(const api::GameMessage::Ptr & msg) const;

		/**
		 * \brief processes messages received over the network
		 */
		void NetworkNews(const api::GameMessage::Ptr & msg) const;

		/**
		 * \brief setter for the manager
		 */
		void setManager(NetworkManager * manager) { _manager = manager; }

	private:
		/**
		 * \brief forbidden
		 */
		NetworkMailbox(const NetworkMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkMailbox & operator=(const NetworkMailbox &) = delete;

		NetworkManager * _manager;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_NETWORKMAILBOX_H__ */

/**
 * @}
 */
