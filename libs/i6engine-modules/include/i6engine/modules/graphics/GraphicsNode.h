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
	class SceneNode;
} /* namespace Ogre */

namespace i6engine {
namespace api {
namespace graphics {
	enum class BillboardOrigin;
} /* namespace graphics */
} /* namespace api */
namespace modules {

	class BillboardComponent;
	class BoundingBoxComponent;
	class CameraComponent;
	class GraphicsManager;
	class LineComponent;
	class LuminousComponent;
	class MeshComponent;
	class MovableTextComponent;
	class ParticleComponent;

	/**
	 * \ingroup Graphic
	 *
	 * \class GraphicsNode
	 * \brief Saves all graphics-related data associated with one GameObject.
	 */
	class GraphicsNode {
		friend class GraphicsManager;
		friend class MeshComponent;

	public:
		/**
		 * \brief Gets SceneNode of the object
		 *
		 * Gets SceneNode of the object
		 * \return SceneNode of the object
		 */
		inline Ogre::SceneNode * getSceneNode() const {
			return _sceneNode;
		}

		void addTicker(MeshComponent * mesh);

		void removeTicker(MeshComponent * mesh);

		void addTicker(MovableTextComponent * mesh);

		void removeTicker(MovableTextComponent * mesh);

	private:
		/**
		 * \brief Creates a new node with unique name
		 *
		 *     Invokes Ogre's SceneNode constructor createChildSceneNode
		 *
		 *			Creates a new SceneNode with unique name according to the goid
		 */
		GraphicsNode(GraphicsManager * manager, const int64_t goid, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale);

		/**
		 * \brief ~GraphicsNode
		 *
		 *     Deletes whatever variables have been pointing to objects (_parentNode, _cameraNode, _meshEntity, _light)
		 *     Then calls Ogre's removeChild to delete Ogre's SceneNode-Object and Ogre's SceneManager's destroySceneNode to destroy the scene.
		 */
		~GraphicsNode();

		/**
		 * \brief Creates or updates a mesh component
		 *
		 *     creates a new Ogre Entity by calling Ogre's createEntity, then configures it
		 *
		 *			Creates or updates a mesh component for the object with given meshFile and visibility-state
		 * \param[in] coid ComponentID to identifiy the subentity
		 */
		void createMeshComponent(const int64_t coid, const std::string & meshName, const bool visible, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale);
		void updateMeshComponent(const int64_t coid, const std::string & meshName, const bool visible);
		void updateMeshComponentVisibility(const int64_t coid, const bool visible);

		/**
		 * \brief Creates or updates a material component
		 *
		 *     sets material name
		 *
		 *			Creates or updates a material component for the object with given materialName.
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
		 * \brief plays an animation
		 */
		void playAnimation(const int64_t coid, const std::string & anim, bool looping, double offsetPercent);

		/**
		 * \brief sets animation speed
		 */
		void setAnimationSpeed(int64_t coid, double animationSpeed);

		/**
		 * \brief stops an animation
		 */
		void stopAnimation(int64_t coid);

		/**
		 * \brief adds an event being triggered, when a specified frame time of the animation is reached
		 * note that all events are removed when another animation is played!
		 */
		void addAnimationFrameEvent(int64_t coid, uint64_t frame, const std::function<void(void)> & func);

		/**
		 * \brief Deletes the MeshComponent of the Node
		 *
		 *     Deletes an Ogre Entity by calling sm->destroyEntity
		 */
		void deleteMeshComponent(const int64_t coid);

		/**
		 * \brief Creates or updates camera position
		 *
		 *     creates a new Ogre camera by calling Ogre's createCamera, then configures it
		 *
		 *			Creates a new camera if _camera is NULL, otherwise it changes position to parameter
		 */
		void createCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov);
		void updateCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov);

		/**
		 * \brief Updates the camera frustum
		 */
		void updateCameraFrustumComponent(const int64_t coid, const double left, const double right, const double top, const double bottom);

		/**
		 * \brief enables or disables a compositor
		 */
		void enableCompositor(int64_t coid, const std::string & compositor, bool enabled);

		/**
		 * \brief Creates or updates a viewport
		 *
		 *     creates a new Ogre viewport by calling Ogre's addViewport, then sets its dimensions
		 */
		void createOrUpdateViewport(const int64_t coid, int zOrder, const double left, const double top, const double width, const double height, const double red, const double green, const double blue, const double alpha);

