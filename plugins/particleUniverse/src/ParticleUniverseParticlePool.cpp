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

#include "ParticleUniverseParticlePool.h"

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

#include "OGRE/OgreSceneNode.h"

namespace ParticleUniverse {

	
	ParticlePool::~ParticlePool() {
		// Destroy all visual particles
		destroyAllVisualParticles();

		// Destroy all emitter particles
		destroyAllEmitterParticles();

		// Destroy all technique particles
		destroyAllTechniqueParticles();

		// Destroy all affector particles
		destroyAllAffectorParticles();

		// Destroy all particle system particles
		destroyAllSystemParticles();
	}
	
	void ParticlePool::destroyParticles(const Particle::ParticleType particleType) {
		switch (particleType) {
		case Particle::PT_VISUAL: {
			destroyAllVisualParticles();
			break;
		}
		case Particle::PT_EMITTER: {
			destroyAllEmitterParticles();
			break;
		}
		case Particle::PT_TECHNIQUE: {
			destroyAllTechniqueParticles();
			break;
		}
		case Particle::PT_AFFECTOR: {
			destroyAllAffectorParticles();
			break;
		}
		case Particle::PT_SYSTEM: {
			destroyAllSystemParticles();
			break;
		}
		default: {
			break;
		}
		}
	}
	
	void ParticlePool::destroyAllVisualParticles() {
		for (vector<VisualParticle *>::iterator it = mVisualParticles.begin(); it != mVisualParticles.end(); ++it) {
			PU_DELETE_T(*it, VisualParticle, MEMCATEGORY_SCENE_OBJECTS);
		}
		mVisualParticles.clear();
		mVisualParticlesPool.clear();
	}
	
	void ParticlePool::destroyAllEmitterParticles() {
		for (vector<ParticleEmitter *>::iterator itEmitter = mEmitters.begin(); itEmitter != mEmitters.end(); ++itEmitter) {
			ParticleSystemManager::getSingletonPtr()->destroyEmitter(*itEmitter);
		}
		mEmitters.clear();
		mParticleEmitterPool.clear();
	}
	
	void ParticlePool::destroyAllTechniqueParticles() {
		for (vector<ParticleTechnique *>::iterator itTechnique = mTechniques.begin(); itTechnique != mTechniques.end(); ++itTechnique) {
			ParticleSystemManager::getSingletonPtr()->destroyTechnique(*itTechnique);
		}
		mTechniques.clear();
		mParticleTechniquePool.clear();
	}
	
	void ParticlePool::destroyAllAffectorParticles() {
		for (vector<ParticleAffector *>::iterator itAffector = mAffectors.begin(); itAffector != mAffectors.end(); ++itAffector) {
			ParticleSystemManager::getSingletonPtr()->destroyAffector(*itAffector);
		}
		mAffectors.clear();
		mParticleAffectorPool.clear();
	}
	
	void ParticlePool::destroyAllSystemParticles() {
		// There is still a problem if the scene is cleared and this Particle System isn't deleted yet.
		// mParentTechnique doesn�t always have a parent ParticleSystem; this is in case of an alias technique.
		if (mParentTechnique->getParentSystem()) {
			Ogre::SceneManager * sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
			for (vector<String>::iterator itSystem = mSystems.begin(); itSystem != mSystems.end(); ++itSystem) {
				ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(*itSystem, sceneManager);
			}
		}
		mSystems.clear();
		mParticleSystemPool.clear();
	}
	
	void ParticlePool::setParentTechnique(ParticleTechnique * parentTechnique) {
		mParentTechnique = parentTechnique;
	}
	
	bool ParticlePool::isEmpty() {
		return mVisualParticlesPool.isEmpty() && mParticleEmitterPool.isEmpty() && mParticleTechniquePool.isEmpty() && mParticleAffectorPool.isEmpty() && mParticleSystemPool.isEmpty();
	}
	
	bool ParticlePool::isEmpty(const Particle::ParticleType particleType) {
		switch (particleType) {
		case Particle::PT_VISUAL: {
			return mVisualParticlesPool.isEmpty();
			break;
		}
		case Particle::PT_EMITTER: {
			return mParticleEmitterPool.isEmpty();
			break;
		}
		case Particle::PT_TECHNIQUE: {
			return mParticleTechniquePool.isEmpty();
			break;
		}
		case Particle::PT_AFFECTOR: {
			return mParticleAffectorPool.isEmpty();
			break;
		}
		case Particle::PT_SYSTEM: {
			return mParticleSystemPool.isEmpty();
			break;
		}
		default: {
			break;
		}
		}

		return true;
	}
	
