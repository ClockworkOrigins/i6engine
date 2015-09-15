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

#include "wx/ogre/rendersystem.h"

#include "wx/ogre/prerequisites.h"
#include "wx/ogre/utils.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
 #ifndef OGRE_PLUGINDIR
 #define OGRE_PLUGINDIR ""
 #endif
#else 
 #define OGRE_PLUGINDIR ""
#endif

wxOgreRenderSystem::wxOgreRenderSystem() {
#ifndef WXOGRE_DEBUG
	_root = new Ogre::Root("", "", "");
#else
	_root = new Ogre::Root("", "", "boot.log");
#endif
}

wxOgreRenderSystem::wxOgreRenderSystem(const Ogre::String & plugins, const Ogre::String & config, const Ogre::String & log) {
	// Create a new Ogre ROOT
    // Root("plugins.cfg","config.cfg","boot.log");
	_root = new Ogre::Root(plugins, config, log);
}

wxOgreRenderSystem::~wxOgreRenderSystem() {
    delete _root;
    _root = nullptr;
}

void wxOgreRenderSystem::LoadPlugin(const Ogre::String & plugin) {
    try {
#if WXOGRE_DEBUG == 1
		_root->loadPlugin(plugin + "_d");
#else
		_root->loadPlugin(plugin);
#endif
	} catch (Ogre::Exception & e) {
        wxOgreExceptionBox(e);
    }
}

void wxOgreRenderSystem::SelectOgreRenderSystem(const Ogre::String & render) {
    Ogre::RenderSystemList renderList = _root->getAvailableRenderers();
    // check through all available renderers, if there is one the
    // string is "render"
	for (Ogre::RenderSystemList::iterator it = renderList.begin(); it != renderList.end(); ++it) {
        Ogre::RenderSystem * renderSys = *it;
        if (renderSys->getName() == render) {
            _root->setRenderSystem(renderSys);
            break;
        }
    }

}

void wxOgreRenderSystem::Initialise() {
    _root->initialise(false);
}

bool wxOgreRenderSystem::HasCapability(const Ogre::Capabilities & c) {
    return _root->getRenderSystem()->getCapabilities()->hasCapability(c);
}
