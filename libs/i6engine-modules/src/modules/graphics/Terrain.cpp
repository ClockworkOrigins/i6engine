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

#include "OGRE/OgreImage.h"
#include "OGRE/Terrain/OgreTerrainGroup.h"

namespace i6engine {
namespace modules {

	Terrain::Terrain(GraphicsManager * manager, const std::string & heightmap, const std::string & texture, const double size) : _manager(manager), _mTerrainGroup(), _mTerrainGlobals(), _mTerrainsImported(false), _heightmap(heightmap), _texture(texture), _size(size) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		_mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

		_mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(_manager->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, 65, _size);
		_mTerrainGroup->setFilenameConvention(Ogre::String("i6engineTerrain"), Ogre::String("dat"));
		_mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

		configureTerrainDefaults();

		for (int64_t x = 0; x <= 0; ++x) {
			for (int64_t y = 0; y <= 0; ++y) {
				defineTerrain(x, y);
			}
		}

		_mTerrainGroup->loadAllTerrains(true);
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
		 // _mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
		 _mTerrainGlobals->setCompositeMapAmbient(_manager->getSceneManager()->getAmbientLight());
		 // _mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
		 Ogre::Terrain::ImportData & defaultimp = _mTerrainGroup->getDefaultImportSettings();
		 defaultimp.terrainSize = 65;
		 defaultimp.worldSize = _size;
		 defaultimp.inputScale = 1.0; // kein unterschied bei 6000 erkennbar die terrain.png is 8 bpp
		 defaultimp.minBatchSize = 33;
		 defaultimp.maxBatchSize = 65;

		 defaultimp.layerList.resize(1);
		 defaultimp.layerList[0].worldSize = 20; // scale factor of one tile
		 defaultimp.layerList[0].textureNames.push_back(_texture);
	}

	void Terrain::defineTerrain(const int64_t x, const int64_t y) {
		ASSERT_THREAD_SAFETY_FUNCTION

		 Ogre::String filename = _mTerrainGroup->generateFilename(long(x), long(y));

		 if (Ogre::ResourceGroupManager::getSingleton().resourceExists(_mTerrainGroup->getResourceGroup(), filename)) {
			 _mTerrainGroup->defineTerrain(long(x), long(y));
		 } else {
			 Ogre::Image img;

			 getTerrainImage(x % 2 != 0, y % 2 != 0, img);

			 _mTerrainGroup->defineTerrain(0, 0, &img);
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

} /* namespace modules */
} /* namespace i6engine */