	size_t ParticlePool::getSize() {
		return mVisualParticlesPool.getSize() + mParticleEmitterPool.getSize() + mParticleTechniquePool.getSize() + mParticleAffectorPool.getSize() + mParticleSystemPool.getSize();
	}
	
	size_t ParticlePool::getSize(const Particle::ParticleType particleType) {
		switch (particleType) {
		case Particle::PT_VISUAL: {
			return mVisualParticlesPool.getSize();
			break;
		}
		case Particle::PT_EMITTER: {
			return mParticleEmitterPool.getSize();
			break;
		}
		case Particle::PT_TECHNIQUE: {
			return mParticleTechniquePool.getSize();
			break;
		}
		case Particle::PT_AFFECTOR: {
			return mParticleAffectorPool.getSize();
			break;
		}
		case Particle::PT_SYSTEM: {
			return mParticleSystemPool.getSize();
			break;
		}
		default: {
			break;
		}
		}

		return 0;
	}
	
	void ParticlePool::_increaseVisualParticlePool(size_t size, Particle::ParticleBehaviourList & behaviours) {
		size_t oldSize = mVisualParticles.size();
		if (size < oldSize) {
			return;
		}

		// Create new visual particles
		VisualParticle * particle = nullptr;
		for (size_t i = oldSize; i < size; i++) {
			particle = PU_NEW_T(VisualParticle, MEMCATEGORY_SCENE_OBJECTS)();
			particle->copyBehaviours(behaviours);
			mVisualParticlesPool.addElement(particle);
			mVisualParticles.push_back(particle);
		}
	}
	
