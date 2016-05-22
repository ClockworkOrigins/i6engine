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

#include "i6engine/core/messaging/IPKey.h"

#include <cmath>
#include <sstream>

namespace i6e {
namespace core {

	IPKey::IPKey(const std::string & ip, uint16_t port) : _ip(), _port(port) {
		size_t pos = 0;
		for (size_t i = 0; i < 4; ++i) {
			size_t bytend = ip.find(".", pos);
			std::stringstream ss(ip.substr(pos, bytend));
			int t;
			ss >> t;
			_ip += uint32_t(std::pow(256, 3 - i) * t);
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
			_ip += uint32_t(std::pow(256, 3 - i) * t);
			pos = bytend + 1;
		}
		std::stringstream sport(str.substr(ipend + 1, str.length() - ipend - 1));
		sport >> _port;
	}

	std::string IPKey::getIP() const {
		std::ostringstream ret;
		ret << int((((_ip / 256) / 256) / 256) % 256) << "." << int(((_ip / 256) / 256) % 256) << "." << int((_ip / 256) % 256) << "." << int(_ip % 256);
		return ret.str();
	}

	bool IPKey::operator==(const IPKey & other) const {
		return _ip == other._ip && _port == other._port;
	}

	bool IPKey::operator!=(const IPKey & other) const {
		return !(*this == other);
	}

	bool IPKey::isValid() const {
		return getIP() != "0.0.0.0" && _port > 0;
	}

	std::string IPKey::toString() const {
		return (isValid()) ? (getIP() + std::string(":") + std::to_string(_port)) : "Invalid IP";
	}

	std::ostream & operator<<(std::ostream & stream, const IPKey & key) {
		stream << key.toString();
		return stream;
	}

} /* namespace core */
} /* namespace i6e */
