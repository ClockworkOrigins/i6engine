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

#include "i6engine/modules/graphics/components/ParticleComponent.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreSceneManager.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"

namespace i6engine {
namespace modules {

	ParticleComponent::ParticleComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & emitterName, const Vec3 & pos) : _manager(manager), _parent(parent), _sceneNode(nullptr) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name, pos.toOgre());
		ParticleUniverse::ParticleSystem * particleSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(name, emitterName, sm);
		_sceneNode->attachObject(particleSystem);
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

} /* namespace modules */
} /* namespace i6engine */
