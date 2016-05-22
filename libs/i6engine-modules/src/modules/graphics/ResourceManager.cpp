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

#include "i6engine/modules/graphics/ResourceManager.h"

#include "OGRE/OgreConfigFile.h"
#include "OGRE/OgreResourceGroupManager.h"
#include "OGRE/OgreString.h"

namespace i6e {
namespace modules {

	ResourceManager::ResourceManager(const std::string & ogrePath) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		Ogre::ConfigFile cf;
		cf.load(ogrePath + "/resources.cfg"); // resources.cfg file has to be in /bin

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;

		while (seci.hasMoreElements()) {
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap * settings = seci.getNext();

			 for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i) {
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}
	}

	ResourceManager::~ResourceManager() {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("i6engine");
	}

} /* namespace modules */
} /* namespace i6e */
