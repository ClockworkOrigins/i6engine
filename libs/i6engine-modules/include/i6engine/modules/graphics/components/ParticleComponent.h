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
	 * \class ParticleComponent
	 * \brief Handles all light related functionality
	 */
	class ParticleComponent {
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
		 * \brief Create a new ParticleComponent
		 */
		ParticleComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & emitterName, const Vec3 & pos);

		/**
		 * \brief ~ParticleComponent
		 */
		~ParticleComponent();

		/**
		 * \brief lets a particle fade out
		 */
		void particleFadeOut();

		/**
		 * \brief forbidden
		 */
		ParticleComponent(const ParticleComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const ParticleComponent & operator=(const ParticleComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_PARTICLECOMPONENT_H__ */

/**
 * @}
 */
