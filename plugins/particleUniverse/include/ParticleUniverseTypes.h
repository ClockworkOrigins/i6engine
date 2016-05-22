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

#ifndef __PU_TYPES_H__
#define __PU_TYPES_H__

#include "OGRE/OgreColourValue.h"
#include "OgrePlatform.h"
#include "OgrePrerequisites.h"
#include "OgreString.h"
#include "OgreStringVector.h"
#include "OgreVector2.h"
#include "OgreVector3.h"

namespace ParticleUniverse {

	// Define types
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef int int32;
	typedef short int16;
	typedef unsigned char uchar;

	/** Define other types. Particle Universe needs its own types to be independent from external resources, such as the Ogre renderer. 
		For now, the types are still borrowed from Ogre. If the Ogre renderer is replaced by another renderer, the types below must be re-implemented.
	*/
	typedef Ogre::Real Real;
	typedef Ogre::Vector3 Vector3;
	typedef Ogre::Vector4 Vector4;
	typedef Ogre::Vector2 Vector2;
	typedef Ogre::Quaternion Quaternion;
	typedef Ogre::ColourValue ColourValue;
	typedef Ogre::String String;
	typedef Ogre::StringUtil StringUtil;
	typedef Ogre::StringConverter StringConverter;
	typedef Ogre::StringVector StringVector;
	typedef Ogre::StringVectorPtr StringVectorPtr;
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR < 10
	typedef StringUtil::StrStreamType StringStream;
#else
	typedef Ogre::StringStream StringStream;
#endif

	// Constants
	const String BLANK_STRING = "";

	// Typedef's for specific 'Ogre' containers.
	// vector container
	template<class T>
	struct vector : public std::vector<T, Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy>> {
	};

	// list container
	template<class T>
	struct list : public std::list<T, Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy>> { 
	};

	// map container
	template<class T, class M>
	struct map : public std::map<T, M, std::less<T>, Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy>> { 
	};

} /* namespace ParticleUniverse */

#endif /* __PU_TYPES_H__ */
