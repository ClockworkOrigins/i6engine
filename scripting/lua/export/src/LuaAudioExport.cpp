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

#include "LuaAudioExport.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

namespace i6engine {
namespace lua {
namespace audio {

	void createNode(int64_t comId, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d, bool cacheable) {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->createNode(comId, f, l, m, p, d, cacheable);
	}

	void deleteNode(int64_t comId) {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->deleteNode(comId);
	}

	void updateListener(const Vec3 & position, const Quaternion & rotation, const Vec3 & velocity) {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->updateListener(position, rotation, velocity);
	}

	void updatePosition(int64_t comId, const Vec3 & position) {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->updatePosition(comId, position);
	}

	uint64_t playSound(const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable) {
		return i6engine::api::EngineController::GetSingleton().getAudioFacade()->playSound(file, maxDistance, pos, dir, cacheable);
	}

	uint64_t playSoundWithCallbackScript(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & file, const std::string & func) {
		return i6engine::api::EngineController::GetSingleton().getAudioFacade()->playSoundWithCallback(f, m, p, d, cacheable, [file, func](bool b) {
			i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(file, func, b);
		});
	}

	uint64_t playSoundWithCallbackFunction(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & func) {
		return i6engine::api::EngineController::GetSingleton().getAudioFacade()->playSoundWithCallback(f, m, p, d, cacheable, [func](bool b) {
			i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, b);
		});
	}

	void stopSound(uint64_t handle) {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->stopSound(handle);
	}

	void resetAudioSubSystem() {
		i6engine::api::EngineController::GetSingleton().getAudioFacade()->resetSubSystem();
	}

} /* namespace audio */
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerAudio() {
	return
		def("createNode", &i6engine::lua::audio::createNode),
		def("deleteNode", &i6engine::lua::audio::deleteNode),
		def("updateListener", &i6engine::lua::audio::updateListener),
		def("updatePosition", &i6engine::lua::audio::updatePosition),
		def("playSound", &i6engine::lua::audio::playSound),
		def("playSoundWithCallback", &i6engine::lua::audio::playSoundWithCallbackScript),
		def("playSoundWithCallback", &i6engine::lua::audio::playSoundWithCallbackFunction),
		def("stopSound", &i6engine::lua::audio::stopSound),
		def("resetAudioSubSystem", &i6engine::lua::audio::resetAudioSubSystem)
		;
}
