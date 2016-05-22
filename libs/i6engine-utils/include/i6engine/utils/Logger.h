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
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_LOGGER_H__
#define __I6ENGINE_UTILS_LOGGER_H__

#include <string>

#include "i6engine/i6engineBuildSettings.h"

#ifdef ISIXE_WITH_LOGGING
	#include "i6engine/utils/i6eSystemParameters.h"
	#include "i6engine/utils/Singleton.h"

	#include "boost/log/common.hpp"
	#include "boost/log/core.hpp"
	#include "boost/log/sinks.hpp"
#endif

#ifdef ISIXE_WITH_LOGGING
	#define ISIXE_LOG_DEBUG(module, message) {\
		std::stringstream logMessageString; \
		logMessageString << message; \
		i6e::utils::Logger::GetSingleton().LogMessage(i6e::utils::LogLevel::LOGDEBUG, __FILE__, __LINE__, module, logMessageString.str());\
	}
	#define ISIXE_LOG_INFO(module, message) {\
		std::stringstream logMessageString; \
		logMessageString << message; \
		i6e::utils::Logger::GetSingleton().LogMessage(i6e::utils::LogLevel::LOGINFO, __FILE__, __LINE__, module, logMessageString.str());\
	}
	#define ISIXE_LOG_WARN(module, message) {\
		std::stringstream logMessageString; \
		logMessageString << message; \
		i6e::utils::Logger::GetSingleton().LogMessage(i6e::utils::LogLevel::LOGWARN, __FILE__, __LINE__, module, logMessageString.str());\
	}
	#define ISIXE_LOG_ERROR(module, message) {\
		std::stringstream logMessageString; \
		logMessageString << message; \
		i6e::utils::Logger::GetSingleton().LogMessage(i6e::utils::LogLevel::LOGERROR, __FILE__, __LINE__, module, logMessageString.str());\
	}
	#define ISIXE_LOG_FATAL(module, message) {\
		std::stringstream logMessageString; \
		logMessageString << message; \
		i6e::utils::Logger::GetSingleton().LogMessage(i6e::utils::LogLevel::LOGFATAL, __FILE__, __LINE__, module, logMessageString.str());\
	}

	#define ISIXE_LOG_SETLEVEL(level) {\
		i6e::utils::Logger::GetSingleton().setLogLevel(level);\
	}
#else
	#define ISIXE_LOG_DEBUG(module, message) {\
	}
	#define ISIXE_LOG_INFO(module, message) {\
	}
	#define ISIXE_LOG_WARN(module, message) {\
	}
	#define ISIXE_LOG_ERROR(module, message) {\
	}
	#define ISIXE_LOG_FATAL(module, message) {\
	}

	#define ISIXE_LOG_SETLEVEL(level) {\
	}
#endif // ISIXE_WITH_LOGGING

namespace i6e {
namespace utils {

	static const std::string logEnumNames[] = { "Debug", "Info", "Warning", "ERROR", "FATAL", "---" };

	/**
	 * \brief Defines values to specify severity/importance of log message.
	 */
	enum class LogLevel : short {
		LOGDEBUG = 0,
		LOGINFO = 1,
		LOGWARN = 2,
		LOGERROR = 3,
		LOGFATAL = 4,
		LOGNONE = 5
	};

#ifdef ISIXE_WITH_LOGGING
	class Logger;

	typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> TextSink;
	typedef boost::shared_ptr<TextSink> SinkPtr;
	typedef boost::shared_ptr<Logger> LogPtr;

	/**
	 * \class Logger
	 * \brief Implements logging to file and console.
	 *
	 * Allows thread-safe logging to file and console.
	 * \note
	 * Logger.h defines the macros:
	 * \li \c ISIXE_LOG_X("Module", "Message") Writes log message with LogLevel X ('LOGDEBUG', 'LOGINFO', LOG'WARN', 'LOGERROR', 'LOGFATAL', 'LOGNONE')
	 * \li \c ISIXE_LOG_SETLEVEL(level) Sets the new LogLevel (for example: i6e::utils::LogLevel::LOGDEBUG)
	 *
	 * See \ref macrodoc for a list of all globally defined macros.
	 */
	class ISIXE_UTILS_API Logger : public i6e::utils::Singleton<Logger> {
		friend class i6e::utils::Singleton<Logger>;

	public:
		/**
		 * \brief Sets LogLevel. Everything at least as important as logLevel is logged.
		 */
		inline void setLogLevel(const LogLevel logLevel) { _logLevel = logLevel; }

		/**
		 * \brief Writes message to log for different types
		 */
		void LogMessage(LogLevel level, const std::string & file, const int line, const std::string & module, const std::string & message);

	protected:
		Logger();

	private:
		boost::log::sources::channel_logger_mt<std::string> _logger;
		LogLevel _logLevel;
	};
#endif /* ISIXE_WITH_LOGGING */

} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_LOGGER_H__ */

/**
 * @}
 */
