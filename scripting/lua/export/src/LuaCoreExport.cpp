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
#include "i6engine/core/messaging/MessageSubscriber.h"
#include "i6engine/core/subsystem/ModuleController.h"

#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/luabind/operator.hpp"

namespace i6e {
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

	i6e::core::Message::Ptr createMessage(uint16_t messageType, uint16_t subtype, const Method::Type method, i6e::core::MessageStruct * content, Subsystem::Type sender) {
		return boost::make_shared<i6e::core::Message>(messageType, subtype, i6e::core::Method(int(method)), content, i6e::core::Subsystem(int(sender)));
	}

	Method::Type getMethod(i6e::core::Message * m) {
		return Method::Type(int(m->getMethod()));
	}

	Subsystem::Type getSender(i6e::core::Message * m) {
		return Subsystem::Type(int(m->getSender()));
	}

	struct MessageStructWrapper : public i6e::core::MessageStruct, public luabind::wrap_base {
		MessageStructWrapper() : MessageStruct(), luabind::wrap_base() {
		}

		MessageStructWrapper(const int64_t id, const i6e::core::IPKey & send, const int64_t waitID) : MessageStruct(id, send, waitID), luabind::wrap_base() {
		}

		MessageStructWrapper(int64_t id, int64_t waitID) : MessageStruct(id, waitID), luabind::wrap_base() {
		}

		virtual MessageStruct * copy() {
			return luabind::call_member<MessageStructWrapper *>(this, "copy");
		}

		static MessageStruct * default_copy(i6e::core::MessageStruct * ptr) {
			return ptr->MessageStruct::copy();
		}
	};

	struct MessageSubscriberWrapper : public i6e::core::MessageSubscriber, public luabind::wrap_base {
		MessageSubscriberWrapper() : MessageSubscriber(), luabind::wrap_base() {
		}

		virtual void processMessages() {
			luabind::call_member<void>(this, "processMessages");
		}

		static void default_processMessages(i6e::core::MessageSubscriber * ptr) {
			ptr->MessageSubscriber::processMessages();
		}
	};

	struct ModuleControllerWrapper : i6e::core::ModuleController, public luabind::wrap_base {
		ModuleControllerWrapper(Subsystem::Type s) : ModuleController(i6e::core::Subsystem(int(s))), luabind::wrap_base() {
		}

		virtual void OnThreadStart() {
			luabind::call_member<void>(this, "OnThreadStart");
		}

		virtual void Tick() {
			luabind::call_member<void>(this, "Tick");
		}

		virtual void ShutDown() {
			luabind::call_member<void>(this, "ShutDown");
		}

		void registerMessageType(int msgType, const std::string & func) {
			i6e::api::EngineController::GetSingleton().getMessagingFacade()->registerMessageType(msgType, this, [this, func](const i6e::core::Message::Ptr & msg) {
				luabind::call_member<void>(this, func.c_str(), msg);
			});
		}

		void unregisterMessageType(int msgType) {
			i6e::api::EngineController::GetSingleton().getMessagingFacade()->unregisterMessageType(msgType, this);
		}
	};

} /* namespace core */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerCore() {
	return
		class_<i6e::lua::core::Subsystem>("Subsystem")
			.def(constructor<>())
			.enum_("Subsystem")
			[
				value("Unknown", i6e::lua::core::Subsystem::Type::Unknown),
				value("Application", i6e::lua::core::Subsystem::Type::Application),
				value("Audio", i6e::lua::core::Subsystem::Type::Audio),
				value("Graphic", i6e::lua::core::Subsystem::Type::Graphic),
				value("GUI", i6e::lua::core::Subsystem::Type::GUI),
				value("Input", i6e::lua::core::Subsystem::Type::Input),
				value("Network", i6e::lua::core::Subsystem::Type::Network),
				value("Object", i6e::lua::core::Subsystem::Type::Object),
				value("Physic", i6e::lua::core::Subsystem::Type::Physic),
				value("Scripting", i6e::lua::core::Subsystem::Type::Scripting)
			],

		class_<i6e::core::IPKey>("IPKey")
			.def(constructor<>())
			.def(constructor<const std::string &, uint16_t>())
			.def(constructor<const std::string &>())
			.def(self == self)
			.def("getIP", &i6e::core::IPKey::getIP)
			.def("getPort", &i6e::core::IPKey::getPort)
			.def("isValid", &i6e::core::IPKey::isValid)
			.def("toString", &i6e::core::IPKey::toString)
			.def(tostring(self)),
			
		class_<i6e::lua::core::Method>("Method")
			.def(constructor<>())
			.enum_("Method")
			[
				value("Create", i6e::lua::core::Method::Type::Create),
				value("Update", i6e::lua::core::Method::Type::Update),
				value("Delete", i6e::lua::core::Method::Type::Delete)
			],

		def("createMessage", &i6e::lua::core::createMessage),

		class_<i6e::core::Message, boost::shared_ptr<i6e::core::Message>>("Message")
			.def(constructor<>())
			.def("getMessageType", &i6e::core::Message::getMessageType)
			.def("getSubtype", &i6e::core::Message::getSubtype)
			.def("getMethod", &i6e::lua::core::getMethod)
			.def("setMessageType", &i6e::core::Message::setMessageType)
			.def("getSender", &i6e::lua::core::getSender)
			.def("getMessageInfo", &i6e::core::Message::getMessageInfo)
			.def("getContent", &i6e::core::Message::getContent),

		class_<i6e::core::MessageStruct, i6e::lua::core::MessageStructWrapper>("MessageStruct")
			.def(constructor<>())
			.def(constructor<const int64_t, const i6e::core::IPKey &, const int64_t>())
			.def(constructor<const int64_t, const int64_t>())
			.def("copy", &i6e::core::MessageStruct::copy, &i6e::lua::core::MessageStructWrapper::default_copy)
			.def("getID", &i6e::core::MessageStruct::getID)
			.def("getWaitID", &i6e::core::MessageStruct::getWaitID)
			.def_readwrite("id", &i6e::core::MessageStruct::_id)
			.def_readwrite("sender", &i6e::core::MessageStruct::_sender)
			.def_readwrite("waitID", &i6e::core::MessageStruct::_waitForId),

		class_<i6e::core::MessageSubscriber, i6e::lua::core::MessageSubscriberWrapper>("MessageSubscriber")
			.def(constructor<>())
			.def("processMessages", &i6e::core::MessageSubscriber::processMessages, &i6e::lua::core::MessageSubscriberWrapper::default_processMessages)
			.def("notifyNewID", &i6e::core::MessageSubscriber::notifyNewID),

		class_<i6e::core::ModuleController, i6e::lua::core::ModuleControllerWrapper>("ModuleController")
			.def(constructor<i6e::lua::core::Subsystem::Type>())
			.def("OnThreadStart", &i6e::lua::core::ModuleControllerWrapper::OnThreadStart)
			.def("Tick", &i6e::lua::core::ModuleControllerWrapper::Tick)
			.def("ShutDown", &i6e::lua::core::ModuleControllerWrapper::ShutDown)
			.def("registerMessageType", &i6e::lua::core::ModuleControllerWrapper::registerMessageType)
			.def("unregisterMessageType", &i6e::lua::core::ModuleControllerWrapper::unregisterMessageType)
			.def("getFrameTime", &i6e::core::ModuleController::getFrameTime)
		;
}
