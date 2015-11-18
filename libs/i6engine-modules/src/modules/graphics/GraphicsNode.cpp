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

#include "i6engine/modules/graphics/GraphicsNode.h"

#include <algorithm>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GraphicsConfig.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/components/LuminousComponent.h"
#include "i6engine/modules/graphics/components/ParticleComponent.h"
#include "i6engine/modules/graphics/graphicswidgets/MovableText.h"

#include "OGRE/OgreBillboard.h"
#include "OGRE/OgreBillboardSet.h"
#include "OGRE/OgreCamera.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSubEntity.h"

namespace i6engine {
namespace modules {

	GraphicsNode::GraphicsNode(GraphicsManager * manager, const int64_t goid, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) : _manager(manager), _gameObjectID(goid), _sceneNode(nullptr), _parentNode(nullptr), _cameras(), _lights(), _particles(), _sceneNodes(), _animationState(), _animationSpeed(1.0), _lastTime(), _billboardSets(), _movableTexts(), _observer(), _boundingBox() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		Ogre::SceneManager * sm = _manager->getSceneManager();

		Ogre::SceneNode * root = sm->getRootSceneNode();

		std::stringstream name;
		name << "SN_" << goid;

		_sceneNode = root->createChildSceneNode(name.str(), position.toOgre());
		_sceneNode->setOrientation(rotation.toOgre());
		_sceneNode->setScale(scale.toOgre());
	}

	GraphicsNode::~GraphicsNode() {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_animationState) {
			_manager->removeTicker(this);
		}

		Ogre::SceneManager * sm = _manager->getSceneManager();

		Ogre::SceneNode * root = sm->getRootSceneNode();
		if (_parentNode != nullptr) {
			delete _parentNode;
		}
		for (const std::pair<int64_t, Ogre::SceneNode *> & cam : _cameras) {
			deleteCameraComponent(cam.first);
		}
		for (const std::pair<int64_t, Ogre::SceneNode *> & sns : _sceneNodes) {
			deleteMeshComponent(sns.first);
		}
		for (const std::pair<int64_t, LuminousComponent *> & light : _lights) {
			delete light.second;
		}
		_lights.clear();
		for (const std::pair<int64_t, ParticleComponent *> & part : _particles) {
			delete part.second;
		}
		_particles.clear();
		for (const std::pair<int64_t, MovableText *> & text : _movableTexts) {
			deleteMovableText(text.first);
		}
		if (_boundingBox) {
			removeBoundingBox();
		}

		root->removeChild(_sceneNode);

