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

#include "TerrainShapeCreatorApplication.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/graphics/ResourceManager.h"
#include "i6engine/modules/graphics/Terrain.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"

#include "OGRE/Ogre.h"
#include "OGRE/OgreDefaultHardwareBufferManager.h"
#include "OGRE/Terrain/OgreTerrainGroup.h"

namespace i6engine {
namespace tools {

	void TerrainShapeCreatorApplication::Initialize() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::GraphicsMessageType, TerrainShapeCreatorApplication::Mailbox, this);
		_manager = new modules::GraphicsManager(nullptr);
	}

	void TerrainShapeCreatorApplication::AfterInitialize() {
		std::cout << "Loading Template '" << _goTemplate << "'" << std::endl;
		api::EngineController::GetSingleton().getObjectFacade()->createObject(_goTemplate, api::objects::GOTemplate(), api::EngineController::GetSingleton().getUUID(), false);
		notifyNewID(0);
	}

	void TerrainShapeCreatorApplication::Mailbox(const api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == api::messages::GraphicsMessageType && msg->getSubtype() == api::graphics::GraTerrain) {
			std::cout << "Creating terrain" << std::endl;

			std::string heightmap = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->heightmap;
			double size = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->size;
			double inputScale = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->inputScale;
			std::vector<std::tuple<double, std::string, std::string, double, double>> layers = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->layers;
			int64_t minX = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->minX;
			int64_t minY = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->minY;
			int64_t maxX = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->maxX;
			int64_t maxY = dynamic_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->maxY;

			modules::Terrain * terrain = new modules::Terrain(_manager, heightmap, size, inputScale, layers, minX, minY, maxX, maxY);

			Ogre::Terrain * pTerrain = terrain->getTerrainGroup()->getTerrain(0, 0);
			int terrainPageSize = pTerrain->getSize(); // Number of vertices along x/z axe

			std::cout << "Generating CollisionShape" << std::endl;

			// >>> We need to mirror the ogre-height-data along the z axis first!
			// This is related to how Ogre and Bullet differ in heightmap storing
			float * pTerrainHeightData = pTerrain->getHeightData();
			float * pTerrainHeightDataConvert = new float[size_t(terrainPageSize * (std::abs(maxX - minX)) * terrainPageSize * (std::abs(maxY - minY)))];
			for (int64_t i = minX; i <= maxX; i++) {
				for (int64_t j = minY; j <= maxY; j++) {
					Ogre::Terrain * pTerrain = terrain->getTerrainGroup()->getTerrain(long(i), long(j));
					for (int k = 0; k < terrainPageSize; ++k) {
						memcpy(pTerrainHeightDataConvert + terrainPageSize * (i - minX) + std::abs(maxX - minX) * (j - minY) * terrainPageSize + terrainPageSize * k, pTerrainHeightData + terrainPageSize * (i - minX) + std::abs(maxX - minX) * (j - minY) * terrainPageSize + terrainPageSize * (terrainPageSize - k - 1), sizeof(float) * (terrainPageSize));
					}
				}
			}
			// <<< End of conversion

			btHeightfieldTerrainShape * pHeightShape = new btHeightfieldTerrainShape(int(terrainPageSize * (std::abs(maxX - minX))), int(terrainPageSize * (std::abs(maxY - minY))), pTerrainHeightDataConvert, 1, /* Terrains getHeightData() is already scaled perfectly */ pTerrain->getMinHeight(), pTerrain->getMaxHeight(), 1, /* upVector is Y positive in ogre-, bullet- and our world */ PHY_FLOAT, true);

			// Scale the mesh along x/z
			float unitsBetweenVertices = pTerrain->getWorldSize() / (terrainPageSize - 1);
			btVector3 scaling(unitsBetweenVertices, 1, unitsBetweenVertices);
			pHeightShape->setLocalScaling(scaling);

			// Ogre uses DiamonSubdivision for Terrain-mesh, so bullet should use it too
			pHeightShape->setUseDiamondSubdivision(true);

			std::cout << "Exporting bullet file" << std::endl;

			btDefaultSerializer * serializer = new btDefaultSerializer();
			serializer->startSerialization();
			pHeightShape->serializeSingleShape(serializer);
			serializer->finishSerialization();

			FILE * file = fopen(_outFile.c_str(), "wb");
			fwrite(serializer->getBufferPointer(), size_t(serializer->getCurrentBufferSize()), 1, file);
			fclose(file);
			std::cout << "Exported '" << _outFile << "'" << std::endl;

			delete serializer;
			delete pHeightShape;
			delete terrain;

			api::EngineController::GetSingleton().stop();
		}
	}

} /* namespace tools */
} /* namespace i6engine */
