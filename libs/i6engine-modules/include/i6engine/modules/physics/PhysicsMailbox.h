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
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_PHYSICSMAILBOX_H__
#define __I6ENGINE_MODULES_PHYSICSMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6engine {
namespace modules {

	class PhysicsController;
	class PhysicsManager;

	/**
	 * \class PhysicsMailbox
	 * \brief Handles the incoming messages on the physic channel and calls the corresponding methods.
	 */
	class PhysicsMailbox {
		friend class PhysicsController;

	protected:
		/**
		 * \brief Constructor
		 */
		explicit PhysicsMailbox(PhysicsManager * pm) : _manager(pm) {
			ASSERT_THREAD_SAFETY_CONSTRUCTOR
		}

		/**
		 * \brief Destructor
		 */
		~PhysicsMailbox() {}

		/**
		 * \brief Handles incoming messages.
		 * \todo PhyNode Create and Update Method have identical Code.
		 */
		void News(const api::GameMessage::Ptr & msg) const;

	private:
		PhysicsManager * _manager;

		/**
		 * \brief forbidden
		 */
		PhysicsMailbox(const PhysicsMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		PhysicsMailbox & operator=(const PhysicsMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_PHYSICSMAILBOX_H__ */

/**
 * @}
 */
