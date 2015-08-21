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

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/messaging/MessageSubscriber.h"
#include "i6engine/core/subsystem/ModuleController.h"

#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace core {

	i6engine::core::Message::Ptr createMessage(uint16_t messageType, uint16_t subtype, const i6engine::core::Method method, i6engine::core::MessageStruct * content, i6engine::core::Subsystem sender) {
		return boost::make_shared<i6engine::core::Message>(messageType, subtype,method, content, sender);
	}

	struct MessageStructWrapper : public i6engine::core::MessageStruct, public boost::python::wrapper<i6engine::core::MessageStruct> {
		MessageStructWrapper() : MessageStruct(), boost::python::wrapper<i6engine::core::MessageStruct>() {
		}

		MessageStructWrapper(const int64_t id, const i6engine::core::IPKey & send, const int64_t waitID) : MessageStruct(id, send, waitID), boost::python::wrapper<i6engine::core::MessageStruct>() {
		}

		MessageStructWrapper(int64_t id, int64_t waitID) : MessageStruct(id, waitID), boost::python::wrapper<i6engine::core::MessageStruct>() {
		}

		MessageStructWrapper(const i6engine::core::MessageStruct & arg) : i6engine::core::MessageStruct(arg), boost::python::wrapper<i6engine::core::MessageStruct>() {
		}

		virtual MessageStruct * copy() {
			if (boost::python::override c = this->get_override("copy")) {
				return boost::python::call<MessageStruct *>(c.ptr());
			}
			return MessageStruct::copy();
		}

		MessageStruct * default_copy() {
			return this->MessageStruct::copy();
		}
	};

	struct MessageSubscriberWrapper : public i6engine::core::MessageSubscriber, public boost::python::wrapper<i6engine::core::MessageSubscriber> {
		MessageSubscriberWrapper() : MessageSubscriber(), boost::python::wrapper<i6engine::core::MessageSubscriber>() {
		}

		MessageSubscriberWrapper(const i6engine::core::MessageSubscriber & arg) : i6engine::core::MessageSubscriber(), boost::python::wrapper<i6engine::core::MessageSubscriber>() {
		}

		virtual void processMessages() {
			if (boost::python::override pm = this->get_override("processMessages")) {
				boost::python::call<void>(pm.ptr());
				return;
			}
			MessageSubscriber::processMessages();
		}

		void default_processMessages() {
			this->MessageSubscriber::processMessages();
		}
	};

	struct ModuleControllerWrapper : public i6engine::core::ModuleController, public boost::python::wrapper<i6engine::core::ModuleController> {
		ModuleControllerWrapper() : ModuleController(i6engine::core::Subsystem::Unknown), boost::python::wrapper<i6engine::core::ModuleController>() {
		}

		ModuleControllerWrapper(i6engine::core::Subsystem s) : ModuleController(s), boost::python::wrapper<i6engine::core::ModuleController>() {
		}

		ModuleControllerWrapper(const i6engine::core::ModuleController & arg) : i6engine::core::ModuleController(arg.getSubsystem()), boost::python::wrapper<i6engine::core::ModuleController>() {
		}

		virtual void OnThreadStart() {
			boost::python::call<void>(this->get_override("OnThreadStart").ptr());
		}

		virtual void Tick() {
			boost::python::call<void>(this->get_override("Tick").ptr());
		}

		virtual void ShutDown() {
			boost::python::call<void>(this->get_override("ShutDown").ptr());
		}

		void registerMessageType(int msgType, const std::string & func) {
			i6engine::api::EngineController::GetSingleton().getMessagingFacade()->registerMessageType(msgType, this, [this, func](const i6engine::core::Message::Ptr & msg) {
				boost::python::call<void>(this->get_override(func.c_str()).ptr());
			});
		}

		void unregisterMessageType(int msgType) {
			i6engine::api::EngineController::GetSingleton().getMessagingFacade()->unregisterMessageType(msgType, this);
		}
	};

} /* namespace core */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingCorePython) {
	using namespace boost::python;

	enum_<i6engine::core::Subsystem>("Subsystem")
		.value("Unknown", i6engine::core::Subsystem::Unknown)
		.value("Application", i6engine::core::Subsystem::Application)
		.value("Audio", i6engine::core::Subsystem::Audio)
		.value("Graphic", i6engine::core::Subsystem::Graphic)
		.value("GUI", i6engine::core::Subsystem::GUI)
		.value("Input", i6engine::core::Subsystem::Input)
		.value("Network", i6engine::core::Subsystem::Network)
		.value("Object", i6engine::core::Subsystem::Object)
		.value("Physic", i6engine::core::Subsystem::Physic)
		.value("Scripting", i6engine::core::Subsystem::Scripting)
		.export_values();

	class_<i6engine::core::IPKey>("IPKey")
		.def(init<>())
		.def(init<const std::string &, uint16_t>())
		.def(init<const std::string &>())
		.def(self == self)
		.def(self != self)
		.def("getIP", &i6engine::core::IPKey::getIP)
		.def("getPort", &i6engine::core::IPKey::getPort)
		.def("isValid", &i6engine::core::IPKey::isValid)
		.def("toString", &i6engine::core::IPKey::toString);

	enum_<i6engine::core::Method>("Method")
		.value("Create", i6engine::core::Method::Create)
		.value("Update", i6engine::core::Method::Update)
		.value("Delete", i6engine::core::Method::Delete)
		.export_values();

	class_<i6engine::core::Message, boost::shared_ptr<i6engine::core::Message>>("Message")
		.def(init<>())
		.def("getMessageType", &i6engine::core::Message::getMessageType)
		.def("getSubtype", &i6engine::core::Message::getSubtype)
		.def("getMethod", &i6engine::core::Message::getMethod)
		.def("setMessageType", &i6engine::core::Message::setMessageType)
		.def("getSender", &i6engine::core::Message::getSender)
		.def("getMessageInfo", &i6engine::core::Message::getMessageInfo)
		.def("getContent", &i6engine::core::Message::getContent, return_internal_reference<>());

	class_<i6engine::python::core::MessageStructWrapper, boost::noncopyable>("MessageStruct")
		.def(init<>())
		.def(init<const int64_t, const i6engine::core::IPKey &, const int64_t>())
		.def(init<const int64_t, const int64_t>())
		.def("copy", &i6engine::core::MessageStruct::copy, &i6engine::python::core::MessageStructWrapper::default_copy, return_internal_reference<>())
		.def("getID", &i6engine::core::MessageStruct::getID)
		.def("getWaitID", &i6engine::core::MessageStruct::getWaitID)
		.def_readwrite("id", &i6engine::core::MessageStruct::_id)
		.def_readwrite("sender", &i6engine::core::MessageStruct::_sender)
		.def_readwrite("waitID", &i6engine::core::MessageStruct::_waitForId);

	class_<i6engine::python::core::MessageSubscriberWrapper>("MessageSubscriber")
		.def(init<>())
		.def("processMessages", &i6engine::core::MessageSubscriber::processMessages, &i6engine::python::core::MessageSubscriberWrapper::default_processMessages)
		.def("notifyNewID", &i6engine::core::MessageSubscriber::notifyNewID);

	class_<i6engine::python::core::ModuleControllerWrapper, boost::noncopyable>("ModuleController")
		.def(init<>())
		.def(init<i6engine::core::Subsystem>())
		.def("OnThreadStart", pure_virtual(&i6engine::core::ModuleController::OnThreadStart))
		.def("Tick", pure_virtual(&i6engine::core::ModuleController::Tick))
		.def("ShutDown", pure_virtual(&i6engine::core::ModuleController::ShutDown))
		.def("registerMessageType", &i6engine::python::core::ModuleControllerWrapper::registerMessageType)
		.def("unregisterMessageType", &i6engine::python::core::ModuleControllerWrapper::unregisterMessageType)
		.def("getFrameTime", &i6engine::core::ModuleController::getFrameTime);
}
