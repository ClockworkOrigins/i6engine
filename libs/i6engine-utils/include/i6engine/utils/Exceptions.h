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

#ifndef __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONS_H__
#define __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONS_H__

#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Logger.h"

#include "boost/exception/all.hpp"

#define ISIXE_THROW_SUBSYSTEM(module, message) {\
	std::stringstream logMessageString;\
	logMessageString << message;\
	throw i6e::utils::exceptions::SubsystemException(module, logMessageString.str(), __FILE__, __LINE__);\
}

#define ISIXE_THROW_FAILURE(module, message) {\
	std::stringstream logMessageString;\
	logMessageString << message;\
	throw i6e::utils::exceptions::SystemFailureException(module, logMessageString.str(), __FILE__, __LINE__);\
}

#define ISIXE_THROW_API(module, message) {\
	std::stringstream logMessageString;\
	logMessageString << message;\
	throw i6e::utils::exceptions::ApiException(module, logMessageString.str(), __FILE__, __LINE__);\
}

#define ISIXE_THROW_MESSAGE(module, message) {\
	std::stringstream logMessageString;\
	logMessageString << message;\
	throw i6e::utils::exceptions::MessageException(module, logMessageString.str(), __FILE__, __LINE__);\
}

#define ISIXE_THROW_API_COND(module, message, condition) {\
	if (!(condition)) {\
		ISIXE_THROW_API(module, message)\
	}\
}

/**
 * \brief use this to suggest an information how to resolve the exception
 */
#define ISIXE_THROW_API_INFO(module, message, info) {\
	std::stringstream logMessageString;\
	logMessageString << message << "\nPossible solutin: " << info;\
	throw i6e::utils::exceptions::ApiException(module, logMessageString.str(), __FILE__, __LINE__);\
}

namespace i6e {
namespace utils {
namespace exceptions {

	/**
	 * \brief Internal data structure for exceptions.
	 */
	struct ISIXE_UTILS_API loginfo {
		LogLevel level;
		std::string message;
		std::string module;
		std::string file;
		uint16_t line;

	public:
		loginfo(const LogLevel l, const std::string & msg, const std::string & m, const std::string & f, const uint16_t li) : level(l), message(msg), module(m), file(f), line(li) {
		}
	};

	typedef boost::error_info<struct tag_logmessage, loginfo> excinfo_log;

	/**
	 * \class i6exception
	 * \brief Base type for custom exceptions.
	 *
	 * Base type for custom exceptions.
	 */
	struct i6exception : virtual boost::exception, virtual std::exception {
	public:
		/**
		 * \brief Writes the exception's logmessage.
		 */
		ISIXE_UTILS_API void writeLog();

		/**
		 * \brief Returns the exception's errorcode or -1 if no errorcode is present.
		 */
		ISIXE_UTILS_API int getErrorcode();

		/**
		 * \brief Overrides std::exception method to output message to console.
		 */
		ISIXE_UTILS_API virtual const char * what() const throw() override;

	protected:
		i6exception(const LogLevel loglevel, const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line);
	};

	/**
	 * \class SubsystemException
	 * \brief Non-fatal, potentially reparable exception within a single subsystem.
	 *
	 * Non-fatal, potentially reparable exception within the confines of a single subsystem.
	 */
	struct SubsystemException : virtual i6exception {
	public:
		ISIXE_UTILS_API SubsystemException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line);

		/**
		 * \brief Passes the exception from the subsystem to the main game loop.
		 */
		ISIXE_UTILS_API void PassToMain();
	};

	/**
	 * \class SystemFailureException
	 * \brief Critical exception that cannot be repaired by a single subsystem.
	 *
	 * Critical exception that cannot be repaired by a single subsystem. Most likely fatal.
	 */
	struct SystemFailureException : virtual i6exception {
	public:
		ISIXE_UTILS_API SystemFailureException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line);
	};

	/**
	 * \class ApiException
	 * \brief Occurs when the engine detects a programmer's error.
	 *
	 * Occurs when the engine detects a programmer's error like null-parameters, wrong call-sequence.
	 */
	struct ApiException : virtual i6exception {
	public:
		ISIXE_UTILS_API ApiException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line);
	};

	/**
	 * \class MessageException
	 * \brief Occurs when a wrong message is detected
	 *
	 * Occurs when a wrong message is detected
	 */
	struct MessageException : virtual i6exception {
	public:
		ISIXE_UTILS_API MessageException(const std::string & logmodule, const std::string & logmsg, const std::string & file, const uint16_t line);
	};

} /* namespace exceptions */
} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_EXCEPTIONS_EXCEPTIONS_H__ */

/**
 * @}
 */
