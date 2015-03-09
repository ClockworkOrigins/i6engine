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

#include "i6engine/modules/object/ObjectMailbox.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/object/ObjectManager.h"

namespace i6engine {
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
			ISIXE_LOG_ERROR("ObjectMailbox", "NewsComponent: Message for invalid Object: " + msg->getMessageInfo());
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
				ISIXE_THROW_FAILURE("ObjectMailbox", "NewsComponent: Message for invalid Object: " + msg->getMessageInfo());
			}

			go->News(msg);
		} else if (method == core::Method::Delete) {
			_manager->NewsDelete(msg);
		}
	}

} /* namespace modules */
} /* namespace i6engine */
