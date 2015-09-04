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

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/PhysicsFacade.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace physics {

	void setGravity(const math::i6eVector & vec3Gravity) {
		i6engine::api::EngineController::GetSingleton().getPhysicsFacade()->setGravity(vec3Gravity);
	}

	void resetPhysicsSubSystem() {
		i6engine::api::EngineController::GetSingleton().getPhysicsFacade()->resetSubSystem();
	}

	void pausePhysics() {
		i6engine::api::EngineController::GetSingleton().getPhysicsFacade()->pause();
	}

	void unpausePhysics() {
		i6engine::api::EngineController::GetSingleton().getPhysicsFacade()->unpause();
	}

} /* namespace physics */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingPhysicsPython) {
	using namespace boost::python;
	def("setGravity", &i6engine::python::physics::setGravity);
	def("resetPhysicsSubSystem", &i6engine::python::physics::resetPhysicsSubSystem);
	def("pausePhysics", &i6engine::python::physics::pausePhysics);
	def("unpausePhysics", &i6engine::python::physics::unpausePhysics);
}
