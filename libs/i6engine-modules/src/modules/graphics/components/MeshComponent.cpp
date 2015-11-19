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

#include "i6engine/modules/graphics/components/MeshComponent.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreEntity.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSubEntity.h"

namespace i6engine {
namespace modules {

	MeshComponent::MeshComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & meshName, const bool visible, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) : _manager(manager), _parent(parent), _sceneNode(nullptr), _animationState(nullptr), _animationSpeed(1.0), _lastTime(), _movableTextObservers(), _boundingBoxObservers() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name, position.toOgre());
		_sceneNode->setOrientation(rotation.toOgre());
		_sceneNode->setScale(scale.toOgre());
		Ogre::Entity * meshEntity = sm->createEntity("mesh_" + std::to_string(goid) + "_" + std::to_string(coid), meshName);
		meshEntity->setVisible(visible);
		meshEntity->setCastShadows(true);
		_sceneNode->attachObject(meshEntity);

		meshEntity->getMesh()->buildEdgeList();

		try {
			unsigned short src, dest;
			if (!meshEntity->getMesh()->suggestTangentVectorBuildParams(Ogre::VertexElementSemantic::VES_TANGENT, src, dest)) {
				meshEntity->getMesh()->buildTangentVectors(Ogre::VertexElementSemantic::VES_TANGENT, src, dest);
			}
		} catch (const Ogre::Exception & e) {
			ISIXE_LOG_WARN("GraphicsNode", e.what());
		}

		if (api::EngineController::GetSingletonPtr()->getDebugdrawer() == 3 || api::EngineController::GetSingletonPtr()->getDebugdrawer() == 4) {
			_sceneNode->showBoundingBox(true);
		}
	}

	MeshComponent::~MeshComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::SceneManager * sm = _manager->getSceneManager();
		while (!_movableTextObservers.empty()) {
			_parent->deleteMovableText(*_movableTextObservers.begin());
		}
		while (!_boundingBoxObservers.empty()) {
			_parent->removeBoundingBox(*_boundingBoxObservers.begin());
		}

		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));

		_sceneNode->detachObject(entity);
		sm->destroyEntity(entity);

		_parent->getSceneNode()->removeAndDestroyChild(_sceneNode->getName());
	}

	void MeshComponent::updateMeshComponent(const std::string & meshName, const bool isVisible) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::SceneManager * sm = _manager->getSceneManager();
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));

		std::string name = entity->getName();
		_sceneNode->detachObject(entity);
		sm->destroyEntity(entity);

		// create subnode
		entity = sm->createEntity(name, meshName);
		_sceneNode->attachObject(entity);
		entity->setVisible(isVisible);

		unsigned short src, dest;
		if (!entity->getMesh()->suggestTangentVectorBuildParams(Ogre::VertexElementSemantic::VES_TANGENT, src, dest)) {
			entity->getMesh()->buildTangentVectors(Ogre::VertexElementSemantic::VES_TANGENT, src, dest);
		}

		if (api::EngineController::GetSingletonPtr()->getDebugdrawer() == 3 || api::EngineController::GetSingletonPtr()->getDebugdrawer() == 4) {
			_sceneNode->showBoundingBox(true);
		}
	}

	void MeshComponent::setMaterial(const std::string & materialName) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));
		entity->setMaterialName(materialName);
	}

	void MeshComponent::setCustomParameter(uint32_t num, const Vec4 & value) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));
		unsigned int n = entity->getNumSubEntities();
		for (unsigned int i = 0; i < n; ++i) {
			Ogre::SubEntity * pSub = entity->getSubEntity(i);
			pSub->setCustomParameter(num, value.toOgre()); // RGBA
		}
	}

	void MeshComponent::playAnimation(const std::string & anim, bool looping, double offsetPercent) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));

		if (_animationState == nullptr) {
			_parent->addTicker(this);
		} else {
			_animationState->setEnabled(false);
		}

		_animationState = entity->getAnimationState(anim);
		_animationState->setEnabled(true);
		_animationState->setLoop(looping);
		_animationState->setTimePosition(offsetPercent * _animationState->getLength());
		_animationSpeed = 1.0;

		_lastTime = api::EngineController::GetSingleton().getCurrentTime();
	}

	void MeshComponent::stopAnimation() {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_animationState);
		_animationState->setEnabled(false);
		_animationState = nullptr;
		_parent->removeTicker(this);
	}

	void MeshComponent::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_animationState);
		uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
		_animationState->addTime(_animationSpeed * (cT - _lastTime) / 1000000.0);
		_lastTime = cT;
	}

	void MeshComponent::removeMovableTextObserver(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (auto it = _movableTextObservers.begin(); it != _movableTextObservers.end(); it++) {
			if (*it == coid) {
				_movableTextObservers.erase(it);
				break;
			}
		}
	}

	void MeshComponent::removeBoundingBoxObserver(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (auto it = _boundingBoxObservers.begin(); it != _boundingBoxObservers.end(); it++) {
			if (*it == coid) {
				_boundingBoxObservers.erase(it);
				break;
			}
		}
	}

	Ogre::Entity * MeshComponent::getEntity() const {
		ASSERT_THREAD_SAFETY_FUNCTION
		return dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));
	}

} /* namespace modules */
} /* namespace i6engine */
