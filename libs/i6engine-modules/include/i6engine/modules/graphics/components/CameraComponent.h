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

#ifndef __I6ENGINE_MODULES_COMPONENTS_CAMERACOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_CAMERACOMPONENT_H__

#include <cstdint>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

namespace Ogre {
	class SceneNode;
} /* namespace Ogre */

namespace i6e {
namespace modules {

	class GraphicsManager;
	class GraphicsNode;

	/**
	 * \ingroup Graphic
	 *
	 * \class CameraComponent
	 * \brief Handles all light related functionality
	 */
	class CameraComponent {
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

		int _zOrder;

		/**
		 * \brief Create a new CameraComponent
		 */
		CameraComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov);

		/**
		 * \brief ~CameraComponent
		 */
		~CameraComponent();

		void updateCameraComponent(const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov);

		/**
		 * \brief Updates the camera frustum
		 */
		void updateCameraFrustumComponent(const double left, const double right, const double top, const double bottom);

		/**
		 * \brief enables or disables a compositor
		 */
		void enableCompositor(const std::string & compositor, bool enabled);

		/**
		 * \brief Creates or updates a viewport
		 *
		 * creates a new Ogre viewport by calling Ogre's addViewport, then sets its dimensions
		 */
		void createOrUpdateViewport(int zOrder, const double left, const double top, const double width, const double height, const double red, const double green, const double blue, const double alpha);

		/**
		 * \brief forbidden
		 */
		CameraComponent(const CameraComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const CameraComponent & operator=(const CameraComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_CAMERACOMPONENT_H__ */

/**
 * @}
 */
