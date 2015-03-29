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

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	BillboardComponent::BillboardComponent(const int64_t id, const attributeMap & params) : Component(id, params), _material(), _width(), _height(), _billboardOrigin() {
		Component::_objFamilyID = components::BillboardComponent;
		Component::_objComponentID = components::BillboardComponent;

		_material = params.find("material")->second;
		_width = boost::lexical_cast<double>(params.find("width")->second);
		_height = boost::lexical_cast<double>(params.find("height")->second);
		_billboardOrigin = graphics::BillboardOrigin(boost::lexical_cast<uint32_t>(params.find("origin")->second));
	}

	BillboardComponent::~BillboardComponent() {
	}

	ComPtr BillboardComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("BillboardComponent", "origin not set!", params.find("origin") != params.end());
		ISIXE_THROW_API_COND("BillboardComponent", "material not set!", params.find("material") != params.end());
		ISIXE_THROW_API_COND("BillboardComponent", "width not set!", params.find("width") != params.end());
		ISIXE_THROW_API_COND("BillboardComponent", "height not set!", params.find("height") != params.end());
		return utils::make_shared<BillboardComponent, Component>(id, params);
	}

	void BillboardComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboardSet, core::Method::Create, new graphics::Graphics_BillboardSet_Create(_objOwnerID, getID(), _material, _width, _height, _billboardOrigin), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void BillboardComponent::Finalize() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboardSet, core::Method::Delete, new graphics::Graphics_BillboardSet_Delete(_objOwnerID, getID()), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void BillboardComponent::createOrUpdateBillboard(const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboard, core::Method::Update, new graphics::Graphics_Billboard_Update(_objOwnerID, getID(), identifier, offset, width, height, u0, v0, u1, v1), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void BillboardComponent::deleteBillboard(const std::string & identifier) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraBillboardRemove, core::Method::Update, new graphics::Graphics_BillboardRemove_Update(_objOwnerID, getID(), identifier), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap BillboardComponent::synchronize() const {
		attributeMap params;
		params.insert(std::make_pair("material", _material));
		params.insert(std::make_pair("width", std::to_string(_width)));
		params.insert(std::make_pair("height", std::to_string(_height)));
		params.insert(std::make_pair("origin", std::to_string(uint32_t(_billboardOrigin))));
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
