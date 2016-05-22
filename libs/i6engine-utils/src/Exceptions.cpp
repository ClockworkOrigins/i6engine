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
