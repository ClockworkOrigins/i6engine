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

#include "i6engine/api/components/BillboardComponent.h"

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

	BillboardComponent::BillboardComponent(const int64_t id, const attributeMap & params) : Component(id, params), _material(), _width(), _height(), _billboardOrigin() {
		_objFamilyID = components::BillboardComponent;
		_objComponentID = components::BillboardComponent;

		parseAttribute<true>(params, "material", _material);
		parseAttribute<true>(params, "width", _width);
		parseAttribute<true>(params, "height", _height);
		parseAttribute<true>(params, "origin", _billboardOrigin);
	}

	BillboardComponent::~BillboardComponent() {
	}

	void BillboardComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboardSet, core::Method::Create, new graphics::Graphics_BillboardSet_Create(_objOwnerID, getID(), _material, _width, _height, _billboardOrigin), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void BillboardComponent::Finalize() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboardSet, core::Method::Delete, new graphics::Graphics_BillboardSet_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void BillboardComponent::createOrUpdateBillboard(const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboard, core::Method::Update, new graphics::Graphics_Billboard_Update(_objOwnerID, getID(), identifier, offset, width, height, u0, v0, u1, v1), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void BillboardComponent::deleteBillboard(const std::string & identifier) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboardRemove, core::Method::Update, new graphics::Graphics_BillboardRemove_Update(_objOwnerID, getID(), identifier), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap BillboardComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "material", _material);
		writeAttribute(params, "width", _width);
		writeAttribute(params, "height", _height);
		writeAttribute(params, "origin", _billboardOrigin);
		return params;
	}

	std::pair<AddStrategy, int64_t> BillboardComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> BillboardComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		return result;
	}

} /* namespace api */
} /* namespace i6e */
