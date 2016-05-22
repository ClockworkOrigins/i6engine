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

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/PhysicsFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
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
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingPhysicsPython) {
	using namespace boost::python;
	def("setGravity", &i6e::python::physics::setGravity);
	def("resetPhysicsSubSystem", &i6e::python::physics::resetPhysicsSubSystem);
	def("pausePhysics", &i6e::python::physics::pausePhysics);
	def("unpausePhysics", &i6e::python::physics::unpausePhysics);
}
