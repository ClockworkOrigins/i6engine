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
