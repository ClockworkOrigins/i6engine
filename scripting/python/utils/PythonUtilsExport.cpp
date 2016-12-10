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

#include "i6engine/utils/i6eRandom.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"
#include "i6engine/utils/Semaphore.h"
#include "i6engine/utils/Splash.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace utils {

	uint32_t rand() {
		return i6e::utils::Random::GetSingleton().rand();
	}

	uint32_t rand(uint32_t max) {
		return i6e::utils::Random::GetSingleton().rand(max);
	}

	uint32_t rand(uint32_t min, uint32_t max) {
		return i6e::utils::Random::GetSingleton().rand(min, max);
	}

	void setSeed(uint32_t seed) {
		return i6e::utils::Random::GetSingleton().setSeed(seed);
	}

	void logDebug(const std::string & module, const std::string & message) {
		ISIXE_LOG_DEBUG(module, message);
	}

	void logInfo(const std::string & module, const std::string & message) {
		ISIXE_LOG_INFO(module, message);
	}

	void logWarn(const std::string & module, const std::string & message) {
		ISIXE_LOG_WARN(module, message);
	}

	void logError(const std::string & module, const std::string & message) {
		ISIXE_LOG_ERROR(module, message);
	}

	void logFatal(const std::string & module, const std::string & message) {
		ISIXE_LOG_FATAL(module, message);
	}

	void setLogLevel(i6e::utils::LogLevel level) {
		ISIXE_LOG_SETLEVEL(level);
	}

} /* namespace utils */
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingUtilsPython) {
	using namespace boost::python;
	def("rand", static_cast<uint32_t(*)()>(&i6e::python::utils::rand));
	def("rand", static_cast<uint32_t(*)(uint32_t)>(&i6e::python::utils::rand));
	def("rand", static_cast<uint32_t(*)(uint32_t, uint32_t)>(&i6e::python::utils::rand));
	def("setSeed", &i6e::python::utils::setSeed);
	def("split", &i6e::utils::split);

	enum_<i6e::utils::LogLevel>("LogLevel")
		.value("LOGDEBUG", i6e::utils::LogLevel::LOGDEBUG)
		.value("LOGINFO", i6e::utils::LogLevel::LOGINFO)
		.value("LOGWARN", i6e::utils::LogLevel::LOGWARN)
		.value("LOGERROR", i6e::utils::LogLevel::LOGERROR)
		.value("LOGFATAL", i6e::utils::LogLevel::LOGFATAL)
		.value("LOGNONE", i6e::utils::LogLevel::LOGNONE)
		.export_values();

	class_<i6e::utils::Semaphore, boost::noncopyable>("Semaphore")
		.def(init<>())
		.def("notify", &i6e::utils::Semaphore::notify)
		.def("wait", &i6e::utils::Semaphore::wait);

	class_<i6e::utils::Splash, boost::noncopyable>("Splash")
		.def(init<>())
		.def("showSplash", &i6e::utils::Splash::showSplash)
		.def("closeSplash", &i6e::utils::Splash::closeSplash);
}
