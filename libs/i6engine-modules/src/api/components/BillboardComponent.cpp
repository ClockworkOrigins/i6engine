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

namespace i6engine {
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

	std::pair<AddStrategy, int64_t> BillboardComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> BillboardComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
