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

#ifndef __I6ENGINE_MODULES_NETWORKERRORS_H__
#define __I6ENGINE_MODULES_NETWORKERRORS_H__

#include <string>

#include "i6engine/utils/i6eThreadSafety.h"

namespace i6engine {
namespace modules {

	class NetworkManager;

	class NetworkErrors {
		friend class NetworkManager;

	private:
		/**
		 * \brief constructor taking NetworkController
		 */
		explicit NetworkErrors(NetworkManager * manager);

		/**
		 * \brief destructor
		 */
		~NetworkErrors();

		/**
		 * \brief called when connection to m2etis failed
		 */
		void connectionFailed(const std::string & other);

		/**
		 * \brief called when player leaves game
		 */
		void connectionClosed(const std::string & other);

		/**
		 * \brief called when address or port is already in use while connecting
		 */
		void bindFailure(const std::string & other);

		NetworkManager * _manager;

		/**
		 * \brief forbidden
		 */
		NetworkErrors(const NetworkErrors &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkErrors & operator=(const NetworkErrors &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_NETWORKERRORS_H__ */

/**
 * @}
 */
