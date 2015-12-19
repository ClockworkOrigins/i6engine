/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

namespace i6engine {
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
		CameraComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, const double fov);

		/**
		 * \brief ~CameraComponent
		 */
		~CameraComponent();

		void updateCameraComponent(const Vec3 & position, const Vec3 & lookAt, const double nC, const double fov);

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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COMPONENTS_CAMERACOMPONENT_H__ */

/**
 * @}
 */
