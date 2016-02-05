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

#include "i6engine/api/components/LineComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	LineComponent::LineComponent(const int64_t id, const attributeMap & params) : Component(id, params), _startPos(), _endPos(), _colour() {
		Component::_objFamilyID = components::LineComponent;
		Component::_objComponentID = components::LineComponent;

		parseAttribute<true>(params, "startPos", _startPos);
		parseAttribute<false>(params, "endPos", _endPos);
		parseAttribute<false>(params, "colour", _colour);
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
		_startPos.insertInMap("startPos", params);
		_endPos.insertInMap("endPos", params);
		_colour.insertInMap("colour", params);

		return params;
	}

	std::pair<AddStrategy, int64_t> LineComponent::howToAdd(const ComPtr & comp) const {
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
} /* namespace i6engine */
