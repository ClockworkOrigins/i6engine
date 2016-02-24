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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	WaypointComponent::WaypointComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), _name(), _connections() {
		Component::_objFamilyID = components::WaypointComponent;
		Component::_objComponentID = components::WaypointComponent;

		std::string connections;
		parseAttribute<true>(params, "name", _name);
		parseAttribute<true>(params, "connections", connections);
		_connections = utils::split(connections, ";");
	}

	WaypointComponent::~WaypointComponent() {
	}

	void WaypointComponent::Init() {
	}

	void WaypointComponent::Finalize() {
		for (std::string connection : _connections) {
			for (auto & go : EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Waypoint")) {
				auto wp = go->getGOC<WaypointComponent>(components::ComponentTypes::WaypointComponent);
				if (wp->getName() == connection) {
					wp->removeConnection(_name);
				}
			}
		}
	}

	attributeMap WaypointComponent::synchronize() const {
		attributeMap params;

		std::string connections;
		for (size_t i = 0; i < _connections.size(); i++) {
			connections += _connections[i];
			if (i < _connections.size() - 1) {
				connections += ";";
			}
		}
		writeAttribute(params, "name", _name);
		writeAttribute(params, "connections", connections);

		return params;
	}

	std::vector<componentOptions> WaypointComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Name", [this]() {
			return _name;
		}, [this](std::string s) {
			_name = s;
			return true;
		}, "String"));
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
		}, "String"));

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
				_connections.erase(_connections.begin() + int(i));
				break;
			}
		}
	}

} /* namespace api */
} /* namespace i6engine */
