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

#include "i6engine/utils/Exceptions.h"

#ifndef ISIXE_WITH_LOGGING
	#include <iostream>
#endif /* ISIXE_WITH_LOGGING */

#include "i6engine/utils/ExceptionQueue.h"

namespace i6e {
namespace utils {
namespace exceptions {

	i6exception::i6exception(const LogLevel loglevel, const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line) {
		(*this) << excinfo_log(loginfo(loglevel, logmsg, logmodule, file, line));
	}

	void i6exception::writeLog() {
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
#ifdef ISIXE_WITH_LOGGING
			Logger::GetSingleton().LogMessage(info->level, info->file, info->line, info->module, info->message);
#else /* ISIXE_WITH_LOGGING */
			std::cerr << logEnumNames[int(info->level)] << ": " << info->module << " - " << info->message << " (" << info->file << ":" << info->line << ")" << std::endl;
#endif /* ISIXE_WITH_LOGGING */
		}
	}

	const char * i6exception::what() const throw() {
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
			std::ostringstream ss;
			ss << "(" << info->file << ":" << info->line << ") " << info->module << ": " << info->message;
			return ss.str().c_str();
		}
		return "No Message!";
	}

	SubsystemException::SubsystemException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line) : i6exception(LogLevel::LOGERROR, logmodule, logmsg, file, line) {
	}

	void SubsystemException::PassToMain() {
		if (loginfo const * info = boost::get_error_info<excinfo_log>(*this)) {
			ExceptionQueue::GetSingleton().enqueue(*info);
		} else {
			ISIXE_LOG_FATAL("Exceptions", "Trying to enqueue SubsystemException without loginfo attached.")
		}
	}

	SystemFailureException::SystemFailureException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line) : i6exception(LogLevel::LOGFATAL, logmodule, logmsg, file, line) {
	}

	ApiException::ApiException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line) : i6exception(LogLevel::LOGFATAL, logmodule, logmsg, file, line) {
	}

	MessageException::MessageException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line) : i6exception(LogLevel::LOGFATAL, logmodule, logmsg, file, line) {
	}

} /* namespace exceptions */
} /* namespace utils */
} /* namespace i6e */
