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

#include "clockUtils/argParser/ArgumentParser.h"
#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreDefaultHardwareBufferManager.h"
#include "OGRE/OgreLogManager.h"
#include "OGRE/OgreRoot.h"

int main(int argc, char ** argv) {
	REGISTER_VARIABLE(std::string, prefix, "", "Prefix of the newly created textures");
	REGISTER_VARIABLE(std::string, diffuseTexture, "", "Diffuse map to be used");
	REGISTER_VARIABLE(std::string, normalTexture, "", "Normal map to be used");
	REGISTER_VARIABLE(std::string, specularTexture, "", "Specular map to be used");
	REGISTER_VARIABLE(std::string, displacementTexture, "", "Displacement map to be used");

	if (PARSE_COMMANDLINE() != clockUtils::ClockError::SUCCESS) {
		std::cerr << GETLASTPARSERERROR() << std::endl;
		return 1;
	} else if (HELPSET()) {
		std::cout << GETHELPTEXT() << std::endl;
	} else if (!prefix.isSet()) {
		std::cerr << "Parameter 'prefix' not set!" << std::endl;
		return 1;
	} else if (!diffuseTexture.isSet()) {
		std::cerr << "Parameter 'diffuseTexture' not set!" << std::endl;
		return 1;
	} else if (!normalTexture.isSet()) {
		std::cerr << "Parameter 'normalTexture' not set!" << std::endl;
		return 1;
	} else if (!specularTexture.isSet()) {
		std::cerr << "Parameter 'specularTexture' not set!" << std::endl;
		return 1;
	} else if (!displacementTexture.isSet()) {
		std::cerr << "Parameter 'displacementTexture' not set!" << std::endl;
		return 1;
	} else {
		std::string ogrePath;
		// GRAPHIC is correct, because we want the same variable as for ogre
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load("i6engine.ini");
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue("GRAPHIC", "ogreConfigsPath", ogrePath)) {
			std::cerr << "An exception has occurred: value ogreConfigsPath in section GRAPHIC not found!" << std::endl;
			return 1;
		}
		Ogre::LogManager * lm = new Ogre::LogManager();
		lm->createLog("ogre.log", true, false, false);
		Ogre::Root * root = new Ogre::Root("", ogrePath + "/ogre.cfg", "");
		Ogre::DefaultHardwareBufferManager * dhbm = new Ogre::DefaultHardwareBufferManager();
		i6e::modules::ResourceManager * resourceManager = new i6e::modules::ResourceManager(ogrePath);

		Ogre::Image combined;

		std::string pref = prefix;

		combined.loadTwoImagesAsRGBA(diffuseTexture, specularTexture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_A8R8G8B8);
		combined.save(pref + "_diffusespecular.dds");

		combined.loadTwoImagesAsRGBA(normalTexture, displacementTexture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::PF_A8R8G8B8);
		combined.save(pref + "_normalheight.dds");
		
		delete resourceManager;
		delete dhbm;
		delete root;
		delete lm;
	}

	return 0;
}

