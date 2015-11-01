/*
 * Copyright (C) 2007-2008 Martin Pieuchot
 *
 * This library is free software; you can redistribute it and/or
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include "wx/ogre/resources.h"

#include "wx/ogre/utils.h"

#include "OGRE/OgreConfigFile.h"

wxOgreResources::wxOgreResources() {
	_resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
}
Ogre::StringVectorPtr wxOgreResources::GetResourcesList(const Ogre::String & g) {
	return _resourceGroupManager->listResourceNames(g);
}

Ogre::StringVectorPtr wxOgreResources::GetResourcesList(const Ogre::String & g, const Ogre::String & p) {
	return _resourceGroupManager->findResourceNames(g, p);
}

Ogre::StringVector wxOgreResources::GetResourcesGroups() {
	return _resourceGroupManager->getResourceGroups();
}

bool wxOgreResources::LoadResourceFile(const Ogre::String & file) {
	Ogre::ConfigFile cf;
	cf.load(file);
	Ogre::ConfigFile::SectionIterator it = cf.getSectionIterator();

	while (it.hasMoreElements()) {
		Ogre::String group = it.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap * settings = it.getNext();

		 for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i) {
			Ogre::String type = i->first;
			Ogre::String location = i->second;

			try {
				_resourceGroupManager->addResourceLocation(location, type, group);
			} catch (Ogre::Exception & e) {
				wxOgreExceptionBox(e);
				return false;
			}
		}
	}
	return true;
}

void wxOgreResources::InitialiseAllResources() {
	try {
		_resourceGroupManager->initialiseAllResourceGroups();
	} catch (Ogre::Exception & e) {
		wxOgreExceptionBox(e);
	}
}

void wxOgreResources::AddResource(const Ogre::String & p, const Ogre::String & t, const Ogre::String & g) {
	try {
		_resourceGroupManager->addResourceLocation(p, t, g);
	} catch (Ogre::Exception & e) {
		wxOgreExceptionBox(e);
	}
}
