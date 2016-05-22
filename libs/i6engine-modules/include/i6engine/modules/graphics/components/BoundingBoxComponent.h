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

#ifndef __I6ENGINE_MODULES_COMPONENTS_BOUNDINGBOXCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_BOUNDINGBOXCOMPONENT_H__

#include <cstdint>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

namespace Ogre {
	class ManualObject;
} /* namespace Ogre */

namespace i6e {
namespace modules {

	class GraphicsManager;
	class GraphicsNode;
	class MeshComponent;

	/**
	 * \ingroup Graphic
	 *
	 * \class BoundingBoxComponent
	 * \brief Handles all light related functionality
	 */
	class BoundingBoxComponent {
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
		Ogre::ManualObject * _boundingBox;

		MeshComponent * _mesh;

		int64_t _id;

		/**
		 * \brief Create a new BoundingBoxComponent
		 */
		BoundingBoxComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, MeshComponent * mesh, const Vec3 & colour);

		/**
		 * \brief ~BoundingBoxComponent
		 */
		~BoundingBoxComponent();

		/**
		 * \brief forbidden
		 */
		BoundingBoxComponent(const BoundingBoxComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const BoundingBoxComponent & operator=(const BoundingBoxComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_BOUNDINGBOXCOMPONENT_H__ */

/**
 * @}
 */
