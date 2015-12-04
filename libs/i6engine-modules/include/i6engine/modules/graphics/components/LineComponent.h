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

#ifndef __I6ENGINE_MODULES_COMPONENTS_PARTICLECOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_PARTICLECOMPONENT_H__

#include <cstdint>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

#include "OGRE/OgreMaterial.h"

namespace Ogre {
	class ManualObject;
	class SceneNode;
} /* namespace Ogre */

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COMPONENTS_PARTICLECOMPONENT_H__ */

/**
 * @}
 */
