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
