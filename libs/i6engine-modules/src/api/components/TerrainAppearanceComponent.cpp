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

	TerrainAppearanceComponent::TerrainAppearanceComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), _heightmap(), _size(), _inputScale(), _vertices(), _layers(), _minX(), _maxX(), _minY(), _maxY() {
		Component::_objFamilyID = components::TerrainAppearanceComponent;
		Component::_objComponentID = components::TerrainAppearanceComponent;

		parseAttribute<true>(params, "heightmap", _heightmap);
		parseAttribute<true>(params, "size", _size);
		parseAttribute<true>(params, "inputScale", _inputScale);
		parseAttribute<true>(params, "vertices", _vertices);

		uint32_t size;

		parseAttribute<true>(params, "layers", size);
		parseAttribute<true>(params, "minX", _minX);
		parseAttribute<true>(params, "minY", _minY);
		parseAttribute<true>(params, "maxX", _maxX);
		parseAttribute<true>(params, "maxY", _maxY);

		for (uint32_t i = 0; i < size; i++) {
			double layerSize;
			std::string diffuseSpecular;
			std::string normalDisplacement;
			parseAttribute<true>(params, "layer_" + std::to_string(i) + "_size", layerSize);
			parseAttribute<true>(params, "layer_" + std::to_string(i) + "_diffusespecular", diffuseSpecular);
			parseAttribute<true>(params, "layer_" + std::to_string(i) + "_normal", normalDisplacement);

			if (i == 0) { // special case, first layer is always there
				_layers.push_back(std::make_tuple(layerSize, diffuseSpecular, normalDisplacement, 0.0, 0.0));
			} else {
				double minHeight;
				double fadeDist;
				parseAttribute<true>(params, "layer_" + std::to_string(i) + "_minHeight", minHeight);
				parseAttribute<true>(params, "layer_" + std::to_string(i) + "_fadeDist", fadeDist);
				_layers.push_back(std::make_tuple(layerSize, diffuseSpecular, normalDisplacement, minHeight, fadeDist));
			}
		}
	}

	TerrainAppearanceComponent::~TerrainAppearanceComponent() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Delete, new graphics::Graphics_Terrain_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Create, new graphics::Graphics_Terrain_Create(_objOwnerID, getID(), _heightmap, _size, _inputScale, _vertices, _layers, _minX, _minY, _maxX, _maxY), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Update, new graphics::Graphics_Terrain_Update(getID(), _heightmap, _size, _inputScale, _vertices, _layers, _minX, _minY, _maxX, _maxY), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::saveCollisionShape(const std::string & outFile, const std::function<void(void)> & cb) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSaveTerrainShape, core::Method::Create, new graphics::Graphics_SaveTerrainShape_Create(getID(), outFile, cb), core::Subsystem::Object));
	}

	attributeMap TerrainAppearanceComponent::synchronize() const {
		attributeMap params;

		params["heightmap"] = _heightmap;
		params["size"] = boost::lexical_cast<std::string>(_size);
		params["inputScale"] = std::to_string(_inputScale);
		params["vertices"] = std::to_string(_vertices);
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
		}, "String"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Size", [this]() {
			return boost::lexical_cast<std::string>(_size);
		}, [this](std::string s) {
			_size = std::stod(s);
			sendUpdateMessage();
			return true;
		}, "Double"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Input Scale", [this]() {
			return std::to_string(_inputScale);
		}, [this](std::string s) {
			_inputScale = std::stod(s);
			sendUpdateMessage();
			return true;
		}, "Double"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Vertices", [this]() {
			return std::to_string(_vertices);
		}, [this](std::string s) {
			_vertices = std::stoul(s);
			sendUpdateMessage();
			return true;
		}, "Integer"));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
