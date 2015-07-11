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

#ifndef __I6ENGINE_MODULES_GRAPHICSNODE_H__
#define __I6ENGINE_MODULES_GRAPHICSNODE_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/components/LuminousAppearanceComponent.h"

namespace Ogre {
	class AnimationState;
	class Billboard;
	class BillboardSet;
	class Camera;
	class Entity;
	class Light;
	class ManualObject;
	class ParticleSystem;
	class SceneNode;
} /* namespace Ogre */

namespace i6engine {
namespace api {
namespace graphics {
	enum class BillboardOrigin;
} /* namespace graphics */
} /* namespace api */
namespace modules {

	class GraphicsManager;
	class MovableText;

	/**
	 * \ingroup Graphic
	 *
	 * \class GraphicsNode
	 * \brief Saves all graphics-related data associated with one GameObject.
	 */
	class GraphicsNode {
		friend class GraphicsManager;

	private:
		/**
		 * \brief Creates a new node with unique name
		 *
		 *     Invokes Ogre's SceneNode constructor createChildSceneNode
		 *
		 *			Creates a new SceneNode with unique name according to the goid
		 * \param   const int goid, const Vec3 & position
		 * \return   nothing
		 *
		 */
		GraphicsNode(GraphicsManager * manager, const int64_t goid, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale);

		/**
		 * \brief ~GraphicsNode
		 *
		 *     Deletes whatever variables have been pointing to objects (_parentNode, _cameraNode, _meshEntity, _light)
		 *     Then calls Ogre's removeChild to delete Ogre's SceneNode-Object and Ogre's SceneManager's destroySceneNode to destroy the scene.
		 * \param   no params
		 * \return   nothing
		 *
		 */
		~GraphicsNode();

		/**
		 * \brief Gets SceneNode of the object
		 *
		 * Gets SceneNode of the object
		 * \return SceneNode of the object
		 */
		inline Ogre::SceneNode * getSceneNode() const { return _sceneNode; }
		Ogre::SceneNode * getOrCreateSceneNode(const int64_t coid, const Vec3 & pos, const Quaternion & rot, const Vec3 & scale);

		/**
		 * \brief Sets the node's parent node
		 *
		 *     Sets the Node's parent node. If newParent is NULL, the node is attached to the scenegraph's root node.
		 *
		 * \param   GraphicsNode * newParent)
		 * \return   nothing
		 *
		 */
		void setParent(GraphicsNode * newParent);

		 /**
		 * \brief Creates or updates a material component
		 *
		 *     sets material name
		 *
		 *			Creates or updates a material component for the object with given materialName.
		 *
		 * \param   const std::string & materialName
		 * \return   nothing
		 *
		 */
		void setMaterial(const int64_t coid, const std::string & materialName);

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
		 * \brief Creates or updates a mesh component
		 *
		 *     creates a new Ogre Entity by calling Ogre's createEntity, then configures it
		 *
		 *			Creates or updates a mesh component for the object with given meshFile and visibility-state
		 * \param[in] coid ComponentID to identifiy the subentity
		 * \param   const std::string & meshName, const bool isVisible
		 * \return   nothing
		 *
		 */
		void createMeshComponent(const int64_t coid, const std::string & meshName, const bool visible);
		void updateMeshComponent(const int64_t coid, const std::string & meshName, const bool visible);

		/**
		 * \brief Creates or updates camera position
		 *
		 *     creates a new Ogre camera by calling Ogre's createCamera, then configures it
		 *
		 *			Creates a new camera if _camera is NULL, otherwise it changes position to parameter
		 * \param   const Vec3 & position, const Vec3 & lookAt, const int nC
		 * \return   nothing
		 *
		 */
		void createCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, const double fov);
		void updateCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, const double fov);

		/**
		 * \brief Creates or updates a viewport
		 *
		 *     creates a new Ogre viewport by calling Ogre's addViewport, then sets its dimensions
		 *
		 * \param   const float left, const float top, const float width, const float height, const float red, const float green, const float blue, const float alpha
		 * \return   nothing
		 *
		 */
		void createOrUpdateViewport(const int64_t coid, const double left, const double top, const double width, const double height, const double red, const double green, const double blue, const double alpha);

