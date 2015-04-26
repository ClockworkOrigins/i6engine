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

	TerrainAppearanceComponent::TerrainAppearanceComponent(const int64_t id, const i6engine::api::attributeMap & params) : Component(id, params), _heightmap(params.at("heightmap")), _size(boost::lexical_cast<double>(params.at("size"))), _inputScale(boost::lexical_cast<double>(params.at("inputScale"))), _layers() {
		Component::_objFamilyID = components::TerrainAppearanceComponent;
		Component::_objComponentID = components::TerrainAppearanceComponent;

		uint32_t size = std::stoul(params.at("layers"));
		_minX = std::stol(params.at("minX"));
		_minY = std::stol(params.at("minY"));
		_maxX = std::stol(params.at("maxX"));
		_maxY = std::stol(params.at("maxY"));

		for (uint32_t i = 0; i < size; i++) {
			ISIXE_THROW_API_COND("TerrainAppearanceComponent", "layer_" << i << "_size not set!", params.find("layer_" + std::to_string(i) + "_size") != params.end());
			ISIXE_THROW_API_COND("TerrainAppearanceComponent", "layer_" << i << "_diffusespecular not set!", params.find("layer_" + std::to_string(i) + "_diffusespecular") != params.end());
			ISIXE_THROW_API_COND("TerrainAppearanceComponent", "layer_" << i << "_normal not set!", params.find("layer_" + std::to_string(i) + "_normal") != params.end());

			if (i == 0) { // special case, first layer is always there
				_layers.push_back(std::make_tuple(boost::lexical_cast<double>(params.at("layer_" + std::to_string(i) + "_size")), params.at("layer_" + std::to_string(i) + "_diffusespecular"), params.at("layer_" + std::to_string(i) + "_normal"), 0.0, 0.0));
			} else {
				ISIXE_THROW_API_COND("TerrainAppearanceComponent", "layer_" << i << "_minHeight not set!", params.find("layer_" + std::to_string(i) + "_minHeight") != params.end());
				ISIXE_THROW_API_COND("TerrainAppearanceComponent", "layer_" << i << "_fadeDist not set!", params.find("layer_" + std::to_string(i) + "_fadeDist") != params.end());
				_layers.push_back(std::make_tuple(boost::lexical_cast<double>(params.at("layer_" + std::to_string(i) + "_size")), params.at("layer_" + std::to_string(i) + "_diffusespecular"), params.at("layer_" + std::to_string(i) + "_normal"), boost::lexical_cast<double>(params.at("layer_" + std::to_string(i) + "_minHeight")), boost::lexical_cast<double>(params.at("layer_" + std::to_string(i) + "_fadeDist"))));
			}
		}
	}

	TerrainAppearanceComponent::~TerrainAppearanceComponent() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Delete, new graphics::Graphics_Terrain_Delete(_objOwnerID, getID()), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	ComPtr TerrainAppearanceComponent::createC(const int64_t id, const i6engine::api::attributeMap & params) {
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "heightmap not set!", params.find("heightmap") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "size not set!", params.find("size") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "inputScale not set!", params.find("inputScale") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "layers not set!", params.find("layers") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "minX not set!", params.find("minX") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "minY not set!", params.find("minY") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "maxX not set!", params.find("maxX") != params.end());
		ISIXE_THROW_API_COND("TerrainAppearanceComponent", "maxY not set!", params.find("maxY") != params.end());
		return utils::make_shared<TerrainAppearanceComponent, Component>(id, params);
	}

	void TerrainAppearanceComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Create, new graphics::Graphics_Terrain_Create(_objOwnerID, getID(), _heightmap, _size, _inputScale, _layers, _minX, _minY, _maxX, _maxY), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Update, new graphics::Graphics_Terrain_Update(getID(), _heightmap, _size, _inputScale, _layers, _minX, _minY, _maxX, _maxY), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap TerrainAppearanceComponent::synchronize() const {
		attributeMap params;

		params["heightmap"] = _heightmap;
		params["size"] = boost::lexical_cast<std::string>(_size);
		params["inputScale"] = std::to_string(_inputScale);
		params["layers"] = std::to_string(_layers.size());
		params["minX"] = std::to_string(_minX);
		params["minY"] = std::to_string(_minY);
		params["maxX"] = std::to_string(_maxX);
		params["maxY"] = std::to_string(_maxY);

		for (size_t i = 0; i < _layers.size(); i++) {
			params["layer_" + std::to_string(i) + "_size"] = std::to_string(std::get<0>(_layers[i]));
			params["layer_" + std::to_string(i) + "_diffusespecular"] = std::get<1>(_layers[i]);
			params["layer_" + std::to_string(i) + "_normal"] = std::get<2>(_layers[i]);
		}

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
		result.push_back(std::make_tuple(AccessState::READWRITE, "Size", [this]() {
			return boost::lexical_cast<std::string>(_size);
		}, [this](std::string s) {
			_size = boost::lexical_cast<double>(s);
			sendUpdateMessage();
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Input Scale", [this]() {
			return std::to_string(_inputScale);
		}, [this](std::string s) {
			_inputScale = boost::lexical_cast<double>(s);
			sendUpdateMessage();
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