	void ParticlePool::_increaseParticleEmitterPool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique) {
		size_t oldSize = mEmitters.size();
		if (size < oldSize) {
			return;
		}

		// Create new emitters, based on the already created emitters in the technique and which are marked for emission.
		size_t numberOfEmittedEmitters = technique->getNumEmittedEmitters();
		if (numberOfEmittedEmitters == 0) {
			return;
		}

		ParticleEmitter * existingEmitter = nullptr;
		ParticleEmitter * clonedEmitter = nullptr;
		size_t numEmitters = technique->getNumEmitters();

		// Distribute size equally
		size_t increment = (size-oldSize) / numberOfEmittedEmitters;

		// Run through emitters of the technique
		for (size_t emitterCount = 0; emitterCount < numEmitters; emitterCount++) {
			existingEmitter = technique->getEmitter(emitterCount);
			if (existingEmitter->_isMarkedForEmission()) {
				// Clone the emitter 'increment' times and add to the pool
				for (size_t i = 0; i < increment; i++) {
					clonedEmitter = ParticleSystemManager::getSingletonPtr()->cloneEmitter(existingEmitter);
					clonedEmitter->_setMarkedForEmission(true);
					clonedEmitter->copyBehaviours(behaviours);
					mParticleEmitterPool.addElement(clonedEmitter->getName(), clonedEmitter);
					mEmitters.push_back(clonedEmitter);

					/** Pooled emitters should be prepared here, because it cannot be done anywhere else 
						because of the limitation of iterating through the pool. Besides, this is faster.
					*/
					clonedEmitter->_prepare(technique);
				}
			}
		}
	}
	
	void ParticlePool::_increaseParticleTechniquePool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleSystem * system) {
		size_t oldSize = mTechniques.size();
		if (size < oldSize) {
			return;
		}

		// Create new techniques, based on the techniques in the particle system and which are marked for emission.
		size_t numberOfEmittedTechniques = system->getNumEmittedTechniques();
		if (numberOfEmittedTechniques == 0) {
			return;
		}

		ParticleTechnique * existingTechnique = nullptr;
		ParticleTechnique * clonedTechnique = nullptr;
		size_t numTechniques = system->getNumTechniques();

		// Distribute size equally
		size_t increment = (size-oldSize) / numberOfEmittedTechniques;

		// Run through techniques of the system
		for (size_t techniqueCount = 0; techniqueCount < numTechniques; techniqueCount++) {
			existingTechnique = system->getTechnique(techniqueCount);
			if (existingTechnique->_isMarkedForEmission()) {
				// Clone the technique 'increment' times and add to the pool
				for (size_t i = 0; i < increment; i++) {
					clonedTechnique = ParticleSystemManager::getSingletonPtr()->cloneTechnique(existingTechnique);
					clonedTechnique->_setMarkedForEmission(true);
					clonedTechnique->copyBehaviours(behaviours);
					mParticleTechniquePool.addElement(clonedTechnique->getName(), clonedTechnique);
					mTechniques.push_back(clonedTechnique);

					/** Important note:
						Calling the _prepare() function of the cloned techniques must not be done here. It is
						handled in the _update() function which is called when processing all particles in the
						pool.
					*/
				}
			}
		}
	}
	
	void ParticlePool::_increaseParticleAffectorPool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique) {
		size_t oldSize = mAffectors.size();
		if (size < oldSize) {
			return;
		}

		// Create new affectors, based on the already created affectors in the technique and which are marked for emission.
		size_t numberOfEmittedAffectors = technique->getNumEmittedAffectors();
		if (numberOfEmittedAffectors == 0) {
			return;
		}

		ParticleAffector * existingAffector = nullptr;
		ParticleAffector * clonedAffector = nullptr;
		size_t numAffectors = technique->getNumAffectors();

		// Distribute size equally
		size_t increment = (size-oldSize) / numberOfEmittedAffectors;

		// Run through affectors of the technique
		for (size_t affectorCount = 0; affectorCount < numAffectors; affectorCount++) {
			existingAffector = technique->getAffector(affectorCount);
			if (existingAffector->_isMarkedForEmission()) {
				// Clone the affector 'increment' times and add to the pool
				for (size_t i = 0; i < increment; i++) {
					clonedAffector = ParticleSystemManager::getSingletonPtr()->cloneAffector(existingAffector);
					clonedAffector->_setMarkedForEmission(true);
					clonedAffector->copyBehaviours(behaviours);
					mParticleAffectorPool.addElement(clonedAffector->getName(), clonedAffector);
					mAffectors.push_back(clonedAffector);

					/** Pooled affectors should be prepared here, because it cannot be done anywhere else 
						because of the limitation of iterating through the pool. Besides, this is faster.
					*/
					clonedAffector->_prepare(technique);
				}
			}
		}
	}
	
	void ParticlePool::_increaseParticleSystemPool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique) {
		size_t oldSize = mSystems.size();
		if (size < oldSize) {
			return;
		}

		// Run through all emitters in the technique and check which one emits a particle system
		size_t numberOfEmitters = technique->getNumEmitters();
		if (numberOfEmitters == 0) {
			return;
		}

		ParticleSystem * system = nullptr;
		ParticleSystem * clonedSystem = nullptr;
		ParticleEmitter * emitter = nullptr;
		size_t numberOfEmittedSystems = 0;
		size_t systemCount = 0;

		// First determine the number of emitted particle systems.
		for (systemCount = 0; systemCount < numberOfEmitters; systemCount++) {
			emitter = technique->getEmitter(systemCount);
			if (emitter->getEmitsType() == Particle::PT_SYSTEM) {
				numberOfEmittedSystems++;
			}
		}

		if (numberOfEmittedSystems == 0) {
			return;
		}

		// Distribute size equally
		size_t increment = (size-oldSize) / numberOfEmittedSystems;

		// Fill the pool
		for (systemCount = 0; systemCount < numberOfEmitters; systemCount++) {
			emitter = technique->getEmitter(systemCount);
			if (emitter->getEmitsType() == Particle::PT_SYSTEM) {
				system = ParticleSystemManager::getSingletonPtr()->getParticleSystemTemplate(emitter->getEmitsName());
				if (system) {
					// Create 'increment' ParticleSystems and add to the pool
					std::stringstream ss; 
					ss << this;
					for (size_t i = 0; i < increment; i++) {
						clonedSystem = ParticleSystemManager::getSingletonPtr()->createParticleSystem(emitter->getEmitsName()+ ss.str() + StringConverter::toString(i), emitter->getEmitsName(), technique->getParentSystem()->getSceneManager());
						clonedSystem->_setMarkedForEmission(true);
						mParticleSystemPool.addElement(emitter->getEmitsName(), clonedSystem);
						mSystems.push_back(clonedSystem->getName());

						// Attach the Particle System to a SceneNode
						Ogre::SceneNode * parentNode = technique->getParentSystem()->getParentSceneNode();
						Ogre::SceneNode * node = parentNode->createChildSceneNode();
						node->attachObject(clonedSystem);
						node->setVisible(false);
					}
				}
			}
		}
	}
	
	void ParticlePool::initialisePool() {
		// Not sure what to do here.
	}
	
	void ParticlePool::increasePool(const Particle::ParticleType particleType, size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique) {
		switch (particleType) {
		case Particle::PT_VISUAL: {
			_increaseVisualParticlePool(size, behaviours);
			break;
		}
		case Particle::PT_EMITTER: {
			_increaseParticleEmitterPool(size, behaviours, technique);
			break;
		}
		case Particle::PT_TECHNIQUE: {
			_increaseParticleTechniquePool(size, behaviours, technique->getParentSystem());
			break;
		}
		case Particle::PT_AFFECTOR: {
			_increaseParticleAffectorPool(size, behaviours, technique);
			break;
		}
		case Particle::PT_SYSTEM: {
			_increaseParticleSystemPool(size, behaviours, technique);
			break;
		}
		default: {
			break;
		}
		}
		resetIterator();
	}
	
	Particle * ParticlePool::releaseParticle (const Particle::ParticleType particleType, const String & name) {
		switch (particleType) {
		case Particle::PT_VISUAL: {
			return mVisualParticlesPool.releaseElement();
			break;
		}
		case Particle::PT_EMITTER: {
			return mParticleEmitterPool.releaseElement(name);
			break;
		}
		case Particle::PT_TECHNIQUE: {
			return mParticleTechniquePool.releaseElement(name);
			break;
		}
		case Particle::PT_AFFECTOR: {
			return mParticleAffectorPool.releaseElement(name);
			break;
		}
		case Particle::PT_SYSTEM: {
			return mParticleSystemPool.releaseElement(name);
			break;
		}
		default: {
			break;
		}
		}
		
		return 0;
	}
	
	void ParticlePool::releaseAllParticles() {
		mVisualParticlesPool.releaseAllElements();
		mParticleEmitterPool.releaseAllElements();
		mParticleTechniquePool.releaseAllElements();
		mParticleAffectorPool.releaseAllElements();
		mParticleSystemPool.releaseAllElements();
		resetIterator();
	}
	
	void ParticlePool::lockLatestParticle() {
		if (mLatestParticle) {
			switch (mLatestParticle->particleType) {
			case Particle::PT_VISUAL: {
				mVisualParticlesPool.lockLatestElement();
				break;
			}
			case Particle::PT_EMITTER: {
				mParticleEmitterPool.lockLatestElement();
				break;
			}
			case Particle::PT_TECHNIQUE: {
				mParticleTechniquePool.lockLatestElement();
				break;
			}
			case Particle::PT_AFFECTOR: {
				mParticleAffectorPool.lockLatestElement();
				break;
			}
			case Particle::PT_SYSTEM: {
				mParticleSystemPool.lockLatestElement();
				break;
			}
			default: {
				break;
			}
			}
		}
	}
	
	void ParticlePool::lockAllParticles() {
		mVisualParticlesPool.lockAllElements();
		mParticleEmitterPool.lockAllElements();
		mParticleTechniquePool.lockAllElements();
		mParticleAffectorPool.lockAllElements();
		mParticleSystemPool.lockAllElements();
		resetIterator();
	}
	
	void ParticlePool::resetIterator() {
		mVisualParticlesPool.resetIterator();
		mParticleEmitterPool.resetIterator();
		mParticleTechniquePool.resetIterator();
		mParticleAffectorPool.resetIterator();
		mParticleSystemPool.resetIterator();
		mLatestParticle = nullptr;
	}
	
	Particle * ParticlePool::getFirst() {
		resetIterator();
		mLatestParticle = mVisualParticlesPool.getFirst();
		if (!mLatestParticle) {
			mLatestParticle = mParticleEmitterPool.getFirst();
			if (!mLatestParticle) {
				mLatestParticle = mParticleTechniquePool.getFirst();
				if (!mLatestParticle) {
					mLatestParticle = mParticleAffectorPool.getFirst();
					if (!mLatestParticle) {
						mLatestParticle = mParticleSystemPool.getFirst();
						if (!mLatestParticle) {
							// Todo: Add other particle types
						}
					}
				}
			}
		}

		return mLatestParticle;
	}
	
	Particle * ParticlePool::getNext() {
		// The getFirst() function must be called first, to get the first element.
		if (!mLatestParticle) {
			return nullptr;
		}

		switch (mLatestParticle->particleType) {
		// The previous particle was a visual particle?
		case Particle::PT_VISUAL: {
			mLatestParticle = mVisualParticlesPool.getNext();
			if (!mLatestParticle) {
				// No visual particles left, continue with the emitters
				mLatestParticle = mParticleEmitterPool.getFirst();
				if (!mLatestParticle) {
					// No emitters left, continue with the techniques
					mLatestParticle = mParticleTechniquePool.getFirst();
					if (!mLatestParticle) {
						// No techniques left, continue with the affectors
						mLatestParticle = mParticleAffectorPool.getFirst();
						if (!mLatestParticle) {
							// No affectors left, continue with the systems
							mLatestParticle = mParticleSystemPool.getFirst();
						}
					}
				}
			}
			break;
		}

		// The previous particle was an emitter?
		case Particle::PT_EMITTER: {
			mLatestParticle = mParticleEmitterPool.getNext();
			if (!mLatestParticle) {
				// No emitters left, continue with the techniques
				mLatestParticle = mParticleTechniquePool.getFirst();
				if (!mLatestParticle) {
					// No techniques left, continue with the affectors
					mLatestParticle = mParticleAffectorPool.getFirst();
					if (!mLatestParticle) {
						// No affectors left, continue with the systems
						mLatestParticle = mParticleSystemPool.getFirst();
					}
				}
			}
			break;
		}

		// The previous particle was a technique?
		case Particle::PT_TECHNIQUE: {
			mLatestParticle = mParticleTechniquePool.getNext();
			if (!mLatestParticle) {
				// No techniques left, continue with the affectors
				mLatestParticle = mParticleAffectorPool.getFirst();
				if (!mLatestParticle) {
					// No affectors left, continue with the systems
					mLatestParticle = mParticleSystemPool.getFirst();
				}
			}
			break;
		}

		// The previous particle was an affector?
		case Particle::PT_AFFECTOR: {
			mLatestParticle = mParticleAffectorPool.getNext();
			if (!mLatestParticle) {
				// No affectors left, continue with the systems
				mLatestParticle = mParticleSystemPool.getFirst();
			}
			break;
		}

		// The previous particle was a system?
		case Particle::PT_SYSTEM: {
			mLatestParticle = mParticleSystemPool.getNext();
			break;
		}
		default: {
			break;
		}
		}
		return mLatestParticle;
	}
	
	Particle * ParticlePool::getFirst(const Particle::ParticleType particleType) {
		resetIterator();
		switch (particleType) {
		case Particle::PT_VISUAL: {
			mLatestParticle = mVisualParticlesPool.getFirst();
			break;
		}
		case Particle::PT_EMITTER: {
			mLatestParticle = mParticleEmitterPool.getFirst();
			break;
		}
		case Particle::PT_TECHNIQUE: {
			mLatestParticle = mParticleTechniquePool.getFirst();
			break;
		}
		case Particle::PT_AFFECTOR: {
			mLatestParticle = mParticleAffectorPool.getFirst();
			break;
		}
		case Particle::PT_SYSTEM: {
			mLatestParticle = mParticleSystemPool.getFirst();
			break;
		}
		default: {
			break;
		}
		}

		return mLatestParticle;
	}
	
	Particle * ParticlePool::getNext(const Particle::ParticleType particleType) {
		mLatestParticle = nullptr;
		switch (particleType) {
		case Particle::PT_VISUAL: {
			mLatestParticle = mVisualParticlesPool.getNext();
			break;
		}
		case Particle::PT_EMITTER: {
			mLatestParticle = mParticleEmitterPool.getNext();
			break;
		}
		case Particle::PT_TECHNIQUE: {
			mLatestParticle = mParticleTechniquePool.getNext();
			break;
		}
		case Particle::PT_AFFECTOR: {
			mLatestParticle = mParticleAffectorPool.getNext();
			break;
		}
		case Particle::PT_SYSTEM: {
			mLatestParticle = mParticleSystemPool.getNext();
			break;
		}
		default: {
			break;
		}
		}

		return mLatestParticle;
	}
	
	bool ParticlePool::end() {
		return mVisualParticlesPool.end() && mParticleEmitterPool.end() && mParticleTechniquePool.end() && mParticleAffectorPool.end() && mParticleSystemPool.end();
	}
	
	bool ParticlePool::end(const Particle::ParticleType particleType) {
		switch (particleType) {
		case Particle::PT_VISUAL: {
			return mVisualParticlesPool.end();
			break;
		}
		case Particle::PT_EMITTER: {
			return mParticleEmitterPool.end();
			break;
		}
		case Particle::PT_TECHNIQUE: {
			return mParticleTechniquePool.end();
			break;
		}
		case Particle::PT_AFFECTOR: {
			return mParticleAffectorPool.end();
			break;
		}
		case Particle::PT_SYSTEM: {
			return mParticleSystemPool.end();
			break;
		}
		default: {
			break;
		}
		}

		return end(); // To get rid of the warning
	}

} /* namespace ParticleUniverse */
