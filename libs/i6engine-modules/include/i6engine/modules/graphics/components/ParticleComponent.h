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
		 * Scale of this particle
		 */
		Vec3 _scale;

		/**
		 * \brief Create a new ParticleComponent
		 */
		ParticleComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & emitterName, const Vec3 & pos, const Vec3 & scale);

		/**
		 * \brief ~ParticleComponent
		 */
		~ParticleComponent();

		/**
		 * \brief lets a particle fade out
		 */
		void particleFadeOut();

		/**
		 * \brief sets Scale of this particle
		 */
		void setScale(const Vec3 & scale);

		/**
		 * \brief updates scale if parent changed
		 */
		void updateScale();

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
