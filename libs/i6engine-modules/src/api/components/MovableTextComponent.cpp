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

#include "i6engine/api/components/MovableTextComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	MovableTextComponent::MovableTextComponent(const int64_t id, const attributeMap & params) : Component(id, params), _font(), _text(), _size(), _colour() {
		Component::_objFamilyID = components::MovableTextComponent;
		Component::_objComponentID = components::MovableTextComponent;

		_font = params.find("font")->second;
		_text = params.find("text")->second;
		_size = std::stoi(params.find("size")->second);
		_colour = Vec3(params.find("colour")->second);
	}

	MovableTextComponent::~MovableTextComponent() {
	}

	ComPtr MovableTextComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("MovableTextComponent", "font not set!", params.find("font") != params.end());
		ISIXE_THROW_API_COND("MovableTextComponent", "text not set!", params.find("text") != params.end());
		ISIXE_THROW_API_COND("MovableTextComponent", "size not set!", params.find("size") != params.end());
		ISIXE_THROW_API_COND("MovableTextComponent", "colour not set!", params.find("colour") != params.end());
		return utils::make_shared<MovableTextComponent, Component>(id, params);
	}

	void MovableTextComponent::Init() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Create, new graphics::Graphics_MovableText_Create(_objOwnerID, getID(), getOwnerGO()->getGOC(components::ComponentTypes::MeshAppearanceComponent)->getID(), _font, _text, _size, _colour), i6engine::core::Subsystem::Object));
	}

	void MovableTextComponent::Finalize() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Delete, new graphics::Graphics_MovableText_Delete(_objOwnerID, getID()), i6engine::core::Subsystem::Object));
	}

	attributeMap MovableTextComponent::synchronize() {
		attributeMap params;
		return params;
	}

	std::pair<AddStrategy, int64_t> MovableTextComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> MovableTextComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		return result;
	}

	void MovableTextComponent::setText(const std::string & text) {
		_text = text;
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Update, new graphics::Graphics_MovableText_Update(_objOwnerID, getID(), _font, _text, _size, _colour), i6engine::core::Subsystem::Object));
	}

} /* namespace api */
} /* namespace i6engine */
