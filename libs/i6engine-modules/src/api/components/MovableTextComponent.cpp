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

	MovableTextComponent::MovableTextComponent(const int64_t id, const attributeMap & params) : Component(id, params), _font(), _text(), _size(), _colour(), _position(), _autoScaleCallback() {
		Component::_objFamilyID = components::MovableTextComponent;
		Component::_objComponentID = components::MovableTextComponent;

		parseAttribute<true>(params, "font", _font);
		parseAttribute<true>(params, "text", _text);
		parseAttribute<true>(params, "size", _size);
		parseAttribute<true>(params, "colour", _colour);
		parseAttribute<false>(params, "pos", _position);
	}

	MovableTextComponent::~MovableTextComponent() {
	}

	void MovableTextComponent::Init() {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Create, new graphics::Graphics_MovableText_Create(_objOwnerID, getID(), _font, _text, _size, _colour, _position), core::Subsystem::Object));
	}

	void MovableTextComponent::Finalize() {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Delete, new graphics::Graphics_MovableText_Delete(_objOwnerID, getID()), core::Subsystem::Object));
	}

	attributeMap MovableTextComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "font", _font);
		writeAttribute(params, "text", _text);
		writeAttribute(params, "size", _size);
		writeAttribute(params, "colour", _colour);
		writeAttribute(params, "pos", _position);
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
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableText, core::Method::Update, new graphics::Graphics_MovableText_Update(_objOwnerID, getID(), _font, _text, _size, _colour), core::Subsystem::Object));
	}

	void MovableTextComponent::setAutoScaleCallback(const std::function<double(const Vec3 &, const Vec3 &)> & callback) {
		_autoScaleCallback = callback;
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMovableTextAutoScaleCallback, core::Method::Update, new graphics::Graphics_MovableTextAutoScaleCallback_Update(_objOwnerID, getID(), _autoScaleCallback), core::Subsystem::Object));
	}

} /* namespace api */
} /* namespace i6e */