		if (_sceneNode != nullptr) {
			sm->destroySceneNode(_sceneNode->getName());
		}
	}

	// to be reviewed
	void GraphicsNode::setParent(GraphicsNode * newParent) {
		ASSERT_THREAD_SAFETY_FUNCTION

		// Remove
		if (_parentNode == nullptr) {
			Ogre::SceneManager * sm = _manager->getSceneManager();
			sm->getRootSceneNode()->removeChild(_sceneNode);
		} else {
			_parentNode->getSceneNode()->removeChild(_sceneNode);
		}

		// Add
		if (newParent != nullptr) {
			newParent->getSceneNode()->addChild(_sceneNode);
			_parentNode = newParent;
		} else {
			// If nullptr, attach to root node.
			Ogre::SceneManager * sm = _manager->getSceneManager();
			sm->getRootSceneNode()->addChild(_sceneNode);
		}
	}

	Ogre::SceneNode * GraphicsNode::getOrCreateSceneNode(const int64_t coid, const Vec3 & pos, const Quaternion & rot, const Vec3 & scale) {
		if (_sceneNodes.find(coid) == _sceneNodes.end()) {
			Ogre::SceneNode * newNode = _sceneNode->createChildSceneNode("SN_" + std::to_string(_gameObjectID)  + "_" + std::to_string(coid), pos.toOgre());
			newNode->setOrientation(rot.toOgre());
			newNode->setScale(scale.toOgre());
			_sceneNodes[coid] = newNode;
			return newNode;
		}
		return _sceneNodes[coid];
	}

	// to be reviewed
	void GraphicsNode::createMeshComponent(const int64_t coid, const std::string & meshName, const bool isVisible) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneManager * sm = _manager->getSceneManager();
		std::stringstream s;
		s << "mesh" << _gameObjectID << "_" << coid;

		// create subnode
		Ogre::Entity * meshEntity = sm->createEntity(s.str(), meshName);
		meshEntity->setVisible(isVisible);
		meshEntity->setCastShadows(true);
		_sceneNodes[coid]->attachObject(meshEntity);

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
			_sceneNodes[coid]->showBoundingBox(true);
		}
	}

	// to be reviewed
	void GraphicsNode::updateMeshComponent(const int64_t coid, const std::string & meshName, const bool isVisible) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneManager * sm = _manager->getSceneManager();
		Ogre::SceneNode * sn = _sceneNodes[coid];
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(sn->getAttachedObject(0));

		sn->detachObject(entity);
		sm->destroyEntity(entity);

		std::stringstream s;
		s << "mesh" << _gameObjectID << "_" << coid;

		// create subnode
		entity = sm->createEntity(s.str(), meshName);
		sn->attachObject(entity);
		entity->setVisible(isVisible);

		unsigned short src, dest;
		if (!entity->getMesh()->suggestTangentVectorBuildParams(Ogre::VertexElementSemantic::VES_TANGENT, src, dest)) {
			entity->getMesh()->buildTangentVectors(Ogre::VertexElementSemantic::VES_TANGENT, src, dest);
		}

		if (api::EngineController::GetSingletonPtr()->getDebugdrawer() == 3 || api::EngineController::GetSingletonPtr()->getDebugdrawer() == 4) {
			_sceneNodes[coid]->showBoundingBox(true);
		}
	}

	void GraphicsNode::createCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, const double fov) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::stringstream name;
		name << "SN_" << _gameObjectID << "_" << coid;

		Ogre::SceneNode * cameraNode = _sceneNode->createChildSceneNode(name.str());
		std::stringstream name2;
		name2 << "camera" << _gameObjectID;

		Ogre::Camera * camera = sm->createCamera(name2.str());
		cameraNode->attachObject(camera);

		camera->setPosition(position.toOgre());
		camera->lookAt(lookAt.toOgre());
		camera->setNearClipDistance(nC);
		camera->setFOVy(Ogre::Radian(fov));

		_cameras[coid] = cameraNode;
	}

	void GraphicsNode::updateCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, const double fov) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneNode * sn = _cameras[coid];
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(sn->getAttachedObject(0));

		camera->setPosition(position.toOgre());
		camera->lookAt(lookAt.toOgre());
		camera->setNearClipDistance(nC);
		camera->setFOVy(Ogre::Radian(fov));
	}

	void GraphicsNode::updateCameraFrustumComponent(const int64_t coid, const double left, const double right, const double top, const double bottom) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneNode * sn = _cameras[coid];
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(sn->getAttachedObject(0));
		camera->setFrustumExtents(left, right, top, bottom);
	}

	void GraphicsNode::createOrUpdateViewport(const int64_t coid, const double left, const double top, const double width, const double height, const double red, const double green, const double blue, const double alpha) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::Viewport * vp;

		Ogre::SceneNode * sn = _cameras[coid];
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(sn->getAttachedObject(0));
		if (camera->getViewport() == nullptr) {
			vp = _manager->getRoot()->getAutoCreatedWindow()->addViewport(camera, int(coid), float(left), float(top), float(width), float(height));
		} else {
			vp = camera->getViewport();

			vp->setDimensions(left, top, width, height);
		}

		vp->setBackgroundColour(Ogre::ColourValue(float(red), float(green), float(blue), float(alpha)));

		Ogre::CompositorManager::ResourceMapIterator resourceIterator = Ogre::CompositorManager::getSingleton().getResourceIterator();

		// add all compositor resources to the view container
		while (resourceIterator.hasMoreElements()) {
			Ogre::ResourcePtr resource = resourceIterator.getNext();
			const Ogre::String & compositorName = resource->getName();

			int addPosition = -1;
			if (compositorName == "HDR") {
				// HDR must be first in the chain
				addPosition = 0;
			}
			try {
				Ogre::CompositorManager::getSingleton().addCompositor(vp, compositorName, addPosition);
				Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName, false);
			} catch (...) {
				/// Warn user
				Ogre::LogManager::getSingleton().logMessage("Could not load compositor " + compositorName);
			}
		}
	}

	void GraphicsNode::setMaterial(const int64_t coid, const std::string & materialName) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneNode * sn = _sceneNodes[coid];
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(sn->getAttachedObject(0));
		entity->setMaterialName(materialName);
	}

	void GraphicsNode::setCustomParameter(unsigned int num, const Vec4 & value) {
		ASSERT_THREAD_SAFETY_FUNCTION

		for (const std::pair<int64_t, Ogre::SceneNode *> & sn : _sceneNodes) { // FIXME: (Michael) custom parameter is currently applied to all subentities(ogre) in every submesh(engine)
			Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(sn.second->getAttachedObject(0));
			unsigned int n = entity->getNumSubEntities();
			for (unsigned int i = 0; i < n; ++i) {
				Ogre::SubEntity* pSub = entity->getSubEntity(i);
				pSub->setCustomParameter(num, value.toOgre()); // RGBA
			}
		}
	}

	void GraphicsNode::createLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lights.find(coid) == _particles.end());
		LuminousComponent * lc = new LuminousComponent(_manager, this, _gameObjectID, coid, type, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
		_lights.insert(std::make_pair(coid, lc));
		assert(_lights.find(coid) != _particles.end());
	}

	void GraphicsNode::updateLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lights.find(coid) != _particles.end());
		LuminousComponent * lc = _lights[coid];
		lc->updateLuminousComponent(type, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
	}

	void GraphicsNode::deleteLuminousComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lights.find(coid) != _particles.end());
		LuminousComponent * lc = _lights[coid];
		_lights.erase(coid);
		delete lc;
		assert(_lights.find(coid) == _particles.end());
	}

	void GraphicsNode::createParticleComponent(const int64_t coid, const std::string & emitterName, const Vec3 & pos) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_particles.find(coid) == _particles.end());
		ParticleComponent * pc = new ParticleComponent(_manager, this, _gameObjectID, coid, emitterName, pos);
		_particles.insert(std::make_pair(coid, pc));
		assert(_particles.find(coid) != _particles.end());
	}

	void GraphicsNode::particleFadeOut(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_particles.find(coid) != _particles.end());
		_particles[coid]->particleFadeOut();
	}

	void GraphicsNode::deleteParticleComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_particles.find(coid) != _particles.end());
		ParticleComponent * pc = _particles[coid];
		_particles.erase(coid);
		delete pc;
		assert(_particles.find(coid) == _particles.end());
	}

	void GraphicsNode::deleteCameraComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneManager * sm = _manager->getSceneManager();
		Ogre::SceneNode * sn = _cameras[coid];
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(sn->getAttachedObject(0));
		Ogre::Viewport * vp = camera->getViewport();

		Ogre::CompositorManager::ResourceMapIterator resourceIterator = Ogre::CompositorManager::getSingleton().getResourceIterator();

		// add all compositor resources to the view container
		while (resourceIterator.hasMoreElements()) {
			Ogre::ResourcePtr resource = resourceIterator.getNext();
			const Ogre::String & compositorName = resource->getName();
			try {
				Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName, false);
				Ogre::CompositorManager::getSingleton().removeCompositor(vp, compositorName);
			} catch (...) {
				/// Warn user
				Ogre::LogManager::getSingleton().logMessage("Could not load compositor " + compositorName);
			}
		}

		_manager->getRoot()->getAutoCreatedWindow()->removeViewport(int(coid));		

		sn->detachObject(camera);
		sm->destroyCamera(camera);

		_sceneNode->removeAndDestroyChild(sn->getName());

		_cameras.erase(coid);
	}

	void GraphicsNode::deleteMeshComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneManager * sm = _manager->getSceneManager();

		if (_sceneNodes.find(coid) == _sceneNodes.end()) { // TODO: (Daniel) fix this, how can this happen? Had a coid with a negative large number
			return;
		}

		auto it = _observer.find(coid);
		if (it != _observer.end()) {
			for (auto & i : it->second) {
				deleteMovableText(i);
			}
			_observer.erase(it);
		}

		Ogre::SceneNode * sn = _sceneNodes[coid];
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(sn->getAttachedObject(0));

		sn->detachObject(entity);
		sm->destroyEntity(entity);

		_sceneNode->removeAndDestroyChild(sn->getName());

		_sceneNodes.erase(coid);
	}

	void GraphicsNode::playAnimation(int64_t coid, const std::string & anim, bool looping, double offsetPercent) {
		ASSERT_THREAD_SAFETY_FUNCTION

		Ogre::SceneNode * sn = _sceneNodes[coid];
		Ogre::Entity * entity = dynamic_cast<Ogre::Entity *>(sn->getAttachedObject(0));

		if (_animationState == nullptr) {
			_manager->addTicker(this);
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

	void GraphicsNode::Tick() {
		if (_animationState) {
			uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
			_animationState->addTime(_animationSpeed * (cT - _lastTime) / 1000000.0);
			_lastTime = cT;
		}
		for (auto & p : _movableTexts) {
			p.second->update();
		}
	}

	void GraphicsNode::stopAnimation() {
		_animationState->setEnabled(false);
		_animationState = nullptr;
		_manager->removeTicker(this);
	}

	void GraphicsNode::createBilldboardSetComponent(int64_t coid, const std::string & material, double width, double height, api::graphics::BillboardOrigin bo) {
		Ogre::BillboardSet * billboardSet = _manager->getSceneManager()->createBillboardSet("SN_" + std::to_string(_gameObjectID) + "_" + std::to_string(coid));
		billboardSet->setMaterialName(material);
		billboardSet->setBillboardType(Ogre::BillboardType::BBT_POINT);
		billboardSet->setDefaultDimensions(width, height);
		switch (bo) {
		case api::graphics::BillboardOrigin::CenterLeft: {
			billboardSet->setBillboardOrigin(Ogre::BillboardOrigin::BBO_CENTER_LEFT);
			break;
		}
		default: {
			ISIXE_THROW_FAILURE("GraphicsNode", "Billboard origin not correct!");
		}
		}
		_sceneNode->attachObject(billboardSet);

		_billboardSets.insert(std::make_pair(coid, std::make_pair(billboardSet, std::map<std::string, Ogre::Billboard *>())));
	}

	void GraphicsNode::createOrUpdateBillboard(int64_t coid, const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1) {
		Ogre::Billboard * bb = nullptr;
		if (_billboardSets.find(coid) == _billboardSets.end()) {
			return;
		}
		if (_billboardSets[coid].second.find(identifier) == _billboardSets[coid].second.end()) {
			bb = _billboardSets[coid].first->createBillboard(offset.toOgre(), Ogre::ColourValue::White);
			_billboardSets[coid].second.insert(std::make_pair(identifier, bb));
		} else {
			bb = _billboardSets[coid].second[identifier];
		}
		bb->setDimensions(width, height);
		bb->setTexcoordRect(u0, v0, u1, v1);
		bb->setPosition(offset.toOgre());
	}

	void GraphicsNode::deleteBillboard(int64_t coid, const std::string & identifier) {
		Ogre::Billboard * bb = _billboardSets[coid].second[identifier];
		_billboardSets[coid].first->removeBillboard(bb);
		_billboardSets[coid].second.erase(identifier);
		delete bb;
	}

	void GraphicsNode::deleteBillboardSetComponent(int64_t coid) {
		Ogre::BillboardSet * bs = _billboardSets[coid].first;
		_billboardSets.erase(coid);

		_sceneNode->detachObject(bs);
		_manager->getSceneManager()->destroyBillboardSet(bs);
	}

	void GraphicsNode::createMovableText(int64_t coid, int64_t targetID, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour) {
		if (_movableTexts.find(coid) != _movableTexts.end()) {
			return;
		}
		try {
			MovableText * movableText = new MovableText(_sceneNodes[targetID]->getAttachedObject(0), _manager->getSceneManager(), font);
			movableText->setText(text);
			movableText->setSize(size);
			movableText->setColour(colour);
			movableText->enable(true);
			_movableTexts.insert(std::make_pair(coid, movableText));
			_manager->addTicker(this);
			_observer[targetID].push_back(coid);
		} catch (Ogre::Exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	void GraphicsNode::updateMovableText(int64_t coid, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour) {
		MovableText * movableText = _movableTexts[coid];
		movableText->setText(text);
		movableText->setSize(size);
		movableText->setColour(colour);
	}

	void GraphicsNode::deleteMovableText(int64_t coid) {
		_manager->removeTicker(this);
		delete _movableTexts[coid];
		_movableTexts.erase(coid);
	}

	void GraphicsNode::enableCompositor(int64_t coid, const std::string & compositor, bool enabled) {
		Ogre::SceneNode * sn = _cameras[coid];
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(sn->getAttachedObject(0));
		Ogre::Viewport * vp = camera->getViewport();
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositor, enabled);
	}

	void GraphicsNode::drawBoundingBox(int64_t coid, const Vec3 & colour) {
		Ogre::SceneManager * sm = _manager->getSceneManager();
		_boundingBox = sm->createManualObject("MO_" + std::to_string(_gameObjectID));

		// NOTE: The second parameter to the create method is the resource group the material will be added to.
		// If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
		Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("MO_" + std::to_string(_gameObjectID) + "_Material", "General");
		myManualObjectMaterial->setReceiveShadows(false);
		myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true);
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(colour.getX(), colour.getY(), colour.getZ(), 0);
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(colour.getX(), colour.getY(), colour.getZ());
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(colour.getX(), colour.getY(), colour.getZ());


		_boundingBox->begin("MO_" + std::to_string(_gameObjectID) + "_Material", Ogre::RenderOperation::OT_LINE_LIST);

		Ogre::MovableObject * meshEntity = _sceneNodes[coid]->getAttachedObject(0);

		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_BOTTOM));

		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_TOP));

		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_TOP));

		_boundingBox->end();

		_sceneNode->attachObject(_boundingBox);
	}

	void GraphicsNode::removeBoundingBox() {
		_sceneNode->detachObject(_boundingBox);
		Ogre::SceneManager * sm = _manager->getSceneManager();
		sm->destroyManualObject(_boundingBox);
		_boundingBox = nullptr;
	}

} /* namespace modules */
} /* namespace i6engine */
