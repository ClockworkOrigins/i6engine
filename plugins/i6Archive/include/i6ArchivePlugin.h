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

		i6ArchivePlugin & operator=(const i6ArchivePlugin &) {
			return *this;
		}
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_I6ARCHIVEPLUGIN_H__ */
