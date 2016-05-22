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
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_MODULES_OBJECTMAILBOX_H__
#define __I6ENGINE_MODULES_OBJECTMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_OBJECTMAILBOX_H__ */

/**
 * @}
 */
