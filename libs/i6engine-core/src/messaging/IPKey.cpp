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

#include "i6engine/core/messaging/IPKey.h"

#include <sstream>

#include "i6engine/utils/Exceptions.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace core {

	IPKey::IPKey(const std::string & ip, uint16_t port) : _ip(), _port(port) {
		size_t pos = 0;
		for (size_t i = 0; i < 4; ++i) {
			size_t bytend = ip.find(".", pos);
			std::stringstream ss(ip.substr(pos, bytend));
			int t;
			ss >> t;
			_ip[i] = t;
			pos = bytend + 1;
		}
	}

	IPKey::IPKey(const std::string & str) : _ip(), _port() {
		size_t ipend = str.find(":", 0);
		size_t pos = 0;
		for (size_t i = 0; i < 4; ++i) {
			size_t bytend = str.find(".", pos);
			std::stringstream ss(str.substr(pos, bytend));
			int t;
			ss >> t;
			_ip[i] = t;
			pos = bytend + 1;
		}
		std::stringstream sport(str.substr(ipend + 1, str.length() - ipend - 1));
		sport >> _port;
	}

	std::string IPKey::getIP() const {
		std::ostringstream ret;
		ret << int(_ip[0]) << "." << int(_ip[1]) << "." << int(_ip[2]) << "." << int(_ip[3]);
		return ret.str();
	}

	bool IPKey::operator==(const IPKey & other) const {
		return getIP() == other.getIP() && _port == other._port;
	}

	bool IPKey::operator!=(const IPKey & other) const {
		return !(*this == other);
	}

	bool IPKey::isValid() const {
		return getIP() != "0.0.0.0" && _port > 0;
	}

	std::string IPKey::toString() const {
		return (isValid()) ? (getIP() + std::string(":") + boost::lexical_cast<std::string>(_port)) : "Invalid IP";
	}

	std::ostream & operator<<(std::ostream & stream, const IPKey & key) {
		stream << key.toString();
		return stream;
	}

} /* namespace core */
} /* namespace i6engine */
