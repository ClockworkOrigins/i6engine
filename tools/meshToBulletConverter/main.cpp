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

#include <thread>

#include "MeshStrider.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "i6engine/modules/physics/collisionShapes/MeshStriderCollisionShape.h"

#include "btBulletDynamicsCommon.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/Ogre.h"
#include "OGRE/OgreDefaultHardwareBufferManager.h"

int main(int argc, char ** argv) {
	argc--;
	argv++;

	if (argc != 2) {
		ISIXE_THROW_API("meshToBulletConverter", "Wrong parameters: in.mesh out.bullet");
	} else {
		std::string mesh = argv[0];
		std::string bullet = argv[1];

		std::cout << "Initializing Ogre" << std::endl;
		std::string ogrePath;
		// GRAPHIC is correct, because we want the same variable as for ogre
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load("i6engine.ini");
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue<std::string>("GRAPHIC", "ogreConfigsPath", ogrePath)) {
			ISIXE_THROW_API("meshToBulletConverter", "An exception has occurred: value ogreConfigsPath in section GRAPHIC not found!");
			return 1;
		}
		Ogre::LogManager * lm = new Ogre::LogManager();
		lm->createLog("ogre.log", true, false, false);
		Ogre::Root * root = new Ogre::Root("", ogrePath + "/ogre.cfg", "");
		Ogre::DefaultHardwareBufferManager * dhbm = new Ogre::DefaultHardwareBufferManager();
		i6engine::modules::ResourceManager * resourceManager = new i6engine::modules::ResourceManager(ogrePath);

		std::cout << "Loading mesh '" << mesh << "'" << std::endl;
		Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().load(mesh, "i6engine");
		Ogre::Mesh * mp = meshPtr.get();

		std::cout << "Creating collision shape" << std::endl;
		btCollisionShape * fallShape = new btBvhTriangleMeshShape(new i6engine::tools::MeshStrider(mp), true);

		btDefaultSerializer * serializer = new btDefaultSerializer();
		serializer->startSerialization();
		fallShape->serializeSingleShape(serializer);
		serializer->finishSerialization();

		std::vector<char> data(serializer->getCurrentBufferSize());

		for (size_t i = 0; i < data.size(); i++) {
			data[i] = serializer->getBufferPointer()[i];
		}

		i6engine::modules::MeshStriderCollisionShapeData * mscsd = new i6engine::modules::MeshStriderCollisionShapeData(data);

		std::string serialized = mscsd->Serialize();

		delete mscsd;

		std::ofstream fs(bullet.c_str(), std::ios_base::binary);
		fs << serialized;
		fs.close();
		std::cout << "Exported '" << bullet << "'" << std::endl;
		
		delete serializer;
		delete fallShape;
		delete resourceManager;
		delete dhbm;
		delete root;
		delete lm;
	}

	return 0;
}

