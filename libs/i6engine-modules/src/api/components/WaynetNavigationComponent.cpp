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

#include "i6engine/api/components/WaynetNavigationComponent.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/manager/WaynetManager.h"

namespace i6e {
namespace api {

	WaynetNavigationComponent::WaynetNavigationComponent(const int64_t id, const api::attributeMap & params) : NavigationComponent(id, params) {
		Component::_objComponentID = components::WaynetNavigationComponent;
	}

	WaynetNavigationComponent::~WaynetNavigationComponent() {
	}
	
	std::vector<Vec3> WaynetNavigationComponent::getPath(const Vec3 & from, const Vec3 & to) const {
		return EngineController::GetSingleton().getWaynetManager()->getShortestPath(from, EngineController::GetSingleton().getWaynetManager()->getNearestWaypoint(to));
	}

	std::vector<Vec3> WaynetNavigationComponent::getPath(const Vec3 & from, const std::string & to) const {
		return EngineController::GetSingleton().getWaynetManager()->getShortestPath(from, to);
	}

} /* namespace api */
} /* namespace i6e */
