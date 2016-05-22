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

#include "i6ArchivePlugin.h"

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6ArchiveFactory.h"

#include "OGRE/OgreArchiveManager.h"
#include "OGRE/OgreRoot.h"

#ifndef ISIXE_PLUGIN_I6ARCHIVE_EXPORT
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#define ISIXE_PLUGIN_I6ARCHIVE_EXPORT __declspec(dllexport)
	#else
		#define ISIXE_PLUGIN_I6ARCHIVE_EXPORT
	#endif
#endif

namespace i6engine {
namespace plugins {

	i6ArchivePlugin * plugin;
	i6ArchiveFactory * factory;

	extern "C" void ISIXE_PLUGIN_I6ARCHIVE_EXPORT dllStartPlugin() throw() {
		plugin = OGRE_NEW i6ArchivePlugin();
		Ogre::Root::getSingleton().installPlugin(plugin);
	}

	extern "C" void ISIXE_PLUGIN_I6ARCHIVE_EXPORT dllStopPlugin() {
		Ogre::Root::getSingleton().uninstallPlugin(plugin);
		OGRE_DELETE plugin;
	}

	i6ArchivePlugin::i6ArchivePlugin() : Plugin(), _name("i6Archiver") {
	}

	void i6ArchivePlugin::install() {
		factory = OGRE_NEW i6ArchiveFactory();
		Ogre::ArchiveManager::getSingleton().addArchiveFactory(factory);
	}

	void i6ArchivePlugin::initialise() {
	}

	void i6ArchivePlugin::shutdown() {
	}

	void i6ArchivePlugin::uninstall() {
		OGRE_DELETE factory;
	}

} /* namespace plugins */
} /* namespace i6engine */
