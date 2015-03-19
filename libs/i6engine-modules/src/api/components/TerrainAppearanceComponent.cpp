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

#include "i6engine/api/components/TerrainAppearanceComponent.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/lexical_cast.hpp"
#include "boost/make_shared.hpp"

namespace i6engine {
namespace api {

	TerrainAppearanceComponent::TerrainAppearanceComponent(const int64_t id, const i6engine::api::attributeMap & params) : Component(id, params), _heightmap(params.at("heightmap")), _texture(params.at("texture")), _size(boost::lexical_cast<double>(params.at("size"))) {
		Component::_objFamilyID = components::TerrainAppearanceComponent;
		Component::_objComponentID = components::TerrainAppearanceComponent;
	}

	TerrainAppearanceComponent::~TerrainAppearanceComponent() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Delete, new graphics::Graphics_Terrain_Delete(_objOwnerID, getID()), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	ComPtr TerrainAppearanceComponent::createC(const int64_t id, const i6engine::api::attributeMap & params) {
		return utils::make_shared<TerrainAppearanceComponent, Component>(id, params);
	}

	void TerrainAppearanceComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Create, new graphics::Graphics_Terrain_Create(_objOwnerID, getID(), _heightmap, _texture, _size), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Update, new graphics::Graphics_Terrain_Update(getID(), _heightmap, _texture, _size), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap TerrainAppearanceComponent::synchronize() {
		attributeMap params;

		params["heightmap"] = _heightmap;
		params["texture"] = _texture;
		params["size"] = boost::lexical_cast<std::string>(_size);

		return params;
	}

	std::vector<componentOptions> TerrainAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Heightmap", [this]() {
			return _heightmap;
		}, [this](std::string s) {
			_heightmap = s;
			sendUpdateMessage();
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Texture", [this]() {
			return _texture;
		}, [this](std::string s) {
			_texture = s;
			sendUpdateMessage();
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Size", [this]() {
			return boost::lexical_cast<std::string>(_size);
		}, [this](std::string s) {
			_size = boost::lexical_cast<double>(s);
			sendUpdateMessage();
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
