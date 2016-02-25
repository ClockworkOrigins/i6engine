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

#include "i6engine/modules/graphics/Terrain.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/modules/graphics/GraphicsManager.h"

#include "i6engine/modules/physics/collisionShapes/HeightmapCollisionShape.h"

#include "OGRE/OgreImage.h"
#include "OGRE/Terrain/OgreTerrainGroup.h"

namespace i6engine {
namespace modules {

	Terrain::Terrain(GraphicsManager * manager, const std::string & heightmap, const double size, double inputScale, uint32_t vertices, const std::vector<std::tuple<double, std::string, std::string, double, double>> & layers, int64_t minX, int64_t minY, int64_t maxX, int64_t maxY) : _manager(manager), _mTerrainGroup(), _mTerrainGlobals(), _mTerrainsImported(false), _heightmap(heightmap), _heightdata(), _size(size), _inputScale(inputScale), _vertices(vertices), _layers(layers), _minX(minX), _maxX(maxX), _minY(minY), _maxY(maxY), _dirty(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

		_mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(_manager->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, _vertices, _size);
		_mTerrainGroup->setFilenameConvention(Ogre::String("i6engineTerrain"), Ogre::String("dat"));
		_mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

		configureTerrainDefaults();

		for (int64_t x = minX; x <= maxX; ++x) {
			for (int64_t y = minY; y <= maxY; ++y) {
				defineTerrainHeightmap(x, y);
			}
		}

		_mTerrainGroup->loadAllTerrains(true);

		if (_mTerrainsImported) {
			Ogre::TerrainGroup::TerrainIterator ti = _mTerrainGroup->getTerrainIterator();

			while (ti.hasMoreElements()) {
				Ogre::Terrain * t = ti.getNext()->instance;
				initBlendMaps(t);
			}
		}

		_mTerrainGroup->freeTemporaryResources();
	}

	Terrain::Terrain(GraphicsManager * manager, const std::vector<std::vector<double>> & heightdata, const double size, double inputScale, uint32_t vertices, const std::vector<std::tuple<double, std::string, std::string, double, double>> & layers, int64_t minX, int64_t minY, int64_t maxX, int64_t maxY) : _manager(manager), _mTerrainGroup(), _mTerrainGlobals(), _mTerrainsImported(false), _heightmap(), _heightdata(heightdata), _size(size), _inputScale(inputScale), _vertices(vertices), _layers(layers), _minX(minX), _maxX(maxX), _minY(minY), _maxY(maxY) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		_mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

		_mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(_manager->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, _vertices, _size);
		_mTerrainGroup->setFilenameConvention(Ogre::String("i6engineTerrain"), Ogre::String("dat"));
		_mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

		configureTerrainDefaults();

		for (int64_t x = minX; x <= maxX; ++x) {
			for (int64_t y = minY; y <= maxY; ++y) {
				defineTerrainHeightdata(x, y);
			}
		}

		_mTerrainGroup->loadAllTerrains(true);

		if (_mTerrainsImported) {
			Ogre::TerrainGroup::TerrainIterator ti = _mTerrainGroup->getTerrainIterator();

			while (ti.hasMoreElements()) {
				Ogre::Terrain * t = ti.getNext()->instance;
				initBlendMaps(t);
			}
		}

		_mTerrainGroup->freeTemporaryResources();
	}

	Terrain::~Terrain() {
		ASSERT_THREAD_SAFETY_FUNCTION

		// destroys Terrain
		OGRE_DELETE _mTerrainGroup;
		OGRE_DELETE _mTerrainGlobals;
	}

	void Terrain::configureTerrainDefaults() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mTerrainGlobals->setMaxPixelError(8);
		// testing composite map
		_mTerrainGlobals->setCompositeMapDistance(3000);
		//Ogre::Light * light = _manager->getSceneManager()->getLight("SN_3_5");
		// _mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
		_mTerrainGlobals->setCompositeMapAmbient(_manager->getSceneManager()->getAmbientLight());
		// _mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
		Ogre::Terrain::ImportData & defaultimp = _mTerrainGroup->getDefaultImportSettings();
		defaultimp.terrainSize = _vertices;
		defaultimp.worldSize = _size;
		defaultimp.inputScale = _inputScale;
		defaultimp.minBatchSize = 3;
		defaultimp.maxBatchSize = 65;

		defaultimp.layerList.resize(_layers.size());

