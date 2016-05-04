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

namespace i6e {
namespace api {

	MovableTextComponent::MovableTextComponent(const int64_t id, const attributeMap & params) : Component(id, params), _font(), _text(), _size(), _colour() {
		Component::_objFamilyID = components::MovableTextComponent;
		Component::_objComponentID = components::MovableTextComponent;

		parseAttribute<true>(params, "font", _font);
		parseAttribute<true>(params, "text", _text);
		parseAttribute<true>(params, "size", _size);
		parseAttribute<true>(params, "colour", _colour);
	}

	MovableTextComponent::~MovableTextComponent() {
	}

	void MovableTextComponent::Init() {
		// check whether a MeshAppearanceComponent exists
		auto c = getOwnerGO()->getGOC(components::ComponentTypes::MeshAppearanceComponent);
		if (c == nullptr) {
			ISIXE_THROW_API_INFO("MovableTextComponent", "MovableTextComponent requires a MeshAppearanceComponent", "Add a MeshAppearanceComponent to the GameObject before the MovableTextComponent is added.");
		} else {
			if (c->getID() > getID()) {
				ISIXE_THROW_API_INFO("MovableTextComponent", "MovableTextComponent requires a MeshAppearanceComponent", "Add a MeshAppearanceComponent to the GameObject before the MovableTextComponent is added.");
			}
		}
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Create, new graphics::Graphics_MovableText_Create(_objOwnerID, getID(), getOwnerGO()->getGOC(components::ComponentTypes::MeshAppearanceComponent)->getID(), _font, _text, _size, _colour), core::Subsystem::Object));
	}

	void MovableTextComponent::Finalize() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Delete, new graphics::Graphics_MovableText_Delete(_objOwnerID, getID()), core::Subsystem::Object));
	}

	attributeMap MovableTextComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "font", _font);
		writeAttribute(params, "text", _text);
		writeAttribute(params, "size", _size);
		writeAttribute(params, "colour", _colour);
		return params;
	}

	std::pair<AddStrategy, int64_t> MovableTextComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> MovableTextComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		return result;
	}

	void MovableTextComponent::setText(const std::string & text) {
		_text = text;
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Update, new graphics::Graphics_MovableText_Update(_objOwnerID, getID(), _font, _text, _size, _colour), core::Subsystem::Object));
	}

} /* namespace api */
} /* namespace i6e */