		/**
		 * \brief Deletes CameraComponent of the Node
		 *
		 *     Deletes an Ogre Camera by calling sm->destroyCamera
		 */
		void deleteCameraComponent(const int64_t coid);

		/**
		 * \brief Creates new light
		 * Creates a new LuminousComponent
		 * \param[in] type LightType (LT_SPOT, LT_POINT etc.)
		 * \param[in] diffuse Diffuse colour of the light
		 * \param[in] specular Specular colour of the light
		 * \param[in] attenuation Attenuation of den light
		 * \param[in] direction Direction of the light
		 */
		void createLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter);
		
		/**
		 * \brief Updates a light
		 * Updates attributes of a LuminousComponent
		 * \param[in] type LightType (LT_SPOT, LT_POINT etc.)
		 * \param[in] diffuse Diffuse colour of the light
		 * \param[in] specular Specular colour of the light
		 * \param[in] attenuation Attenuation of den light
		 * \param[in] direction Direction of the light
		 */
		void updateLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter);

		/**
		 * \brief Deletes the LuminousComponent of the Node
		 */
		void deleteLuminousComponent(const int64_t coid);

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
		 * \brief deletes the ParticleEmitter of this Node
		 */
		void deleteParticleComponent(const int64_t coid);

		/**
		 * \brief creates a billboard set
		 */
		void createBilldboardSetComponent(int64_t coid, const std::string & material, double width, double height, api::graphics::BillboardOrigin bo);

		/**
		 * \brief creates a billboard on the billboard set
		 */
		void createOrUpdateBillboard(int64_t coid, const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1);

		/**
		 * \brief deletes the billboard
		 */
		void deleteBillboard(int64_t coid, const std::string & identifier);

		/**
		 * \brief deletes the billboard set
		 */
		void deleteBillboardSetComponent(int64_t coid);

		/**
		 * \brief creates a movable text upon the target mesh
		 */
		void createMovableText(int64_t coid, int64_t targetID, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour);

		/**
		 * \brief updates settings of the movable text
		 */
		void updateMovableText(int64_t coid, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour);

		/**
		 * \brief deletes movable text
		 */
		void deleteMovableText(int64_t coid);

		/**
		 * \brief draws the surrounding bounding box of the mesh with given colour
		 */
		void drawBoundingBox(int64_t coid, const Vec3 & colour);

		/**
		 * \brief removes the bounding box
		 */
		void removeBoundingBox(int64_t coid);

		/**
		 * \brief creates a line with given colour
		 */
		void createLine(int64_t coid, const Vec3 & from, const Vec3 & to, const Vec3 & colour);

		/**
		 * \brief removes a line
		 */
		void removeLine(int64_t coid);

		/*
		 * \brief attaches an other GraphicsNode to a bone of a mesh of this GraphicsNode
		 */
		void attachToBone(int64_t coid, GraphicsNode * otherNode, const std::string & boneName);

		/**
		 * \brief detaches an other GraphicsNode from a bone of a mesh of this GraphicsNode
		 */
		void detachFromBone(int64_t coid, GraphicsNode * otherNode, const std::string & boneName);

		/**
		 * \brief registers that this node is attached to another node so it can detach on deletion
		 */
		void listenAttachment(GraphicsNode * otherNode, int64_t coid, const std::string & boneName);

		/**
		 * \brief unregisters attachment when connection is detached
		 */
		void stopListenAttachment();

		/**
		 * \brief ticking the GraphicsNode
		 */
		void Tick();

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
		 * Cameras node of the object
		 */
		std::map<int64_t, CameraComponent *> _cameras;

		/**
		 * Lights of the object
		 */
		std::map<int64_t, LuminousComponent *> _lights;

		/**
		 * ParticleSystems of the object
		 */
		std::map<int64_t, ParticleComponent *> _particles;

		/**
		 * \brief list of all meshes
		 */
		std::map<int64_t, MeshComponent *> _meshes;

		std::map<int64_t, BillboardComponent *> _billboardSets;

		std::map<int64_t, MovableTextComponent *> _movableTexts;

		std::map<int64_t, BoundingBoxComponent *> _boundingBoxes;

		std::map<int64_t, LineComponent *> _lines;
		GraphicsNode * _attachedTo;
		int64_t _attachedCoid;
		std::string _attachedBone;

		api::WeakGOPtr _go;

		bool _ticking;

		std::vector<MeshComponent *> _tickingMeshes;
		std::vector<MovableTextComponent *> _tickingMovableTexts;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GRAPHICSNODE_H__ */

/**
 * @}
 */
