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

/**
 * \addtogroup core
 * @{
 */

#ifndef __I6ENGINE_CORE_MESSAGE_H__
#define __I6ENGINE_CORE_MESSAGE_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/core/messaging/MessageStruct.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/export.hpp"

namespace i6engine {
namespace core {

	enum class Subsystem;

	enum class Method : uint8_t {
		Create,
		Update,
		Delete
	};

	/**
	 * \class Message
	 * \brief Class representing a message to be sent.
	 *
	 * Every message that is sent in the i6m2etis engine is an instance of this class. A message has a certain type defined by a string and
	 * several variables which can be of varying types to describe the content of the message. To learn more about sending and receiving
	 * messages please look at MessagingController.
	 *
	 * The copies of the message are deleted automatically by the shared_ptr. So don't call the destructor on your message. This
	 * will cause a segmentation fault!
	 * \internal To get an overview on the usage and functionality of the whole messaging subsystem in german have a look at our wiki
	 * (topic: 'Messaging in der i6engine (Kurzbeschreibung)' )
	 *
	 * \note
	 *
	 * Have a look at \link i6e here \endlink for an overview of typedef's in namespace %i6e.
	 */
	class ISIXE_CORE_API Message {
	public:
		typedef boost::shared_ptr<Message> Ptr;

		/**
		 * \brief Default constructor
		 * necessary for serialization
		 */
		Message() : _messageType(), _subType(), _method(), _content(), _sender() {
		}

		/**
		 * \brief Constructor
		 * \param strMessageType Message type.
		 * \param subtype Message subtype.
		 * \param method Message method.
		 * \param content Message content.
		 */
		Message(uint16_t messageType, uint16_t subtype, const Method method, MessageStruct * content, Subsystem sender) : _messageType(messageType), _subType(subtype), _method(method), _content(content), _sender(sender) {
		}

		/**
		 * \brief Standard copy constructor.
		 * \param msg Object to be copied.
		 */
		Message(const Message & msg) : _messageType(msg.getMessageType()), _subType(msg.getSubtype()), _method(msg.getMethod()), _content(), _sender(msg.getSender()) {
			if (msg.getContent() != nullptr) {
				_content = msg.getContent()->copy();
			}
		}

		const Message & operator=(const Message &);

		/**
		 * \brief Destructor, deleting message content.
		 */
		virtual ~Message() {
			delete _content;
		}

		/**
		 * \brief Getter for the message type.
		 * \return The message type.
		 */
		inline uint16_t getMessageType() const {
			return _messageType;
		}

		/**
		 * \brief Getter for the message subtype.
		 * \return The message subtype.
		 */
		inline uint16_t getSubtype() const {
			return _subType;
		}

		/**
		 * \brief Getter for the message method.
		 * \return The message method.
		 */
		inline Method getMethod() const { return _method; }

		/**
		 * \brief Getter for the message content.
		 * \return The message content.
		 */
		inline MessageStruct * getContent() const { return _content; }

#ifdef ISIXE_PROFILING
		/**
		 * \bief insert an timestamp into the message struct.
		 *
		 * \param text Some information about the timestamp.
		 */
		void insertTimestamp(const std::string & text);

		/**
		 * \brief returns a string with all saved timestamps
		 */
		std::string getTimestamps();
#endif /* ISIXE_PROFILING */

		/**
		 * \brief Changes the message type to the given parameter.
		 * \param strMesageType New message type.
		 */
		inline void setMessageType(uint16_t messageType) {
			_messageType = messageType;
		}

		/**
		 * \brief returns the Subsystem sending this message
		 */
		Subsystem getSender() const {
			return _sender;
		}

		/**
		 * \brief Gives informations about the message.
		 * \return String containing Type, SubType, Method, ID, waitForId, sender, receiver.
		 */
		std::string getMessageInfo() const;

		std::string Serialize() const;

		static Message * Deserialize(const std::string & s);

	protected:
		friend class boost::serialization::access;

		/**
		 * \brief Prepares a Message to be sent over the Network.
		 * \param ar
		 * \param version
		 */
		template <typename Archive>
		void serialize(Archive & ar, const unsigned int version) {
			ar & _messageType;
			ar & _subType;
			ar & _method;
			ar & _content;
		}

		uint16_t _messageType;
		uint16_t _subType;
		Method _method;

		MessageStruct * _content;

		Subsystem _sender;
	};

	std::ostream & operator<<(std::ostream & stream, const Message & msg);
	std::ostream & operator<<(std::ostream & stream, const Message::Ptr & msg);

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_MESSAGE_H__ */

/**
 * @}
 */