		/**
		 * \brief Creates new light or updates it
		 *
		 *     Creates a new light using Ogre's createLight constructor and configures it
		 *
		 *			Creates new light if _light is NULL. Then it updates attributes of the light
		 * \param[in] type LightType (LT_SPOT, LT_POINT etc.)
		 * \param[in] diffuse Diffuse colour of the light
		 * \param[in] specular Specular colour of the light
		 * \param[in] attenuation Attenuation of den light
		 * \param[in] direction Direction of the light
		 * \return   nothing
		 *
		 */
		void createLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter);
		void updateLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter);

		/**
		 * \brief Updates the camera frustum
		 */
		void updateCameraFrustumComponent(const int64_t coid, const double left, const double right, const double top, const double bottom);

		/**
		 * \brief Creates a new ParticleEmitter
		 *
		 * \param emitterName Script file for this emitter
		 * \param pos Relative Position to this GraphicsNode
		 */
		void createParticleComponent(const int64_t coid, const std::string & emitterName, const Vec3 & pos);

		/**
		 * \brief lets a particle fade out
		 */
		void particleFadeOut(int64_t coid);

		/**
		 * \brief Deletes the MeshComponent of the Node
		 *
		 *     Deletes an Ogre Entity by calling sm->destroyEntity
		 *
		 * \param   no params
		 * \return   nothing
		 *
		 */
		void deleteMeshComponent(const int64_t coid);

		/**
		 * \brief Deletes the LuminousComponent of the Node
		 *
		 *     Deletes an Ogre lighting object by calling sm->destroyLight
		 *
		 * \param   no params
		 * \return   nothing
		 *
		 */
		void deleteLuminousComponent(const int64_t coid);

		/**
		 * \brief Deletes CameraComponent of the Node
		 *
		 *     Deletes an Ogre Camera by calling sm->destroyCamera
		 *
		 * \param   no params
		 * \return   nothing
		 *
		 */
		void deleteCameraComponent(const int64_t coid);

		/**
		 * \brief deletes the ParticleEmitter of this Node
		 */
		void deleteParticleComponent(const int64_t coid);

		/**
		 * \brief plays an animation
		 */
		void playAnimation(const int64_t coid, const std::string & anim, bool looping, double offsetPercent);

		/**
		 * \brief ticking the GraphicsNode
		 */
		void Tick();

		/**
		 * \brief stops an animation
		 */
		void stopAnimation();

		/**
		 * \brief sets animation speed
		 */
		void setAnimationSpeed(double animationSpeed) {
			_animationSpeed = animationSpeed;
		}

		void createBilldboardSetComponent(int64_t coid, const std::string & material, double width, double height, api::graphics::BillboardOrigin bo);

		void createOrUpdateBillboard(int64_t coid, const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1);

		void deleteBillboard(int64_t coid, const std::string & identifier);

		void deleteBillboardSetComponent(int64_t coid);

		void createMovableText(int64_t coid, int64_t targetID, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour);
		void updateMovableText(int64_t coid, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour);
		void deleteMovableText(int64_t coid);

		void enableCompositor(int64_t coid, const std::string & compositor, bool enabled);

		void drawBoundingBox(int64_t coid, const Vec3 & colour);

		void removeBoundingBox();

		/**
		 * \brief forbidden
		 */
		GraphicsNode(const GraphicsNode &) = delete;

		/**
		 * \brief forbidden
		 */
		const GraphicsNode & operator=(const GraphicsNode &) = delete;

		/**
		 * \brief stores the GraphicsManager controlling this object
		 */
		GraphicsManager * _manager;

		/**
		 * ID of the object
		 */
		int64_t _gameObjectID;

		/**
		 * SceneNode of the object
		 */
		Ogre::SceneNode * _sceneNode;

		/**
		 * Parent node of the object
		 */
		GraphicsNode * _parentNode;

		/**
		 * Cameras node of the object
		 */
		std::map<int64_t, Ogre::SceneNode *> _cameras;

		/**
		 * Lights of the object
		 */
		std::map<int64_t, Ogre::SceneNode *> _lights;

		/**
		 * ParticleSystems of the object
		 */
		std::map<int64_t, Ogre::SceneNode *> _particles;

		/**
		 * \brief list of all subSceneNodes
		 * currently used only for meshes
		 */
		std::map<int64_t, Ogre::SceneNode *> _sceneNodes;

		/**
		 * \brief current animation state
		 */
		Ogre::AnimationState * _animationState;

		double _animationSpeed;

		uint64_t _lastTime;

		std::map<int64_t, std::pair<Ogre::BillboardSet *, std::map<std::string, Ogre::Billboard *>>> _billboardSets;

		std::map<int64_t, MovableText *> _movableTexts;

		/**
		 * \brief list of all MovableTexts depending on a mesh. Key is mesh
		 */
		std::map<int64_t, std::vector<int64_t>> _observer;

		/**
		 * \brief bounding box of this object
		 */
		Ogre::ManualObject * _boundingBox;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GRAPHICSNODE_H__ */

/**
 * @}
 */
