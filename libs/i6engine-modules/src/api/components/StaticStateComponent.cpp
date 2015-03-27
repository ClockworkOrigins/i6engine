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

#include "i6engine/api/components/StaticStateComponent.h"

#include <cassert>

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/make_shared.hpp"

namespace i6engine {
namespace api {

	StaticStateComponent::StaticStateComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(Vec3(params, "pos")), _rotation(Quaternion(params, "rot")), _scale(Vec3(params, "scale")) {
		Component::_objFamilyID = components::StaticStateComponent;
		Component::_objComponentID = components::StaticStateComponent;
	}

	StaticStateComponent::~StaticStateComponent() {
	}

	ComPtr StaticStateComponent::createC(const int64_t id, const attributeMap & params) {
		assert(params.find("pos") != params.end());
		assert(params.find("rot") != params.end());
		assert(params.find("scale") != params.end());
		return utils::make_shared<StaticStateComponent, Component>(id, params);
	}

	void StaticStateComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Create, new graphics::Graphics_Node_Create(_objOwnerID, getID(), _position, _rotation, _scale), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void StaticStateComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Update, new graphics::Graphics_Node_Update(_objOwnerID, getID(), _position, _rotation, _scale), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap StaticStateComponent::synchronize() const {
		attributeMap params;
		_position.insertInMap("pos", params);
		_rotation.insertInMap("rot", params);
		_scale.insertInMap("scale", params);

		return params;
	}

	std::vector<componentOptions> StaticStateComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Position", [this]() {
			return _position.toString();
		}, [this](std::string s) {
			_position = Vec3(s);
			setPosition(_position);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Rotation", [this]() {
			std::stringstream ss;
			ss << _rotation;
			return ss.str();
		}, [this](std::string s) {
			_rotation = Quaternion(s);
			setRotation(_rotation);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Scale", [this]() {
			return _scale.toString();
		}, [this](std::string s) {
			_scale = Vec3(s);
			setScale(_scale);
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
