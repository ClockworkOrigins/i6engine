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

#include "LuaCoreExport.h"

#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/Message.h"

#include "i6engine/luabind/operator.hpp"

namespace i6engine {
namespace lua {
namespace core {

	class Subsystem {
	public:
		enum Type {
			Unknown,
			Application,
			Audio,
			Graphic,
			GUI,
			Input,
			Network,
			Object,
			Physic,
			Scripting
		};
	};

	class Method {
	public:
		enum Type {
			Create,
			Update,
			Delete
		};
	};

	i6engine::core::Message::Ptr createMessage(uint16_t messageType, uint16_t subtype, const Method::Type method, i6engine::core::MessageStruct * content, Subsystem::Type sender) {
		return boost::make_shared<i6engine::core::Message>(messageType, subtype, i6engine::core::Method(int(method)), content, i6engine::core::Subsystem(int(sender)));
	}

	Method::Type getMethod(i6engine::core::Message * m) {
		return Method::Type(int(m->getMethod()));
	}

	Subsystem::Type getSender(i6engine::core::Message * m) {
		return Subsystem::Type(int(m->getSender()));
	}

	struct MessageStructWrapper : i6engine::core::MessageStruct, luabind::wrap_base {
		MessageStructWrapper() : MessageStruct() {
		}

		MessageStructWrapper(const int64_t id, const i6engine::core::IPKey & send, const int64_t waitID) : MessageStruct(id, send, waitID) {
		}

		MessageStructWrapper(int64_t id, int64_t waitID) : MessageStruct(id, waitID) {
		}

		virtual MessageStruct * copy() {
			return luabind::call_member<MessageStructWrapper *>(this, "copy");
		}

		static MessageStruct * default_copy(i6engine::core::MessageStruct * ptr) {
			return ptr->MessageStruct::copy();
		}
	};

} /* namespace core */
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerCore() {
	return
		class_<i6engine::lua::core::Subsystem>("Subsystem")
			.def(constructor<>())
			.enum_("Subsystem")
			[
				value("Unknown", i6engine::lua::core::Subsystem::Type::Unknown),
				value("Application", i6engine::lua::core::Subsystem::Type::Application),
				value("Audio", i6engine::lua::core::Subsystem::Type::Audio),
				value("Graphic", i6engine::lua::core::Subsystem::Type::Graphic),
				value("GUI", i6engine::lua::core::Subsystem::Type::GUI),
				value("Input", i6engine::lua::core::Subsystem::Type::Input),
				value("Network", i6engine::lua::core::Subsystem::Type::Network),
				value("Object", i6engine::lua::core::Subsystem::Type::Object),
				value("Physic", i6engine::lua::core::Subsystem::Type::Physic),
				value("Scripting", i6engine::lua::core::Subsystem::Type::Scripting)
			],

		class_<i6engine::core::IPKey>("IPKey")
			.def(constructor<>())
			.def(constructor<const std::string &, uint16_t>())
			.def(constructor<const std::string &>())
			.def(self == self)
			.def("getIP", &i6engine::core::IPKey::getIP)
			.def("getPort", &i6engine::core::IPKey::getPort)
			.def("isValid", &i6engine::core::IPKey::isValid)
			.def("toString", &i6engine::core::IPKey::toString)
			.def(tostring(self)),
			
		class_<i6engine::lua::core::Method>("Method")
			.def(constructor<>())
			.enum_("Method")
			[
				value("Create", i6engine::lua::core::Method::Type::Create),
				value("Update", i6engine::lua::core::Method::Type::Update),
				value("Delete", i6engine::lua::core::Method::Type::Delete)
			],

		def("createMessage", &i6engine::lua::core::createMessage),

		class_<i6engine::core::Message, boost::shared_ptr<i6engine::core::Message>>("Message")
			.def(constructor<>())
			.def("getMessageType", &i6engine::core::Message::getMessageType)
			.def("getSubtype", &i6engine::core::Message::getSubtype)
			.def("getMethod", &i6engine::lua::core::getMethod)
			.def("setMessageType", &i6engine::core::Message::setMessageType)
			.def("getSender", &i6engine::lua::core::getSender)
			.def("getMessageInfo", &i6engine::core::Message::getMessageInfo),

		class_<i6engine::core::MessageStruct, i6engine::lua::core::MessageStructWrapper>("MessageStruct")
			.def(constructor<>())
			.def(constructor<const int64_t, const i6engine::core::IPKey &, const int64_t>())
			.def(constructor<const int64_t, const int64_t>())
			.def("copy", &i6engine::core::MessageStruct::copy, &i6engine::lua::core::MessageStructWrapper::default_copy)
			.def("getID", &i6engine::core::MessageStruct::getID)
			.def("getWaitID", &i6engine::core::MessageStruct::getWaitID)
			.def_readwrite("id", &i6engine::core::MessageStruct::_id)
			.def_readwrite("sender", &i6engine::core::MessageStruct::_sender)
			.def_readwrite("waitID", &i6engine::core::MessageStruct::_waitForId)
		;
}
