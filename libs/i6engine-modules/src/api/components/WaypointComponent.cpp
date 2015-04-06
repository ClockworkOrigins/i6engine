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
#include "i6engine/utils/i6eString.h"

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
namespace api {

	WaypointComponent::WaypointComponent(const int64_t id, const i6engine::api::attributeMap & params) : Component(id, params), _name(params.at("name")), _connections() {
		Component::_objFamilyID = components::WaypointComponent;
		Component::_objComponentID = components::WaypointComponent;

		std::string connections = params.find("connections")->second;
		_connections = utils::split(connections, ";");
	}

	WaypointComponent::~WaypointComponent() {
	}

	ComPtr WaypointComponent::createC(const int64_t id, const i6engine::api::attributeMap & params) {
		ISIXE_THROW_API_COND("WaypointComponent", "name not set!", params.find("name") != params.end());
		ISIXE_THROW_API_COND("WaypointComponent", "connections not set!", params.find("connections") != params.end());
		return utils::make_shared<WaypointComponent, Component>(id, params);
	}

	void WaypointComponent::Init() {
	}

	attributeMap WaypointComponent::synchronize() const {
		attributeMap params;

		params["name"] = _name;

		std::string connections;

		for (size_t i = 0; i < _connections.size(); i++) {
			connections += _connections[i];
			if (i < _connections.size() - 1) {
				connections += ";";
			}
		}

		params["connections"] = connections;

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
		result.push_back(std::make_tuple(AccessState::READWRITE, "Connections", [this]() {
			std::string connections;

			for (size_t i = 0; i < _connections.size(); i++) {
				connections += _connections[i];
				if (i < _connections.size() - 1) {
					connections += ";";
				}
			}
			return connections;
		}, [this](std::string s) {
			_connections = utils::split(s, ";");
			return true;
		}));

		return result;
	}

	bool WaypointComponent::isConnected(const std::string & name) const {
		for (auto s : _connections) {
			if (s == name) {
				return true;
			}
		}
		return false;
	}

	void WaypointComponent::addConnection(const std::string & name) {
		_connections.push_back(name);
	}

	void WaypointComponent::removeConnection(const std::string & name) {
		for (size_t i = 0; i < _connections.size(); i++) {
			if (_connections[i] == name) {
				_connections.erase(_connections.begin() + i);
				break;
			}
		}
	}

} /* namespace api */
} /* namespace i6engine */
