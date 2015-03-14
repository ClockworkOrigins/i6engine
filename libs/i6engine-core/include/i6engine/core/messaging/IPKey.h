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

#ifndef __I6ENGINE_CORE_IPKEY_H__
#define __I6ENGINE_CORE_IPKEY_H__

#include <cstdint>
#include <string>
#include <utility>

#include "i6engine/utils/i6eSystemParameters.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	#pragma warning(disable : 4351)
#endif

namespace boost {
namespace serialization {
	class access;
} /* namespace serialization */
} /* namespace boost */

namespace i6engine {
namespace core {

	/**
	 * \class IPKey
	 * Represents the IP and Port of a PC.
	 * This class is used to identify the clients.
	 */
	class ISIXE_CORE_API IPKey {
		friend class boost::serialization::access;
		friend ISIXE_CORE_API std::ostream & operator<<(std::ostream & stream, const IPKey & key);

	public:
		/**
		 * \brief default constructor, construction an invalid Key
		 */
		IPKey() : _ip(), _port(0) {
		}

		/**
		 * \brief constructor taking ip and port to create a key
		 */
		IPKey(const std::string & ip, uint16_t port);

		/**
		 * \brief constructor taking a string
		 * \param[in] str formated like ip:port
		 */
		explicit IPKey(const std::string & str);

		/**
		 * \brief destructor
		 */
		~IPKey() {}

		/**
		 * \brief operator to compare two keys
		 */
		bool operator==(const IPKey & other) const;
		bool operator!=(const IPKey & other) const;

		/**
		 * \brief returns the ip of the key
		 */
		std::string getIP() const;

		/**
		 * \brief returns the port of the key
		 */
		uint16_t getPort() const {
			return _port;
		}

		/**
		 * \brief Returns true if this is a valid IP/Port combination.
		 *
		 * This method only does a simple check for an empty IP and for an Port = 0.
		 * An default constructed IPKey is never valid.
		 */
		bool isValid() const;

		/**
		 * \brief Returns the string representation of this IPKey.
		 *
		 * Returns IP:Port if this is a valid IP and "Invalid IP" otherwise
		 */
		std::string toString() const;

		/**
		 * Enables use as a key in an std::unordered_map
		 */
		class Hash {
		public:
			std::size_t operator()(const IPKey & key) const {
				return std::hash<std::string>()(key.toString());
			}
		};

	private:
		/**
		 * \brief Prepares a IPKey to be sent over the Network.
		 */
		template <typename Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & _ip;
			ar & _port;
		}

		uint8_t _ip[4];
		uint16_t _port;
	};

	ISIXE_CORE_API std::ostream & operator<<(std::ostream & stream, const IPKey & key);

} /* namespace core */
} /* namespace i6engine */

#endif /* __I6ENGINE_CORE_IPKEY_H__ */

/**
 * @}
 */
