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

#include "i6engine/api/components/StaticStateComponent.h"

#include <cassert>

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/make_shared.hpp"

namespace i6e {
namespace api {

	StaticStateComponent::StaticStateComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _rotation(), _scale() {
		Component::_objFamilyID = components::StaticStateComponent;
		Component::_objComponentID = components::StaticStateComponent;

		parseAttribute<true>(params, "pos", _position);
		parseAttribute<true>(params, "rot", _rotation);
		parseAttribute<true>(params, "scale", _scale);
	}

	StaticStateComponent::~StaticStateComponent() {
	}

	void StaticStateComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Create, new graphics::Graphics_Node_Create(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void StaticStateComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Update, new graphics::Graphics_Node_Update(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap StaticStateComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "pos", _position);
		writeAttribute(params, "rot", _rotation);
		writeAttribute(params, "scale", _scale);
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
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Rotation", [this]() {
			std::stringstream ss;
			ss << _rotation;
			return ss.str();
		}, [this](std::string s) {
			_rotation = Quaternion(s);
			setRotation(_rotation);
			return true;
		}, "Quaternion"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Scale", [this]() {
			return _scale.toString();
		}, [this](std::string s) {
			_scale = Vec3(s);
			setScale(_scale);
			return true;
		}, "Vec3"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
