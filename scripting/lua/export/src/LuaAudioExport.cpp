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

#include "LuaAudioExport.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

namespace i6e {
namespace lua {
namespace audio {

	void createNode(int64_t comId, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category) {
		i6e::api::EngineController::GetSingleton().getAudioFacade()->createNode(comId, f, l, m, p, d, cacheable, category);
	}

	void deleteNode(int64_t comId) {
		i6e::api::EngineController::GetSingleton().getAudioFacade()->deleteNode(comId);
	}

	void updateListener(const Vec3 & position, const Quaternion & rotation, const Vec3 & velocity) {
		i6e::api::EngineController::GetSingleton().getAudioFacade()->updateListener(position, rotation, velocity);
	}

	void updatePosition(int64_t comId, const Vec3 & position) {
		i6e::api::EngineController::GetSingleton().getAudioFacade()->updatePosition(comId, position);
	}

	api::audio::SoundHandle playSound(const std::string & file, double maxDistance, const Vec3 & pos, const Vec3 & dir, bool cacheable, const std::string & category) {
		return i6e::api::EngineController::GetSingleton().getAudioFacade()->playSound(file, maxDistance, pos, dir, cacheable, category);
	}

	api::audio::SoundHandle playSoundWithCallbackScript(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category, const std::string & file, const std::string & func) {
		return i6e::api::EngineController::GetSingleton().getAudioFacade()->playSoundWithCallback(f, m, p, d, cacheable, category, [file, func](bool b) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(file, func, b);
		});
	}

	api::audio::SoundHandle playSoundWithCallbackFunction(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category, const std::string & func) {
		return i6e::api::EngineController::GetSingleton().getAudioFacade()->playSoundWithCallback(f, m, p, d, cacheable, category, [func](bool b) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, b);
		});
	}

	void stopSound(api::audio::SoundHandle handle) {
		i6e::api::EngineController::GetSingleton().getAudioFacade()->stopSound(handle);
	}

	void resetAudioSubSystem() {
		i6e::api::EngineController::GetSingleton().getAudioFacade()->resetSubSystem();
	}

} /* namespace audio */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerAudio() {
	return
		def("createNode", &i6e::lua::audio::createNode),
		def("deleteNode", &i6e::lua::audio::deleteNode),
		def("updateListener", &i6e::lua::audio::updateListener),
		def("updatePosition", &i6e::lua::audio::updatePosition),
		def("playSound", &i6e::lua::audio::playSound),
		def("playSoundWithCallback", &i6e::lua::audio::playSoundWithCallbackScript),
		def("playSoundWithCallback", &i6e::lua::audio::playSoundWithCallbackFunction),
		def("stopSound", &i6e::lua::audio::stopSound),
		def("resetAudioSubSystem", &i6e::lua::audio::resetAudioSubSystem)
		;
}
