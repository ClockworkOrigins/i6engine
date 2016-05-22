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

#ifndef __I6ENGINE_MODULES_COMPONENTS_MESHCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_MESHCOMPONENT_H__

#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

#include "i6engine/utils/i6eThreadSafety.h"
#include "i6engine/utils/weakPtr.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

namespace Ogre {
	class AnimationState;
	class Entity;
	class SceneNode;
} /* namespace Ogre */

namespace i6e {
namespace api {
	class Component;
	class MeshAppearanceComponent;
} /* namespace api */
namespace modules {

	class BoundingBoxComponent;
	class GraphicsManager;
	class GraphicsNode;
	class MovableTextComponent;

	/**
	 * \ingroup Graphic
	 *
	 * \class MeshComponent
	 * \brief Handles all light related functionality
	 */
	class MeshComponent {
		friend class BoundingBoxComponent;
		friend class GraphicsNode;
		friend class MovableTextComponent;

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
		 * \brief current animation state
		 */
		Ogre::AnimationState * _animationState;

		double _animationSpeed;

		uint64_t _lastTime;
		uint64_t _lastFrameTime;

		std::vector<int64_t> _movableTextObservers;
		std::vector<int64_t> _boundingBoxObservers;

		std::map<GraphicsNode *, std::string> _attachedNodes;
		struct sortFrameFunctions {
			bool operator()(const std::pair<uint64_t, std::function<void(void)>> & a, const std::pair<uint64_t, std::function<void(void)>> & b) const {
				return !(a.first < b.first);
			}
		};

		std::priority_queue<std::pair<uint64_t, std::function<void(void)>>, std::vector<std::pair<uint64_t, std::function<void(void)>>>, sortFrameFunctions> _queueA;
		std::priority_queue<std::pair<uint64_t, std::function<void(void)>>, std::vector<std::pair<uint64_t, std::function<void(void)>>>, sortFrameFunctions> _queueB;

		utils::weakPtr<api::MeshAppearanceComponent, api::Component> _meshComponent;
		bool _shadowCasting;

		/**
		 * \brief Create a new MeshComponent
		 */
		MeshComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & meshName, const bool visible, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale);

		/**
		 * \brief ~MeshComponent
		 */
		~MeshComponent();

		void updateMeshComponent(const std::string & meshName, const bool visible);

		void setVisibility(bool visible);

		void setShadowCasting(bool enabled);

		/**
		 * \brief Creates or updates a material component
		 *
		 *     sets material name
		 *
		 *			Creates or updates a material component for the object with given materialName.
		 */
		void setMaterial(const std::string & materialName);

		/**
		 * \brief Sets a parameter for a shader script.
		 *
		 * The parameter is set for all SubEntities of this mesh.
		 *
		 * \param[in] coid ComponentID to identifiy the subentity
		 * \param num The number, the parameter has in the shader script.
		 * \param value The Value to set the parameter to.
		 */
		void setCustomParameter(uint32_t num, const Vec4 & value);

		/**
		 * \brief plays an animation
		 */
		void playAnimation(const std::string & anim, bool looping, double offsetPercent);

		/**
		 * \brief sets animation speed
		 */
		void setAnimationSpeed(double animationSpeed) {
			_animationSpeed = animationSpeed;
		}

		/**
		 * \brief stops an animation
		 */
		void stopAnimation();

		/**
		 * \brief adds an event being triggered, when a specified frame time of the animation is reached
		 * note that all events are removed when another animation is played!
		 */
		void addAnimationFrameEvent(uint64_t frameTime, const std::function<void(void)> & func);

		void Tick();

		void addMovableTextObserver(int64_t coid) {
			_movableTextObservers.push_back(coid);
		}

		void removeMovableTextObserver(int64_t coid);

		void addBoundingBoxObserver(int64_t coid) {
			_boundingBoxObservers.push_back(coid);
		}

		void removeBoundingBoxObserver(int64_t coid);

		Ogre::Entity * getEntity() const;

		void attachToBone(GraphicsNode * otherNode, const std::string & boneName);

		void detachFromBone(GraphicsNode * otherNode);

		/**
		 * \brief forbidden
		 */
		MeshComponent(const MeshComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const MeshComponent & operator=(const MeshComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_MESHCOMPONENT_H__ */

/**
 * @}
 */
