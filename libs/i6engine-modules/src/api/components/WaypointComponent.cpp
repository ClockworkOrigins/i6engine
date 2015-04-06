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

#include "i6engine/api/components/WaypointComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
namespace api {

	WaypointComponent::WaypointComponent(const int64_t id, const i6engine::api::attributeMap & params) : Component(id, params), _name(params.at("name")), _connections() {
		Component::_objFamilyID = components::WaypointComponent;
		Component::_objComponentID = components::WaypointComponent;

		uint32_t numConnections = std::stoul(params.find("numConnections")->second);

		for (uint32_t i = 0; i < numConnections; i++) {
			ISIXE_THROW_API_COND("WaypointComponent", "connection_" + std::to_string(i) + " not set!", params.find("connection_" + std::to_string(i)) != params.end());
			_connections.push_back(params.find("connection_" + std::to_string(i))->second);
		}
	}

	WaypointComponent::~WaypointComponent() {
	}

	ComPtr WaypointComponent::createC(const int64_t id, const i6engine::api::attributeMap & params) {
		ISIXE_THROW_API_COND("WaypointComponent", "name not set!", params.find("name") != params.end());
		ISIXE_THROW_API_COND("WaypointComponent", "numConnections not set!", params.find("numConnections") != params.end());
		return utils::make_shared<WaypointComponent, Component>(id, params);
	}

	void WaypointComponent::Init() {
	}

	attributeMap WaypointComponent::synchronize() const {
		attributeMap params;

		params["name"] = _name;
		params["numConnections"] = std::to_string(_connections.size());

		for (size_t i = 0; i < _connections.size(); i++) {
			params["connection_" + std::to_string(i)] = _connections[i];
		}

		return params;
	}

	std::vector<componentOptions> WaypointComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Name", [this]() {
			return _name;
		}, [this](std::string s) {
			_name = s;
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
