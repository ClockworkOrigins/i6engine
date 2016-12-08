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

#include "LuaUtilsExport.h"

#include "i6engine/utils/i6eRandom.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"
#include "i6engine/utils/Semaphore.h"
#include "i6engine/utils/Splash.h"

namespace i6e {
namespace lua {
namespace utils {

	class LogLevel {
	public:
		enum LV {
			LOGDEBUG = 0,
			LOGINFO = 1,
			LOGWARN = 2,
			LOGERROR = 3,
			LOGFATAL = 4,
			LOGNONE = 5
		};
	};

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

	void setLogLevel(LogLevel::LV level) {
		ISIXE_LOG_SETLEVEL(i6e::utils::LogLevel(int(level)));
	}

} /* namespace rpg */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerUtils() {
	return
		def("rand", static_cast<uint32_t(*)()>(&i6e::lua::utils::rand)),
		def("rand", static_cast<uint32_t(*)(uint32_t)>(&i6e::lua::utils::rand)),
		def("rand", static_cast<uint32_t(*)(uint32_t, uint32_t)>(&i6e::lua::utils::rand)),
		def("setSeed", &i6e::lua::utils::setSeed),
		def("split", &i6e::utils::split),

		class_<i6e::lua::utils::LogLevel>("LogLevel")
			.def(constructor<>())
			.enum_("LogLevel")
			[
				value("LOGDEBUG", i6e::lua::utils::LogLevel::LV::LOGDEBUG),
				value("LOGINFO", i6e::lua::utils::LogLevel::LV::LOGINFO),
				value("LOGWARN", i6e::lua::utils::LogLevel::LV::LOGWARN),
				value("LOGERROR", i6e::lua::utils::LogLevel::LV::LOGERROR),
				value("LOGFATAL", i6e::lua::utils::LogLevel::LV::LOGFATAL),
				value("LOGNONE", i6e::lua::utils::LogLevel::LV::LOGNONE)
			],

		class_<i6e::utils::Semaphore>("Semaphore")
			.def(constructor<>())
			.def("notify", &i6e::utils::Semaphore::notify)
			.def("wait", &i6e::utils::Semaphore::wait),

		class_<i6e::utils::Splash>("Splash")
			.def(constructor<>())
			.def("showSplash", &i6e::utils::Splash::showSplash)
			.def("closeSplash", &i6e::utils::Splash::closeSplash)
		;
}
