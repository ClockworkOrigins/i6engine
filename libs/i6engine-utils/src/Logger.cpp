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

#include "i6engine/utils/Logger.h"

#include <sstream>

#include "boost/log/utility/setup/console.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace i6e {
namespace utils {

	Logger::Logger() : _logger(), _logLevel(LogLevel::LOGDEBUG) {
		// Without auto_flush set to true, important log messages may be lost when the program crashes.
		boost::log::add_file_log("log/global.log")->locked_backend()->auto_flush(true);
		boost::log::add_console_log(std::cout);
	}

	void Logger::LogMessage(LogLevel level, const std::string & file, const int line, const std::string & module, const std::string & message) {
		if (level == LogLevel::LOGNONE) {
			ISIXE_LOG_WARN("Logger", "NONE is not a valid LogLevel. Falling back to LOGWARN.");
			level = LogLevel::LOGWARN;
		}

		if (level < _logLevel) {
			return;
		}

		// strange strings for colored output in terminal
		BOOST_LOG(_logger) << logEnumNames[int(level)] << " (" << boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::universal_time()) << "): " << module << " - " << message << " (" << file << ":" << line << ")";
	}

} /* namespace utils */
} /* namespace i6e */
