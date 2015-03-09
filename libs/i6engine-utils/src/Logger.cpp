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

#include "i6engine/utils/Logger.h"

#include <sstream>

#include "boost/log/utility/setup/console.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace i6engine {
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
		BOOST_LOG(_logger) << logEnumNames[static_cast<int>(level)] << " (" << boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::universal_time()) << "): " << module << " - " << message << " (" << file << ":" << line << ")";
	}

} /* namespace utils */
} /* namespace i6engine */
