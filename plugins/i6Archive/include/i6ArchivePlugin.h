#ifndef __I6ENGINE_PLUGINS_I6ARCHIVEPLUGIN_H__
#define __I6ENGINE_PLUGINS_I6ARCHIVEPLUGIN_H__

#include "OGRE/OgrePlugin.h"

namespace i6engine {
namespace plugins {

	/**
	 * \class i6ArchivePlugin plugin interface to interact with Ogre
	 */
	class i6ArchivePlugin : public Ogre::Plugin {
	public:
		i6ArchivePlugin();

		const Ogre::String & getName() const {
			return _name;
		}

		void install() override;

		void initialise() override;

		void shutdown() override;

		void uninstall() override;

	private:
		const Ogre::String _name;
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_I6ARCHIVEPLUGIN_H__ */
