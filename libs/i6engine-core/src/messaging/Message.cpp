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

#include "i6engine/core/messaging/Message.h"

#include <sstream>

#include "i6engine/utils/Exceptions.h"

namespace i6engine {
namespace core {

	const Message & Message::operator=(const Message &) {
		return *this;
	}

	std::string Message::getMessageInfo() const {
		std::stringstream ss;
		ss << "MessageInfo: Type: " << _messageType << ", SubType: " << _subType << ", Method: ";
		switch (_method) {
		case Method::Create: {
			ss << "Create";
			break;
		}
		case Method::Update: {
			ss << "Update";
			break;
		}
		case Method::Delete: {
			ss << "Delete";
			break;
		}
		default: {
			ISIXE_THROW_FAILURE("Message", "getMessageInfo() unknown message method");
			return "";
		}
		}
		ss << ", ID: " << _content->_id << ", waitForId: " << _content->_waitForId << ", sender: " << _content->_sender;
		return ss.str();
	}

	std::string Message::Serialize() const {
		std::stringstream ss;
		boost::archive::text_oarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		// Needed to compile on good compilers, because operator<< requires a non-const reference.
		// See http://stackoverflow.com/questions/23440210/error-no-match-for-operator-using-boostserialisation
		Message * m = const_cast<Message *>(this);
		arch << m;
		return ss.str();
	}

	Message * Message::Deserialize(const std::string & s) {
		std::stringstream ss;
		Message * m;
		ss << s;
		boost::archive::text_iarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		arch >> m;
		return m;
	}

	std::ostream & operator<<(std::ostream & stream, const Message & msg) {
		stream << msg.getMessageInfo();
		return stream;
	}

	std::ostream & operator<<(std::ostream & stream, const Message::Ptr & msg) {
		stream << msg->getMessageInfo();
		return stream;
	}

#ifdef ISIXE_PROFILING
	void Message::insertTimestamp(const std::string & text) {
		_content->insertTimestamp(text);
	}

	std::string Message::getTimestamps() {
		return _content->getTimestamps();
	}
#endif /* ISIXE_PROFILING */

} /* namespace core */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_GUID(i6engine::core::Message, "0")
BOOST_CLASS_IMPLEMENTATION(i6engine::core::Message, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6engine::core::MessageStruct, "1")
BOOST_CLASS_IMPLEMENTATION(i6engine::core::MessageStruct, boost::serialization::object_serializable)
