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

/**
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_COMPONENTS_LINECOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_LINECOMPONENT_H__

#include <cstdint>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

#include "OGRE/OgreMaterial.h"

namespace Ogre {
	class ManualObject;
	class SceneNode;
} /* namespace Ogre */

namespace i6e {
namespace modules {

	class GraphicsManager;
	class GraphicsNode;

	/**
	 * \ingroup Graphic
	 *
	 * \class LineComponent
	 * \brief Handles all light related functionality
	 */
	class LineComponent {
		friend class GraphicsNode;

	private:
		/**
		 * \brief stores the GraphicsManager controlling this object
		 */
		GraphicsManager * _manager;

		/**
		 * parent of this Component
		 */
		GraphicsNode * _parent;

		/**
		 * SceneNode of the object
		 */
		Ogre::SceneNode * _sceneNode;

		/**
		 * ManualObject representing the line
		 */
		Ogre::ManualObject * _line;

		/**
		 * Material of the line
		 */
		Ogre::MaterialPtr _material;

		/**
		 * \brief Create a new LineComponent
		 */
		LineComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const Vec3 & startPos, const Vec3 & endPos, const Vec3 & colour);

		/**
		 * \brief ~LineComponent
		 */
		~LineComponent();

		/**
		 * \brief forbidden
		 */
		LineComponent(const LineComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const LineComponent & operator=(const LineComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_LINECOMPONENT_H__ */

/**
 * @}
 */
