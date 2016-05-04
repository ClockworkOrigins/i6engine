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

#include "LuaPhysicsExport.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/PhysicsFacade.h"

namespace i6e {
namespace lua {
namespace physics {

	void setGravity(const math::i6eVector & vec3Gravity) {
		i6e::api::EngineController::GetSingleton().getPhysicsFacade()->setGravity(vec3Gravity);
	}

	void resetPhysicsSubSystem() {
		i6e::api::EngineController::GetSingleton().getPhysicsFacade()->resetSubSystem();
	}

	void pausePhysics() {
		i6e::api::EngineController::GetSingleton().getPhysicsFacade()->pause();
	}

	void unpausePhysics() {
		i6e::api::EngineController::GetSingleton().getPhysicsFacade()->unpause();
	}

} /* namespace physics */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerPhysics() {
	return
		def("setGravity", &i6e::lua::physics::setGravity),
		def("resetPhysicsSubSystem", &i6e::lua::physics::resetPhysicsSubSystem),
		def("pausePhysics", &i6e::lua::physics::pausePhysics),
		def("unpausePhysics", &i6e::lua::physics::unpausePhysics)
		;
}
