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

#include "i6engine/modules/graphics/components/MeshComponent.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreBone.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSubEntity.h"

namespace i6e {
namespace modules {

	MeshComponent::MeshComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & meshName, const bool visible, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) : _manager(manager), _parent(parent), _sceneNode(nullptr), _animationState(nullptr), _animationSpeed(1.0), _lastTime(), _lastFrameTime(0), _movableTextObservers(), _boundingBoxObservers(), _attachedNodes(), _queueA(), _queueB(), _meshComponent(), _shadowCasting(true) {
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

		for (const std::pair<GraphicsNode *, std::string> & p : _attachedNodes) {
			p.first->stopListenAttachment();
			_parent->removeTicker(this);
		}

		auto go = api::EngineController::GetSingleton().getObjectFacade()->getObject(goid);

		if (go != nullptr) {
			_meshComponent = go->getGOC<api::MeshAppearanceComponent>(api::components::ComponentTypes::MeshAppearanceComponent);
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
		entity->setCastShadows(true);

		entity->getMesh()->buildEdgeList();

		unsigned short src, dest;
		if (!entity->getMesh()->suggestTangentVectorBuildParams(Ogre::VertexElementSemantic::VES_TANGENT, src, dest)) {
			entity->getMesh()->buildTangentVectors(Ogre::VertexElementSemantic::VES_TANGENT, src, dest);
		}

		if (api::EngineController::GetSingletonPtr()->getDebugdrawer() == 3 || api::EngineController::GetSingletonPtr()->getDebugdrawer() == 4) {
			_sceneNode->showBoundingBox(true);
		}
	}

	void MeshComponent::setVisibility(bool visible) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));
		entity->setVisible(visible);
	}

	void MeshComponent::setShadowCasting(bool enabled) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));
		entity->setCastShadows(enabled);
		_shadowCasting = enabled;
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
		_lastFrameTime = 0;

		while (!_queueA.empty()) {
			_queueA.pop();
		}
		while (!_queueB.empty()) {
			_queueB.pop();
		}
	}

	void MeshComponent::stopAnimation() {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_animationState);
		_animationState->setEnabled(false);
		_animationState = nullptr;
		_parent->removeTicker(this);
		while (!_queueA.empty()) {
			_queueA.pop();
		}
		while (!_queueB.empty()) {
			_queueB.pop();
		}
	}

	void MeshComponent::addAnimationFrameEvent(uint64_t frameTime, const std::function<void(void)> & func) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (frameTime >= uint64_t(_animationState->getTimePosition() * 1000000)) {
			_queueA.push(std::make_pair(frameTime, func));
		} else {
			_queueB.push(std::make_pair(frameTime, func));
		}
	}

	void MeshComponent::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
		if (_animationState) {
			_animationState->addTime(_animationSpeed * (cT - _lastTime) / 1000000.0);
		}
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0));
		std::map<std::string, api::Transform> boneTransforms;
		for (auto it = entity->getSkeleton()->getBoneIterator().begin(); it != entity->getSkeleton()->getBoneIterator().end(); it++) {
			std::string name = (*it)->getName();
			Ogre::Vector3 skeletonSpacePos = (*it)->_getDerivedPosition();
			Vec3 pos(_sceneNode->convertLocalToWorldPosition(skeletonSpacePos));
			Ogre::Quaternion skeletonSpaceOrient = (*it)->_getDerivedOrientation();
			Quaternion rot(_sceneNode->convertLocalToWorldOrientation(skeletonSpaceOrient));
			boneTransforms.insert(std::make_pair(name, api::Transform(pos, rot)));
		}
		if (!boneTransforms.empty()) {
			auto mesh = _meshComponent.get();
			if (mesh != nullptr) {
				mesh->updateBoneTransforms(boneTransforms);
			}
		}
		for (const std::pair<GraphicsNode *, std::string> & p : _attachedNodes) {
			Ogre::Vector3 newPos = _parent->getSceneNode()->getPosition() + _sceneNode->getPosition() + entity->getSkeleton()->getBone(p.second)->getPosition();
			Ogre::Quaternion newRot = _parent->getSceneNode()->getOrientation() * _sceneNode->getOrientation() * entity->getSkeleton()->getBone(p.second)->getOrientation();
			p.first->getSceneNode()->setPosition(newPos);
			p.first->getSceneNode()->setOrientation(newRot);
			api::GOPtr go = p.first->_go.get();
			if (go != nullptr) {
				auto psc = go->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
				if (psc == nullptr) {
					auto ssc = go->getGOC<api::StaticStateComponent>(api::components::ComponentTypes::StaticStateComponent);
					if (ssc != nullptr) {
						ssc->setPosition(Vec3(newPos));
						ssc->setRotation(Quaternion(newRot));
					}
				} else {
					psc->setPosition(Vec3(newPos), 2);
					psc->setRotation(Quaternion(newRot), 2);
				}
			}
		}
		while (!_queueA.empty() && (uint64_t(_animationState->getTimePosition() * 1000000) >= _queueA.top().first || uint64_t(_animationState->getTimePosition() * 1000000) < _lastFrameTime)) {
			_queueA.top().second();
			_queueB.push(_queueA.top());
			_queueA.pop();
		}
		if (uint64_t(_animationState->getTimePosition() * 1000000) < _lastFrameTime) {
			_queueA = _queueB;
			while (!_queueB.empty()) {
				_queueB.pop();
			}
		}
		_lastFrameTime = uint64_t(_animationState->getTimePosition() * 1000000);
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

	void MeshComponent::attachToBone(GraphicsNode * otherNode, const std::string & boneName) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_attachedNodes[otherNode] = boneName;
		_parent->addTicker(this);
	}

	void MeshComponent::detachFromBone(GraphicsNode * otherNode) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_attachedNodes.erase(otherNode);
		_parent->removeTicker(this);
	}

} /* namespace modules */
} /* namespace i6e */
