#include "i6ArchivePlugin.h"

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
		Ogre::ArchiveManager::getSingleton().addArchiveFactory(OGRE_NEW i6ArchiveFactory());
	}

	void i6ArchivePlugin::initialise() {
	}

	void i6ArchivePlugin::shutdown() {
	}

	void i6ArchivePlugin::uninstall() {
	}

} /* namespace plugins */
} /* namespace i6engine */
