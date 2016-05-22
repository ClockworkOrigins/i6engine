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

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/core/messaging/IPKey.h"
#include "i6engine/core/messaging/Message.h"
#include "i6engine/core/messaging/MessageSubscriber.h"
#include "i6engine/core/subsystem/ModuleController.h"

#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace core {

	i6e::core::Message::Ptr createMessage(uint16_t messageType, uint16_t subtype, const i6e::core::Method method, i6e::core::MessageStruct * content, i6e::core::Subsystem sender) {
		return boost::make_shared<i6e::core::Message>(messageType, subtype,method, content, sender);
	}

	struct MessageStructWrapper : public i6e::core::MessageStruct, public boost::python::wrapper<i6e::core::MessageStruct> {
		MessageStructWrapper() : MessageStruct(), boost::python::wrapper<i6e::core::MessageStruct>() {
		}

		MessageStructWrapper(const int64_t id, const i6e::core::IPKey & send, const int64_t waitID) : MessageStruct(id, send, waitID), boost::python::wrapper<i6e::core::MessageStruct>() {
		}

		MessageStructWrapper(int64_t id, int64_t waitID) : MessageStruct(id, waitID), boost::python::wrapper<i6e::core::MessageStruct>() {
		}

		MessageStructWrapper(const i6e::core::MessageStruct & arg) : i6e::core::MessageStruct(arg), boost::python::wrapper<i6e::core::MessageStruct>() {
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

	struct MessageSubscriberWrapper : public i6e::core::MessageSubscriber, public boost::python::wrapper<i6e::core::MessageSubscriber> {
		MessageSubscriberWrapper() : MessageSubscriber(), boost::python::wrapper<i6e::core::MessageSubscriber>() {
		}

		MessageSubscriberWrapper(const i6e::core::MessageSubscriber &) : i6e::core::MessageSubscriber(), boost::python::wrapper<i6e::core::MessageSubscriber>() {
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

	struct ModuleControllerWrapper : public i6e::core::ModuleController, public boost::python::wrapper<i6e::core::ModuleController> {
		ModuleControllerWrapper() : ModuleController(i6e::core::Subsystem::Unknown), boost::python::wrapper<i6e::core::ModuleController>() {
		}

		ModuleControllerWrapper(i6e::core::Subsystem s) : ModuleController(s), boost::python::wrapper<i6e::core::ModuleController>() {
		}

		ModuleControllerWrapper(const i6e::core::ModuleController & arg) : i6e::core::ModuleController(arg.getSubsystem()), boost::python::wrapper<i6e::core::ModuleController>() {
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

		void registerMessageType(uint16_t msgType, const std::string & func) {
			i6e::api::EngineController::GetSingleton().getMessagingFacade()->registerMessageType(msgType, this, [this, func](const i6e::core::Message::Ptr & msg) {
				boost::python::call<void>(this->get_override(func.c_str()).ptr(), msg);
			});
		}

		void unregisterMessageType(uint16_t msgType) {
			i6e::api::EngineController::GetSingleton().getMessagingFacade()->unregisterMessageType(msgType, this);
		}
	};

} /* namespace core */
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingCorePython) {
	using namespace boost::python;

	enum_<i6e::core::Subsystem>("Subsystem")
		.value("Unknown", i6e::core::Subsystem::Unknown)
		.value("Application", i6e::core::Subsystem::Application)
		.value("Audio", i6e::core::Subsystem::Audio)
		.value("Graphic", i6e::core::Subsystem::Graphic)
		.value("GUI", i6e::core::Subsystem::GUI)
		.value("Input", i6e::core::Subsystem::Input)
		.value("Network", i6e::core::Subsystem::Network)
		.value("Object", i6e::core::Subsystem::Object)
		.value("Physic", i6e::core::Subsystem::Physic)
		.value("Scripting", i6e::core::Subsystem::Scripting)
		.export_values();

	class_<i6e::core::IPKey>("IPKey")
		.def(init<>())
		.def(init<const std::string &, uint16_t>())
		.def(init<const std::string &>())
		.def(self == self)
		.def(self != self)
		.def("getIP", &i6e::core::IPKey::getIP)
		.def("getPort", &i6e::core::IPKey::getPort)
		.def("isValid", &i6e::core::IPKey::isValid)
		.def("toString", &i6e::core::IPKey::toString);

	enum_<i6e::core::Method>("Method")
		.value("Create", i6e::core::Method::Create)
		.value("Update", i6e::core::Method::Update)
		.value("Delete", i6e::core::Method::Delete)
		.export_values();

	class_<i6e::core::Message, boost::shared_ptr<i6e::core::Message>>("Message")
		.def(init<>())
		.def("getMessageType", &i6e::core::Message::getMessageType)
		.def("getSubtype", &i6e::core::Message::getSubtype)
		.def("getMethod", &i6e::core::Message::getMethod)
		.def("setMessageType", &i6e::core::Message::setMessageType)
		.def("getSender", &i6e::core::Message::getSender)
		.def("getMessageInfo", &i6e::core::Message::getMessageInfo)
		.def("getContent", &i6e::core::Message::getContent, return_internal_reference<>());

	class_<i6e::python::core::MessageStructWrapper, boost::noncopyable>("MessageStruct")
		.def(init<>())
		.def(init<const int64_t, const i6e::core::IPKey &, const int64_t>())
		.def(init<const int64_t, const int64_t>())
		.def("copy", &i6e::core::MessageStruct::copy, &i6e::python::core::MessageStructWrapper::default_copy, return_internal_reference<>())
		.def("getID", &i6e::core::MessageStruct::getID)
		.def("getWaitID", &i6e::core::MessageStruct::getWaitID)
		.def_readwrite("id", &i6e::core::MessageStruct::_id)
		.def_readwrite("sender", &i6e::core::MessageStruct::_sender)
		.def_readwrite("waitID", &i6e::core::MessageStruct::_waitForId);

	class_<i6e::python::core::MessageSubscriberWrapper>("MessageSubscriber")
		.def(init<>())
		.def("processMessages", &i6e::core::MessageSubscriber::processMessages, &i6e::python::core::MessageSubscriberWrapper::default_processMessages)
		.def("notifyNewID", &i6e::core::MessageSubscriber::notifyNewID);

	class_<i6e::python::core::ModuleControllerWrapper, boost::noncopyable>("ModuleController")
		.def(init<>())
		.def(init<i6e::core::Subsystem>())
		.def("OnThreadStart", pure_virtual(&i6e::core::ModuleController::OnThreadStart))
		.def("Tick", pure_virtual(&i6e::core::ModuleController::Tick))
		.def("ShutDown", pure_virtual(&i6e::core::ModuleController::ShutDown))
		.def("registerMessageType", &i6e::python::core::ModuleControllerWrapper::registerMessageType)
		.def("unregisterMessageType", &i6e::python::core::ModuleControllerWrapper::unregisterMessageType)
		.def("getFrameTime", &i6e::core::ModuleController::getFrameTime);
}
