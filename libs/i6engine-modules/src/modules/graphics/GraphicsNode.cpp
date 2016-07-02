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

#include "i6engine/modules/graphics/GraphicsNode.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/components/BillboardComponent.h"
#include "i6engine/modules/graphics/components/BoundingBoxComponent.h"
#include "i6engine/modules/graphics/components/CameraComponent.h"
#include "i6engine/modules/graphics/components/LineComponent.h"
#include "i6engine/modules/graphics/components/LuminousComponent.h"
#include "i6engine/modules/graphics/components/MeshComponent.h"
#include "i6engine/modules/graphics/components/MovableTextComponent.h"

#ifdef ISIXE_WITH_PARTICLEUNIVERSE
	#include "i6engine/modules/graphics/components/ParticleComponent.h"
#endif

#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	void GraphicsNode::addTicker(MeshComponent * mesh) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_tickingMeshes.push_back(mesh);
		if (!_ticking) {
			_manager->addTicker(this);
			_ticking = true;
		}
	}

	void GraphicsNode::removeTicker(MeshComponent * mesh) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (size_t i = 0; i < _tickingMeshes.size(); i++) {
			if (_tickingMeshes[i] == mesh) {
				_tickingMeshes.erase(_tickingMeshes.begin() + int(i));
				break;
			}
		}
		_ticking = !_tickingMovableTexts.empty() || !_tickingMeshes.empty();
		if (!_ticking) {
			_manager->removeTicker(this);
		}
	}

	void GraphicsNode::addTicker(MovableTextComponent * movableText) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_tickingMovableTexts.push_back(movableText);
		if (!_ticking) {
			_manager->addTicker(this);
			_ticking = true;
		}
	}

	void GraphicsNode::removeTicker(MovableTextComponent * movableText) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (size_t i = 0; i < _tickingMovableTexts.size(); i++) {
			if (_tickingMovableTexts[i] == movableText) {
				_tickingMovableTexts.erase(_tickingMovableTexts.begin() + int(i));
				break;
			}
		}
		_ticking = !_tickingMovableTexts.empty() || !_tickingMeshes.empty();
		if (!_ticking) {
			_manager->removeTicker(this);
		}
	}

	GraphicsNode::GraphicsNode(GraphicsManager * manager, const int64_t goid, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) : _manager(manager), _gameObjectID(goid), _sceneNode(nullptr), _cameras(), _lights(), _particles(), _meshes(), _billboardSets(), _movableTexts(), _boundingBoxes(), _lines(), _attachedTo(nullptr), _attachedCoid(), _attachedBone(), _go(), _ticking(false), _tickingMeshes(), _tickingMovableTexts() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		Ogre::SceneNode * root = sm->getRootSceneNode();

		std::stringstream name;
		name << "SN_" << goid;

		_sceneNode = root->createChildSceneNode(name.str(), position.toOgre());
		_sceneNode->setOrientation(rotation.toOgre());
		_sceneNode->setScale(scale.toOgre());

		auto go = i6eObjectFacade->getObject(goid);

		if (go != nullptr) {
			_go = go;
		}
	}

	GraphicsNode::~GraphicsNode() {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_ticking) {
			_manager->removeTicker(this);
		}

		Ogre::SceneManager * sm = _manager->getSceneManager();

		Ogre::SceneNode * root = sm->getRootSceneNode();
		for (const std::pair<int64_t, CameraComponent *> & cam : _cameras) {
			delete cam.second;
		}
		_cameras.clear();
		for (const std::pair<int64_t, MeshComponent *> & mesh : _meshes) {
			delete mesh.second;
		}
		_meshes.clear();
		for (const std::pair<int64_t, LuminousComponent *> & light : _lights) {
			delete light.second;
		}
		_lights.clear();

#ifdef ISIXE_WITH_PARTICLEUNIVERSE
		for (const std::pair<int64_t, ParticleComponent *> & part : _particles) {
			delete part.second;
		}
		_particles.clear();
