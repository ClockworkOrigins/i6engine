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

#include "i6engine/api/components/LineComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace api {

	LineComponent::LineComponent(const int64_t id, const attributeMap & params) : Component(id, params), _startPos(), _endPos(), _colour() {
		Component::_objFamilyID = components::LineComponent;
		Component::_objComponentID = components::LineComponent;

		parseAttribute<true>(params, "startPos", _startPos);
		parseAttribute<true>(params, "endPos", _endPos);
		parseAttribute<true>(params, "colour", _colour);
	}

	LineComponent::~LineComponent() {
	}

	void LineComponent::Init() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLine, core::Method::Create, new graphics::Graphics_Line_Create(_objOwnerID, getID(), _startPos, _endPos, _colour), core::Subsystem::Object));
	}

	void LineComponent::Finalize() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLine, core::Method::Delete, new graphics::Graphics_Line_Delete(_objOwnerID, getID()), core::Subsystem::Object));
	}

	attributeMap LineComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "startPos", _startPos);
		writeAttribute(params, "endPos", _endPos);
		writeAttribute(params, "colour", _colour);
		return params;
	}

	std::pair<AddStrategy, int64_t> LineComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> LineComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Start Position", [this]() {
			return _startPos.toString();
		}, [this](std::string s) {
			_startPos = Vec3(s);
			// TODO: (Daniel) send Update
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "End Position", [this]() {
			return _endPos.toString();
		}, [this](std::string s) {
			_endPos = Vec3(s);
			// TODO: (Daniel) send Update
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Colour", [this]() {
			return _colour.toString();
		}, [this](std::string s) {
			_colour = Vec3(s);
			return true;
		}, "Vec3"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
