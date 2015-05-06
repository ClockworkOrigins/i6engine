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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/AudioFacade.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace audio {

	void playSound(const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable) {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->playSound(file, maxDistance, pos, dir, cacheable);
	}

} /* namespace audio */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingAudioPython) {
	using namespace boost::python;

	def("playSound", &i6engine::python::audio::playSound);
}