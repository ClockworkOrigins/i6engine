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

#include "LuaUtilsExport.h"

#include "i6engine/utils/i6eRandom.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"
#include "i6engine/utils/Semaphore.h"
#include "i6engine/utils/Splash.h"

namespace i6engine {
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
		return i6engine::utils::Random::GetSingleton().rand();
	}

	uint32_t rand(uint32_t max) {
		return i6engine::utils::Random::GetSingleton().rand(max);
	}

	uint32_t rand(uint32_t min, uint32_t max) {
		return i6engine::utils::Random::GetSingleton().rand(min, max);
	}

	void setSeed(uint32_t seed) {
		return i6engine::utils::Random::GetSingleton().setSeed(seed);
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
		ISIXE_LOG_SETLEVEL(i6engine::utils::LogLevel(int(level)));
	}

} /* namespace rpg */
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerUtils() {
	return
		def("rand", (uint32_t(*)()) &i6engine::lua::utils::rand),
		def("rand", (uint32_t(*)(uint32_t)) &i6engine::lua::utils::rand),
		def("rand", (uint32_t(*)(uint32_t, uint32_t)) &i6engine::lua::utils::rand),
		def("setSeed", &i6engine::lua::utils::setSeed),
		def("split", &i6engine::utils::split),

		class_<i6engine::lua::utils::LogLevel>("LogLevel")
			.def(constructor<>())
			.enum_("LogLevel")
			[
				value("LOGDEBUG", i6engine::lua::utils::LogLevel::LV::LOGDEBUG),
				value("LOGINFO", i6engine::lua::utils::LogLevel::LV::LOGINFO),
				value("LOGWARN", i6engine::lua::utils::LogLevel::LV::LOGWARN),
				value("LOGERROR", i6engine::lua::utils::LogLevel::LV::LOGERROR),
				value("LOGFATAL", i6engine::lua::utils::LogLevel::LV::LOGFATAL),
				value("LOGNONE", i6engine::lua::utils::LogLevel::LV::LOGNONE)
			],

		class_<i6engine::utils::Semaphore>("Semaphore")
			.def(constructor<>())
			.def("notify", &i6engine::utils::Semaphore::notify)
			.def("wait", &i6engine::utils::Semaphore::wait),

		class_<i6engine::utils::Splash>("Splash")
			.def(constructor<>())
			.def("showSplash", &i6engine::utils::Splash::showSplash)
			.def("closeSplash", &i6engine::utils::Splash::closeSplash)
		;
}
