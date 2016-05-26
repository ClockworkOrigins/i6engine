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

#ifndef __I6ENGINE_PLUGINS_I6ARCHIVEFACTORY_H__
#define __I6ENGINE_PLUGINS_I6ARCHIVEFACTORY_H__

#include "OGRE/OgreArchiveFactory.h"

namespace i6engine {
namespace plugins {

	/**
	 * \class i6ArchiveFactory factory to produce i6Archives
	 */
	class i6ArchiveFactory : public Ogre::ArchiveFactory {
	public:
		i6ArchiveFactory() : ArchiveFactory(), _type("i6arch") {
		}

	private:
		const Ogre::String _type;
        /**
		 * \brief Creates a new object.
         * \param name Name of the object to create
         * \return An object created by the factory. The type of the object depends on the factory. In this case it's an i6Archive
         */
		Ogre::Archive * createInstance(const Ogre::String & name, bool readOnly) override;
		Ogre::Archive * createInstance(const Ogre::String & name) override {
			return createInstance(name, true);
		}

		/**
		 * \brief returns the type this factory supports
		 * here it is i6arch
		 */
		const Ogre::String & getType() const override {
			return _type;
		}

		void destroyInstance(Ogre::Archive * archive) override;

		i6ArchiveFactory & operator=(const i6ArchiveFactory &) {
			return *this;
		}
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_I6ARCHIVEFACTORY_H__ */
