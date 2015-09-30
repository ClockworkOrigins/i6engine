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

#include "i6engine/api/components/SpawnpointComponent.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	SpawnpointComponent::SpawnpointComponent(const int64_t id, const attributeMap & params) : Component(id, params), _spawntypes(), _state(true) {
		Component::_objFamilyID = components::SpawnpointComponent;
		Component::_objComponentID = components::SpawnpointComponent;

		std::string spawnTypes;
		parseAttribute<true>(params, "spawntypes", spawnTypes);
		addSpawntypes(spawnTypes);
	}

	SpawnpointComponent::~SpawnpointComponent() {
		_spawntypes.clear();
	}

	void SpawnpointComponent::Init() {
	}

	void SpawnpointComponent::addSpawntypes(const std::string & types) {
		std::vector<std::string> v = utils::split(types, ";");

		for (std::string & s : v) {
			addSpawntype(s);
		}
	}

	void SpawnpointComponent::removeSpawntype(const std::string & type) {
		for (size_t i = 0; i < _spawntypes.size(); ++i) {
			if (_spawntypes[i] == type) {
				_spawntypes.erase(_spawntypes.begin() + int(i));
				break;
			}
		}
	}

	bool SpawnpointComponent::containsSpawntype(const std::string & type) const {
		for (size_t i = 0; i < _spawntypes.size(); ++i) {
			if (_spawntypes[i] == type) {
				return true;
			}
		}

		return false;
	}

	attributeMap SpawnpointComponent::synchronize() const {
		attributeMap params;

		std::string types;

		for (const std::string & s : _spawntypes) {
			types += s + ";";
		}

		params["spawntypes"] = types;

		return params;
	}

	void SpawnpointComponent::setState(bool b) {
		_state = b;

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComSpawnpoint, core::Method::Update, new components::Component_Spawnpoint_Update(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->registerTimer(2000000, [msg]() {
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			return false;
		}, false, core::JobPriorities::Prio_Low);
	}

	void SpawnpointComponent::News(const GameMessage::Ptr & msg) {
		uint16_t type = msg->getSubtype();

		if (type == api::components::ComSpawnpoint) {
			setState(true);
		} else {
			ISIXE_THROW_FAILURE("SpawnpointComponent", "Unknown message type");
		}
	}

	std::vector<componentOptions> SpawnpointComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Spawntypes", [this]() {
			std::string types;

			for (std::string & s : _spawntypes) {
				types += s + ";";
			}
			return types;
		}, [this](std::string s) {
			_spawntypes.clear();
			addSpawntypes(s);
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
