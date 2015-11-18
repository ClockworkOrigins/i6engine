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

#ifndef __I6ENGINE_MODULES_COMPONENTS_LUMINOUSCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_LUMINOUSCOMPONENT_H__

#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/components/LuminousAppearanceComponent.h"

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
	 * \class LuminousComponent
	 * \brief Handles all light related functionality
	 */
	class LuminousComponent {
		friend class GraphicsNode;

	private:
		/**
		 * \brief stores the GraphicsManager controlling this object
		 */
		GraphicsManager * _manager;

		/**
		 * ID of the object
		 */
		int64_t _gameObjectID;

		/**
		 * parent of this Component
		 */
		GraphicsNode * _parent;

		/**
		 * SceneNode of the object
		 */
		Ogre::SceneNode * _sceneNode;

		/**
		 * \brief Create a new LuminousComponent
		 */
		LuminousComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter);

		/**
		 * \brief ~LuminousComponent
		 */
		~LuminousComponent();

		/**
		 * \brief Creates new light or updates it
		 *
		 * Creates a new light using Ogre's createLight constructor and configures it
		 * \param[in] type LightType (LT_SPOT, LT_POINT etc.)
		 * \param[in] diffuse Diffuse colour of the light
		 * \param[in] specular Specular colour of the light
		 * \param[in] attenuation Attenuation of den light
		 * \param[in] direction Direction of the light
		 */
		void updateLuminousComponent(const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter);

		/**
		 * \brief forbidden
		 */
		LuminousComponent(const LuminousComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const LuminousComponent & operator=(const LuminousComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COMPONENTS_LUMINOUSCOMPONENT_H__ */

/**
 * @}
 */
