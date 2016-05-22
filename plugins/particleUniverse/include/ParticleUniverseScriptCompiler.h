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

#ifndef __PU_SCRIPT_COMPILER_H__
#define __PU_SCRIPT_COMPILER_H__

#include "OgreScriptCompiler.h"

namespace ParticleUniverse {

	// If the Ogre renderer is replaced by another renderer, the types below must be re-implemented.
	typedef Ogre::ScriptCompiler ScriptCompiler;
	typedef Ogre::ScriptCompilerManager ScriptCompilerManager;
	typedef Ogre::ObjectAbstractNode ObjectAbstractNode;
	typedef Ogre::AbstractNodePtr AbstractNodePtr;
	typedef Ogre::AbstractNodeList AbstractNodeList;
	typedef Ogre::PropertyAbstractNode PropertyAbstractNode;

	typedef Ogre::AbstractNodeType AbstractNodeType;
	static const AbstractNodeType ANT_PROPERTY = Ogre::ANT_PROPERTY;
	static const AbstractNodeType ANT_OBJECT = Ogre::ANT_OBJECT;

} /* namespace ParticleUniverse */

#endif /* __PU_SCRIPT_COMPILER_H__ */
