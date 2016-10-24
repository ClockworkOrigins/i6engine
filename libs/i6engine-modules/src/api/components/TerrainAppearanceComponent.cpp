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

#include "i6engine/api/components/TerrainAppearanceComponent.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "boost/make_shared.hpp"

namespace i6e {
namespace api {

	TerrainAppearanceComponent::TerrainAppearanceComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), _heightmap(), _heightdata(), _size(), _inputScale(), _vertices(), _layers(), _minX(), _maxX(), _minY(), _maxY() {
		Component::_objFamilyID = components::TerrainAppearanceComponent;
		Component::_objComponentID = components::TerrainAppearanceComponent;

		try {
			parseAttribute<true>(params, "heightmap", _heightmap);
			parseAttribute<false>(params, "heightdata", _heightdata);
			if (!_heightdata.empty()) {
				_heightmap.clear();
			}
		} catch (utils::exceptions::i6exception) {
			parseAttribute<true>(params, "heightdata", _heightdata);
		}
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
	}

	void TerrainAppearanceComponent::Init() {
		GameMessage::Ptr msg;
		
		if (!_heightmap.empty()) {
			msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrainHeightmap, core::Method::Create, new graphics::Graphics_TerrainHeightmap_Create(getID(), _heightmap, _size, _inputScale, _vertices, _layers, _minX, _minY, _maxX, _maxY), core::Subsystem::Object);
		} else {
			msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrainHeightdata, core::Method::Create, new graphics::Graphics_TerrainHeightdata_Create(getID(), _heightdata, _size, _inputScale, _vertices, _layers, _minX, _minY, _maxX, _maxY), core::Subsystem::Object);
		}

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::Finalize() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrain, core::Method::Delete, new graphics::Graphics_Terrain_Delete(getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg;
		
		if (!_heightmap.empty()) {
			msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrainHeightmap, core::Method::Update, new graphics::Graphics_TerrainHeightmap_Update(getID(), _heightmap, _size, _inputScale, _vertices, _layers, _minX, _minY, _maxX, _maxY), core::Subsystem::Object);
		} else {
			msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrainHeightdata, core::Method::Update, new graphics::Graphics_TerrainHeightdata_Update(getID(), _heightdata, _size, _inputScale, _vertices, _layers, _minX, _minY, _maxX, _maxY), core::Subsystem::Object);
		}

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void TerrainAppearanceComponent::setHeightAtPosition(size_t x, size_t z, double height) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraTerrainSetHeight, core::Method::Update, new graphics::Graphics_TerrainSetHeight_Update(getID(), x, z, height), core::Subsystem::Object));
		_heightdata[size_t(x)][size_t(z)] = height;
	}

	void TerrainAppearanceComponent::setHeightAtPosition(const Vec3 & pos, double height) {
		size_t x, z;
		convertPositionToVertex(pos, x, z);
		setHeightAtPosition(x, z, height);
	}

	double TerrainAppearanceComponent::getHeightAtPosition(size_t x, size_t z) const {
		return _heightdata[x][z];
	}

	double TerrainAppearanceComponent::getHeightAtPosition(const Vec3 & pos) const {
		size_t x, z;
		convertPositionToVertex(pos, x, z);
		return getHeightAtPosition(x, z);
	}

	void TerrainAppearanceComponent::saveCollisionShape(const std::string & outFile, const std::function<void(void)> & cb) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSaveTerrainShape, core::Method::Create, new graphics::Graphics_SaveTerrainShape_Create(getID(), outFile, cb), core::Subsystem::Object));
	}

	attributeMap TerrainAppearanceComponent::synchronize() const {
		attributeMap params;

		if (!_heightmap.empty()) {
			writeAttribute(params, "heightmap", _heightmap);
		} else {
			writeAttribute(params, "heightdata", _heightdata);
		}
		writeAttribute(params, "size", _size);
		writeAttribute(params, "inputScale", _inputScale);
		writeAttribute(params, "vertices", _vertices);
		writeAttribute(params, "layers", _layers.size());
		writeAttribute(params, "minX", _minX);
		writeAttribute(params, "minY", _minY);
		writeAttribute(params, "maxX", _maxX);
		writeAttribute(params, "maxY", _maxY);

		for (size_t i = 0; i < _layers.size(); i++) {
			writeAttribute(params, "layer_" + std::to_string(i) + "_size", std::get<0>(_layers[i]));
			writeAttribute(params, "layer_" + std::to_string(i) + "_diffusespecular", std::get<1>(_layers[i]));
			writeAttribute(params, "layer_" + std::to_string(i) + "_normal", std::get<2>(_layers[i]));
			if (i > 0) {
				writeAttribute(params, "layer_" + std::to_string(i) + "_minHeight", std::get<3>(_layers[i]));
				writeAttribute(params, "layer_" + std::to_string(i) + "_fadeDist", std::get<4>(_layers[i]));
			}
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
			return std::to_string(_size);
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
			_vertices = uint16_t(std::stoul(s));
			sendUpdateMessage();
			return true;
		}, "Integer"));

		return result;
	}

	void TerrainAppearanceComponent::convertPositionToVertex(const Vec3 & pos, size_t & x, size_t & z) const {
		double edgeLength = _size / _vertices;
		x = size_t((pos.getX() + _size * 0.5 + edgeLength * 0.5) / edgeLength - 0.5);
		z = _vertices - size_t((pos.getZ() + _size * 0.5 + edgeLength * 0.5) / edgeLength + 0.5);
	}

} /* namespace api */
} /* namespace i6e */