#endif

		for (const std::pair<int64_t, BillboardComponent *> & bill : _billboardSets) {
			delete bill.second;
		}
		_billboardSets.clear();

		root->removeChild(_sceneNode);
		sm->destroySceneNode(_sceneNode->getName());

		if (_attachedTo) {
			_attachedTo->detachFromBone(_attachedCoid, this, _attachedBone);
			_attachedTo = nullptr;
		}
	}

	void GraphicsNode::createMeshComponent(const int64_t coid, const std::string & meshName, const bool visible, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) == _meshes.end());
		MeshComponent * mc = new MeshComponent(_manager, this, _gameObjectID, coid, meshName, visible, position, rotation, scale);
		_meshes.insert(std::make_pair(coid, mc));
		assert(_meshes.find(coid) != _meshes.end());
	}

	void GraphicsNode::updateMeshComponent(const int64_t coid, const std::string & meshName, const bool isVisible) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->updateMeshComponent(meshName, isVisible);
	}

	void GraphicsNode::updateMeshComponentVisibility(const int64_t coid, const bool isVisible) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->setVisibility(isVisible);
	}

	void GraphicsNode::updateMeshComponentShadowCasting(const int64_t coid, const bool enabled) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->setShadowCasting(enabled);
	}

	void GraphicsNode::setMaterial(const int64_t coid, const std::string & materialName) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->setMaterial(materialName);
	}

	void GraphicsNode::setCustomParameter(unsigned int num, const Vec4 & value) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (const std::pair<int64_t, MeshComponent *> & mesh : _meshes) { // FIXME: (Michael) custom parameter is currently applied to all subentities(ogre) in every submesh(engine)
			mesh.second->setCustomParameter(num, value);
		}
	}

	void GraphicsNode::playAnimation(int64_t coid, const std::string & anim, bool looping, double offsetPercent) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->playAnimation(anim, looping, offsetPercent);
		assert(_ticking);
		assert(!_tickingMeshes.empty());
	}

	void GraphicsNode::setAnimationSpeed(int64_t coid, double animationSpeed) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->setAnimationSpeed(animationSpeed);
	}

	void GraphicsNode::stopAnimation(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->stopAnimation();
	}

	void GraphicsNode::addAnimationFrameEvent(int64_t coid, uint64_t frameTime, const std::function<void(void)> & func) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->addAnimationFrameEvent(frameTime, func);
	}

	void GraphicsNode::deleteMeshComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		_meshes.erase(coid);
		delete mc;
		assert(_meshes.find(coid) == _meshes.end());
	}

	void GraphicsNode::createCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_cameras.find(coid) == _cameras.end());
		CameraComponent * cc = new CameraComponent(_manager, this, _gameObjectID, coid, position, lookAt, nC, aspect, fov);
		_cameras.insert(std::make_pair(coid, cc));
		assert(_cameras.find(coid) != _cameras.end());
	}

	void GraphicsNode::updateCameraComponent(const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_cameras.find(coid) != _cameras.end());
		CameraComponent * cc = _cameras[coid];
		cc->updateCameraComponent(position, lookAt, nC, aspect, fov);
	}

	void GraphicsNode::updateCameraFrustumComponent(const int64_t coid, const double left, const double right, const double top, const double bottom) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_cameras.find(coid) != _cameras.end());
		CameraComponent * cc = _cameras[coid];
		cc->updateCameraFrustumComponent(left, right, top, bottom);
	}

	void GraphicsNode::enableCompositor(int64_t coid, const std::string & compositor, bool enabled) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_cameras.find(coid) != _cameras.end());
		CameraComponent * cc = _cameras[coid];
		cc->enableCompositor(compositor, enabled);
	}

	void GraphicsNode::createOrUpdateViewport(const int64_t coid, int zOrder, const double left, const double top, const double width, const double height, const double red, const double green, const double blue, const double alpha) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_cameras.find(coid) != _cameras.end());
		CameraComponent * cc = _cameras[coid];
		cc->createOrUpdateViewport(zOrder, left, top, width, height, red, green, blue, alpha);
	}

	void GraphicsNode::deleteCameraComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_cameras.find(coid) != _cameras.end());
		CameraComponent * cc = _cameras[coid];
		_cameras.erase(coid);
		delete cc;
		assert(_cameras.find(coid) == _cameras.end());
	}

	void GraphicsNode::createLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lights.find(coid) == _lights.end());
		LuminousComponent * lc = new LuminousComponent(_manager, this, _gameObjectID, coid, type, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
		_lights.insert(std::make_pair(coid, lc));
		assert(_lights.find(coid) != _lights.end());
	}

	void GraphicsNode::updateLuminousComponent(const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lights.find(coid) != _lights.end());
		LuminousComponent * lc = _lights[coid];
		lc->updateLuminousComponent(type, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
	}

	void GraphicsNode::deleteLuminousComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lights.find(coid) != _lights.end());
		LuminousComponent * lc = _lights[coid];
		_lights.erase(coid);
		delete lc;
		assert(_lights.find(coid) == _lights.end());
	}

	void GraphicsNode::createParticleComponent(const int64_t coid, const std::string & emitterName, const Vec3 & pos, const Vec3 & scale) {
		ASSERT_THREAD_SAFETY_FUNCTION
#ifdef ISIXE_WITH_PARTICLEUNIVERSE
		assert(_particles.find(coid) == _particles.end());
		ParticleComponent * pc = new ParticleComponent(_manager, this, _gameObjectID, coid, emitterName, pos, scale);
		_particles.insert(std::make_pair(coid, pc));
		assert(_particles.find(coid) != _particles.end());
#endif
	}

	void GraphicsNode::particleFadeOut(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
#ifdef ISIXE_WITH_PARTICLEUNIVERSE
		assert(_particles.find(coid) != _particles.end());
		_particles[coid]->particleFadeOut();
#endif
	}

	void GraphicsNode::deleteParticleComponent(const int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
#ifdef ISIXE_WITH_PARTICLEUNIVERSE
		assert(_particles.find(coid) != _particles.end());
		ParticleComponent * pc = _particles[coid];
		_particles.erase(coid);
		delete pc;
		assert(_particles.find(coid) == _particles.end());
#endif
	}

	void GraphicsNode::createBilldboardSetComponent(int64_t coid, const std::string & material, double width, double height, api::graphics::BillboardOrigin bo) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_billboardSets.find(coid) == _billboardSets.end());
		BillboardComponent * bc = new BillboardComponent(_manager, this, _gameObjectID, coid, material, width, height, bo);
		_billboardSets.insert(std::make_pair(coid, bc));
		assert(_billboardSets.find(coid) != _billboardSets.end());
	}

	void GraphicsNode::createOrUpdateBillboard(int64_t coid, const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_billboardSets.find(coid) != _billboardSets.end());
		BillboardComponent * bc = _billboardSets[coid];
		bc->createOrUpdateBillboard(identifier, offset, width, height, u0, v0, u1, v1);
	}

	void GraphicsNode::deleteBillboard(int64_t coid, const std::string & identifier) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_billboardSets.find(coid) != _billboardSets.end());
		BillboardComponent * bc = _billboardSets[coid];
		bc->deleteBillboard(identifier);
	}

	void GraphicsNode::deleteBillboardSetComponent(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_billboardSets.find(coid) != _billboardSets.end());
		BillboardComponent * bc = _billboardSets[coid];
		_billboardSets.erase(coid);
		delete bc;
		assert(_billboardSets.find(coid) == _billboardSets.end());
	}

	void GraphicsNode::createMovableText(int64_t coid, const std::string & font, const std::string & text, double size, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_movableTexts.find(coid) == _movableTexts.end());
		MovableTextComponent * mtc = new MovableTextComponent(_manager, this, _gameObjectID, coid, font, text, size, colour);
		_movableTexts.insert(std::make_pair(coid, mtc));
		assert(_movableTexts.find(coid) != _movableTexts.end());
	}

	void GraphicsNode::updateMovableText(int64_t coid, const std::string & font, const std::string & text, double size, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_movableTexts.find(coid) != _movableTexts.end());
		MovableTextComponent * mtc = _movableTexts[coid];
		mtc->updateMovableText(font, text, size, colour);
	}

	void GraphicsNode::updateMovableTextSetAutoScaleCallback(int64_t coid, const std::function<double(const Vec3 &, const Vec3 &)> & callback) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_movableTexts.find(coid) != _movableTexts.end());
		MovableTextComponent * mtc = _movableTexts[coid];
		mtc->setAutoScaleCallback(callback);
	}

	void GraphicsNode::deleteMovableText(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_movableTexts.find(coid) != _movableTexts.end());
		_manager->removeTicker(this);
		delete _movableTexts[coid];
		_movableTexts.erase(coid);
		assert(_movableTexts.find(coid) == _movableTexts.end());
	}

	void GraphicsNode::drawBoundingBox(int64_t coid, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_boundingBoxes.find(coid) == _boundingBoxes.end());
		assert(_meshes.find(coid) != _meshes.end());
		BoundingBoxComponent * bbc = new BoundingBoxComponent(_manager, this, _gameObjectID, coid, _meshes[coid], colour);
		_boundingBoxes.insert(std::make_pair(coid, bbc));
		assert(_boundingBoxes.find(coid) != _boundingBoxes.end());
	}

	void GraphicsNode::removeBoundingBox(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_boundingBoxes.find(coid) != _boundingBoxes.end());
		BoundingBoxComponent * bbc = _boundingBoxes[coid];
		_boundingBoxes.erase(coid);
		delete bbc;
		assert(_boundingBoxes.find(coid) == _boundingBoxes.end());
	}

	void GraphicsNode::createLine(int64_t coid, const Vec3 & from, const Vec3 & to, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_lines.find(coid) == _lines.end());
		LineComponent * lc = new LineComponent(_manager, this, _gameObjectID, coid, from, to, colour);
		_lines.insert(std::make_pair(coid, lc));
		assert(_lines.find(coid) != _lines.end());
	}

	void GraphicsNode::removeLine(int64_t coid) {
		ASSERT_THREAD_SAFETY_FUNCTION
			assert(_lines.find(coid) != _lines.end());
		LineComponent * lc = _lines[coid];
		_lines.erase(coid);
		delete lc;
		assert(_lines.find(coid) == _lines.end());
	}

	void GraphicsNode::attachToBone(int64_t coid, GraphicsNode * otherNode, const std::string & boneName) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->attachToBone(otherNode, boneName);
	}

	void GraphicsNode::detachFromBone(int64_t coid, GraphicsNode * otherNode, const std::string & boneName) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_meshes.find(coid) != _meshes.end());
		MeshComponent * mc = _meshes[coid];
		mc->detachFromBone(otherNode);
	}

	void GraphicsNode::listenAttachment(GraphicsNode * otherNode, int64_t coid, const std::string & boneName) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_attachedTo = otherNode;
		_attachedCoid = coid;
		_attachedBone = boneName;
	}

	void GraphicsNode::stopListenAttachment() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_attachedTo = nullptr;
	}

	void GraphicsNode::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (MeshComponent * mc : _tickingMeshes) {
			mc->Tick();
		}
		for (MovableTextComponent * mtc : _tickingMovableTexts) {
			mtc->Tick();
		}
	}

} /* namespace modules */
} /* namespace i6e */
