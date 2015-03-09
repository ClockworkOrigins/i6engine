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

#ifndef __I6ENGINE_MODULES_OBJECTMAILBOX_H__
#define __I6ENGINE_MODULES_OBJECTMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6engine {
namespace modules {

	class ObjectController;
	class ObjectManager;

	/**
	 * \class ObjectMailbox
	 * \brief Extracts the contents of incoming messages on the "Object" channel and calls the corresponding methods
	 * \see Core/Messaging System
	 */
	class ObjectMailbox {
		friend class ObjectController;

	protected:
		/**
		 * \brief constructor
		 */
		explicit ObjectMailbox(ObjectManager * manager);

		/**
		 * \brief destructor
		 */
		~ObjectMailbox();

		/**
		 * \brief Handles messages for the Object Manager
		 * \param objMessage Any message on the channel
		 */
		void NewsObjectManager(const api::GameMessage::Ptr & msg) const;

		/**
		 * \brief Handles messages for Components
		 * \param objMessage Any message on the channel
		 * \todo Code for Create and Update almost identical. Someone should check this.
		 */
		void NewsComponent(const api::GameMessage::Ptr & msg) const;

		/**
		 * \brief Handles messages for Components.
		 * \param objMessage Any message on the channel
		 */
		void NewsObject(const api::GameMessage::Ptr & msg) const;

	private:
		ObjectManager * _manager;

		/**
		 * \brief forbidden
		 */
		ObjectMailbox(const ObjectMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		ObjectMailbox & operator=(const ObjectMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_OBJECTMAILBOX_H__ */

/**
 * @}
 */