		for (size_t i = 0; i < _layers.size(); i++) {
			defaultimp.layerList[i].worldSize = std::get<0>(_layers[i]); // scale factor of one tile
			defaultimp.layerList[i].textureNames.push_back(std::get<1>(_layers[i]));
			defaultimp.layerList[i].textureNames.push_back(std::get<2>(_layers[i]));
		}
	}

	void Terrain::defineTerrainHeightmap(const int64_t x, const int64_t y) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::String filename = _mTerrainGroup->generateFilename(long(x), long(y));

		if (Ogre::ResourceGroupManager::getSingleton().resourceExists(_mTerrainGroup->getResourceGroup(), filename)) {
			_mTerrainGroup->defineTerrain(long(x), long(y));
		} else {
			Ogre::Image img;

			getTerrainImage(x % 2 != 0, y % 2 != 0, img);

			_mTerrainGroup->defineTerrain(long(x), long(y), &img);
			_mTerrainsImported = true;
		}
	}

	void Terrain::defineTerrainHeightdata(const int64_t x, const int64_t y) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::String filename = _mTerrainGroup->generateFilename(long(x), long(y));

		if (Ogre::ResourceGroupManager::getSingleton().resourceExists(_mTerrainGroup->getResourceGroup(), filename)) {
			_mTerrainGroup->defineTerrain(long(x), long(y));
		} else {
			float * heightdata = new float[_heightdata.size() * _heightdata[0].size()];
			for (size_t i = 0; i < _heightdata.size(); i++) {
				for (size_t j = 0; j < _heightdata[i].size(); j++) {
					heightdata[i * _heightdata[i].size() + j] = _heightdata[j][i];
				}
			}
			_mTerrainGroup->defineTerrain(long(x), long(y), heightdata);
			_mTerrainsImported = true;
		}
	}

	void Terrain::getTerrainImage(const bool flipX, const bool flipY, Ogre::Image & img) {
		ASSERT_THREAD_SAFETY_FUNCTION
		img.load(_heightmap, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		if (flipX) {
			img.flipAroundY();
		}
		if (flipY) {
			img.flipAroundX();
		}
	}

	void Terrain::initBlendMaps(Ogre::Terrain * terrain) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (size_t i = 1; i < _layers.size(); i++) {
			Ogre::Real minHeight = std::get<3>(_layers[i]);
			Ogre::Real fadeDist = std::get<4>(_layers[i]);

			Ogre::TerrainLayerBlendMap * blendMap0 = terrain->getLayerBlendMap(Ogre::uint8(i));

			float * pBlend0 = blendMap0->getBlendPointer();

			for (Ogre::uint16 y = 0; y < 1024 /*terrain->getLayerBlendMapSize()*/; ++y) { // FIXME: (Daniel) there is a bug in OGRE 1.9 to getLayerBlendMapSize always return zero... fix with Ogre update
				for (Ogre::uint16 x = 0; x < 1024 /*terrain->getLayerBlendMapSize()*/; ++x) {
					Ogre::Real tx, ty;

					blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
					Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
					Ogre::Real val = (height - minHeight) / fadeDist;
					val = Ogre::Math::Clamp(val, Ogre::Real(0), Ogre::Real(1));
					*pBlend0++ = val;
				}
			}

			blendMap0->dirty();
			blendMap0->update();
		}
	}

	void Terrain::setHeightAtPosition(uint64_t x, uint64_t z, double height) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Terrain * terrain = _mTerrainGroup->getTerrain(0, 0);
		terrain->setHeightAtPoint(x, z, height);
		_dirty = true;
	}

	void Terrain::saveCollisionShape(const std::string & outFile) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Terrain * pTerrain = _mTerrainGroup->getTerrain(0, 0);
		int terrainPageSize = pTerrain->getSize(); // Number of vertices along x/z axe

		std::vector<double> pTerrainHeightDataConvert((terrainPageSize * (std::abs(_maxX - _minX) + 1) - (std::abs(_maxX - _minX))) * (terrainPageSize * (std::abs(_maxY - _minY) + 1) - (std::abs(_maxY - _minY))), 0.0);
		int64_t singleRowLength = terrainPageSize * (std::abs(_maxY - _minY) + 1) - (std::abs(_maxY - _minY));
		for (int64_t i = _minX; i <= _maxX; i++) {
			for (int64_t j = _minY; j <= _maxY; j++) {
				float * pTerrainHeightData = _mTerrainGroup->getTerrain(long(j), long(i))->getHeightData();
				for (int x = (i == _minX) ? 0 : 1; x < terrainPageSize; x++) {
					for (int z = (j == _minY) ? 0 : 1; z < terrainPageSize; z++) {
						int64_t blockRow = (terrainPageSize * (i - _minX) - (i - _minX) + x) * singleRowLength;
						int64_t columnOffset = (j - _minY) * terrainPageSize - (j - _minY) + z;
						pTerrainHeightDataConvert[singleRowLength * ((std::abs(_maxX - _minX) + 1) * terrainPageSize - (std::abs(_maxX - _minX)) - 1) - blockRow + columnOffset] = double(pTerrainHeightData[x * terrainPageSize + z]);
					}
				}
			}
		}

		double unitsBetweenVertices = pTerrain->getWorldSize() / (terrainPageSize - 1);
		Vec3 scaling(unitsBetweenVertices, 1, unitsBetweenVertices);
		HeightmapCollisionShapeData * hcsd = new HeightmapCollisionShapeData(uint32_t(terrainPageSize * (std::abs(_maxX - _minX) + 1) - (std::abs(_maxX - _minX))), uint32_t(terrainPageSize * (std::abs(_maxY - _minY) + 1) - (std::abs(_maxY - _minY))), pTerrainHeightDataConvert, pTerrain->getMinHeight(), pTerrain->getMaxHeight(), scaling);
		std::string serialized = hcsd->Serialize();

		delete hcsd;

		std::ofstream fs(outFile.c_str(), std::ios_base::binary);
		fs << serialized;
		fs.close();
	}

	void Terrain::update() {
		Ogre::Terrain * terrain = _mTerrainGroup->getTerrain(0, 0);
		initBlendMaps(terrain);
		terrain->update();
		_dirty = false;
	}

} /* namespace modules */
} /* namespace i6engine */
