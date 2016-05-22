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

namespace i6e {
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
		 * \brief default constructor, constructs an invalid Key
		 */
		IPKey() : _ip(), _port(0) {
		}

		/**
		 * \brief constructor taking ip and port to create a key
		 */
		IPKey(const std::string & ip, uint16_t port);

		/**
		 * \brief constructor taking a string
		 * \param[in] str formatted like ip:port
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

		uint32_t _ip;
		uint16_t _port;
	};

	ISIXE_CORE_API std::ostream & operator<<(std::ostream & stream, const IPKey & key);

} /* namespace core */
} /* namespace i6e */

#endif /* __I6ENGINE_CORE_IPKEY_H__ */

/**
 * @}
 */
