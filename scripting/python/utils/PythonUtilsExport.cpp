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

#include "i6engine/utils/i6eRandom.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"
#include "i6engine/utils/Semaphore.h"
#include "i6engine/utils/Splash.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
	namespace utils {

		uint32_t rand() {
			return i6engine::utils::Random::GetSingleton().rand();
		}

		uint32_t rand(uint32_t max) {
			return i6engine::utils::Random::GetSingleton().rand(max);
		}

		uint32_t rand(uint32_t min, uint32_t max) {
			return i6engine::utils::Random::GetSingleton().rand();
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

		void setLogLevel(i6engine::utils::LogLevel level) {
			ISIXE_LOG_SETLEVEL(level);
		}

} /* namespace utils */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingUtilsPython) {
	using namespace boost::python;
	def("rand", (uint32_t(*)()) &i6engine::python::utils::rand);
	def("rand", (uint32_t(*)(uint32_t)) &i6engine::python::utils::rand);
	def("rand", (uint32_t(*)(uint32_t, uint32_t)) &i6engine::python::utils::rand);
	def("setSeed", &i6engine::python::utils::setSeed);
	def("split", &i6engine::utils::split);

	enum_<i6engine::utils::LogLevel>("LogLevel")
		.value("LOGDEBUG", i6engine::utils::LogLevel::LOGDEBUG)
		.value("LOGINFO", i6engine::utils::LogLevel::LOGINFO)
		.value("LOGWARN", i6engine::utils::LogLevel::LOGWARN)
		.value("LOGERROR", i6engine::utils::LogLevel::LOGERROR)
		.value("LOGFATAL", i6engine::utils::LogLevel::LOGFATAL)
		.value("LOGNONE", i6engine::utils::LogLevel::LOGNONE);

	class_<i6engine::utils::Semaphore, boost::noncopyable>("Semaphore")
		.def(init<>())
		.def("notify", &i6engine::utils::Semaphore::notify)
		.def("wait", &i6engine::utils::Semaphore::wait);

	class_<i6engine::utils::Splash, boost::noncopyable>("Splash")
		.def(init<>())
		.def("showSplash", &i6engine::utils::Splash::showSplash)
		.def("closeSplash", &i6engine::utils::Splash::closeSplash);
}
