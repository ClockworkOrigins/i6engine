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

#include "i6engine/utils/Exceptions.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "clockUtils/argParser/ArgumentParser.h"
#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreDefaultHardwareBufferManager.h"
#include "OGRE/OgreLogManager.h"
#include "OGRE/OgreRoot.h"

int main(int argc, char ** argv) {
	REGISTER_VARIABLE_REQUIRED(std::string, prefix, p, "", "Prefix of the newly created textures");
	REGISTER_VARIABLE_REQUIRED(std::string, diffuseTexture, dif, "", "Diffuse map to be used");
	REGISTER_VARIABLE_REQUIRED(std::string, normalTexture, n, "", "Normal map to be used");
	REGISTER_VARIABLE_REQUIRED(std::string, specularTexture, s, "", "Specular map to be used");
	REGISTER_VARIABLE_REQUIRED(std::string, displacementTexture, dis, "", "Displacement map to be used");

	if (PARSE_COMMANDLINE() != clockUtils::ClockError::SUCCESS) {
		std::cerr << GETLASTPARSERERROR() << std::endl;
		return 1;
	} else if (HELPSET()) {
		std::cout << GETHELPTEXT() << std::endl;
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

