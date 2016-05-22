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

#include "i6engine/modules/object/ObjectMailbox.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/object/ObjectManager.h"

namespace i6e {
namespace modules {

	ObjectMailbox::ObjectMailbox(ObjectManager * manager) : _manager(manager) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
	}

	ObjectMailbox::~ObjectMailbox() {
		ASSERT_THREAD_SAFETY_FUNCTION
	}

	void ObjectMailbox::NewsObjectManager(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		core::Method method = msg->getMethod();
		if (method == core::Method::Create) {
			_manager->NewsCreate(msg);
		} else if (method == core::Method::Update) {
			_manager->NewsUpdate(msg);
		} else if (method == core::Method::Delete) {
			_manager->NewsDelete(msg);
		}
	}

	void ObjectMailbox::NewsComponent(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		int64_t goid = msg->getContent()->getWaitID();
		GOPtr go = _manager->getObject(goid);
		if (go == nullptr) {
			ISIXE_LOG_WARN("ObjectMailbox", "NewsComponent: Message for invalid Object: " << msg->getMessageInfo());
			return;
		}

		go->News(msg);
	}

	void ObjectMailbox::NewsObject(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		core::Method method = msg->getMethod();

		if (method == core::Method::Create) {
			_manager->NewsCreate(msg);
		} else if (method == core::Method::Update) {
			int64_t goid = msg->getContent()->getWaitID();
			GOPtr go = _manager->getObject(goid);
			if (go == nullptr) {
				ISIXE_THROW_FAILURE("ObjectMailbox", "NewsComponent: Message for invalid Object: " << msg->getMessageInfo());
			}

			go->News(msg);
		} else if (method == core::Method::Delete) {
			_manager->NewsDelete(msg);
		}
	}

} /* namespace modules */
} /* namespace i6e */
