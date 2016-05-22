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

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniversePlugin.h"

#include "OGRE/OgreRoot.h"

namespace ParticleUniverse {

	ParticleUniversePlugin * plugin;

	extern "C" void _ParticleUniverseExport dllStartPlugin() throw() {
		plugin = PU_NEW ParticleUniversePlugin();
		Ogre::Root::getSingleton().installPlugin(plugin);
	}

	extern "C" void _ParticleUniverseExport dllStopPlugin() {
		Ogre::Root::getSingleton().uninstallPlugin(plugin);
		PU_DELETE plugin;
	}

} /* namespace ParticleUniverse */
