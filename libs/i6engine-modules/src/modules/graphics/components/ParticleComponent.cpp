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

#include "i6engine/modules/graphics/components/ParticleComponent.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreSceneManager.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"

namespace i6e {
namespace modules {

	ParticleComponent::ParticleComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & emitterName, const Vec3 & pos, const Vec3 & scale) : _manager(manager), _parent(parent), _sceneNode(nullptr), _scale(scale) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name, pos.toOgre());
		ParticleUniverse::ParticleSystem * particleSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(name, emitterName, sm);
		_sceneNode->attachObject(particleSystem);
		particleSystem->setScale(_sceneNode->_getDerivedScale() * _scale.toOgre());
		particleSystem->start();
	}

	ParticleComponent::~ParticleComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ParticleUniverse::ParticleSystem * part = dynamic_cast<ParticleUniverse::ParticleSystem *>(_sceneNode->getAttachedObject(0));
		_sceneNode->detachObject(part);
		part->stop();

		_parent->getSceneNode()->removeAndDestroyChild(_sceneNode->getName());
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(part, _manager->getSceneManager());
	}

	void ParticleComponent::particleFadeOut() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ParticleUniverse::ParticleSystem * part = dynamic_cast<ParticleUniverse::ParticleSystem *>(_sceneNode->getAttachedObject(0));
		if (part == nullptr) {
			ISIXE_THROW_FAILURE("ParticleComponent", "Particle system broken, can't fade out");
		}
		part->stopFade();
	}

	void ParticleComponent::setScale(const Vec3 & scale) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_scale = scale;
		ParticleUniverse::ParticleSystem * part = dynamic_cast<ParticleUniverse::ParticleSystem *>(_sceneNode->getAttachedObject(0));
		if (part == nullptr) {
			ISIXE_THROW_FAILURE("ParticleComponent", "Particle system broken, can't fade out");
		}
		part->setScale(_sceneNode->_getDerivedScale() * _scale.toOgre());
	}

	void ParticleComponent::updateScale() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ParticleUniverse::ParticleSystem * part = dynamic_cast<ParticleUniverse::ParticleSystem *>(_sceneNode->getAttachedObject(0));
		if (part == nullptr) {
			ISIXE_THROW_FAILURE("ParticleComponent", "Particle system broken, can't fade out");
		}
		part->setScale(_sceneNode->_getDerivedScale() * _scale.toOgre());
	}

} /* namespace modules */
} /* namespace i6e */
