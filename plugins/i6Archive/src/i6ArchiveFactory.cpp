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

#include "i6ArchiveFactory.h"

#include "i6Archive.h"

namespace i6engine {
namespace plugins {

	Ogre::Archive * i6ArchiveFactory::createInstance(const Ogre::String & name, bool readOnly) {
		return OGRE_NEW i6Archive(name, "i6arch");
	}

	void i6ArchiveFactory::destroyInstance(Ogre::Archive * archive) {
		OGRE_DELETE archive;
	}

} /* namespace plugins */
} /* namespace i6engine */
