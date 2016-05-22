/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_PHYSICSMAILBOX_H__
#define __I6ENGINE_MODULES_PHYSICSMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PHYSICSMAILBOX_H__ */

/**
 * @}
 */
