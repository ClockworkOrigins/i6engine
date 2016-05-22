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

#include "i6engine/core/messaging/Message.h"

#include <sstream>

#include "i6engine/utils/Exceptions.h"

namespace i6e {
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
			ISIXE_THROW_FAILURE("Message", "getMessageInfo() unknown message method: " << int(_method));
		}
		}
		ss << ", ID: " << _content->_id << ", waitForId: " << _content->_waitForId << ", sender: " << _content->_sender;
		return ss.str();
	}

	std::string Message::Serialize() const {
		std::stringstream ss;
		boost::archive::text_oarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		// Needed to compile on some compilers, because operator<< requires a non-const reference.
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

#ifdef ISIXE_WITH_PROFILING
	void Message::insertTimestamp(const std::string & text) {
		_content->insertTimestamp(text);
	}

	std::string Message::getTimestamps() {
		return _content->getTimestamps();
	}
#endif /* ISIXE_WITH_PROFILING */

} /* namespace core */
} /* namespace i6e */

BOOST_CLASS_EXPORT_GUID(i6e::core::Message, "0")
BOOST_CLASS_IMPLEMENTATION(i6e::core::Message, boost::serialization::object_serializable)
BOOST_CLASS_EXPORT_GUID(i6e::core::MessageStruct, "1")
BOOST_CLASS_IMPLEMENTATION(i6e::core::MessageStruct, boost::serialization::object_serializable)
