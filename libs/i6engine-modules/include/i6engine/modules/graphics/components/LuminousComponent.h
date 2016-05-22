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

#ifndef __I6ENGINE_MODULES_COMPONENTS_LUMINOUSCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_LUMINOUSCOMPONENT_H__

#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/components/LuminousAppearanceComponent.h"

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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_LUMINOUSCOMPONENT_H__ */

/**
 * @}
 */
