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

#include "i6engine/utils/Exceptions.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreDefaultHardwareBufferManager.h"
#include "OGRE/OgreLogManager.h"
#include "OGRE/OgreRoot.h"

int main(int argc, char ** argv) {
	argc--;
	argv++;

	if (argc != 5) {
		ISIXE_THROW_API("meshToBulletConverter", "Wrong parameters: prefix diffuseTexture normalTexture specularTexture displacementTexture");
	} else {
		std::string prefix = argv[0];
		std::string diffuseTexture = argv[1];
		std::string normalTexture = argv[2];
		std::string specularTexture = argv[3];
		std::string displacementTexture = argv[4];

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

		Ogre::Image combined;

		combined.loadTwoImagesAsRGBA(diffuseTexture, specularTexture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_A8R8G8B8);
		combined.save(prefix + "_diffusespecular.dds");

		combined.loadTwoImagesAsRGBA(normalTexture, displacementTexture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_A8R8G8B8);
		combined.save(prefix + "_normalheight.dds");
		
		delete resourceManager;
		delete dhbm;
		delete root;
		delete lm;
	}

	return 0;
}

