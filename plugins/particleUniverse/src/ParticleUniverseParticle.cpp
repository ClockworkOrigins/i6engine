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

#include "ParticleUniverseParticle.h"

#include "ParticleUniverseBehaviour.h"
#include "ParticleUniverseSystemManager.h"

namespace ParticleUniverse {

	Real Particle::DEFAULT_TTL = 10.0;
	Real Particle::DEFAULT_MASS = 1.0;
	
	Particle::~Particle() {
		for (ParticleBehaviourIterator it = mBehaviours.begin(); it != mBehaviours.end(); ++it) {
			ParticleSystemManager::getSingletonPtr()->destroyBehaviour(*it);
		}
	}
	
	void Particle::_initForEmission() {
		mEventFlags = 0;
		timeFraction = 0.0;

		/*	Note, that this flag must only be set as soon as the particle is emitted. As soon as the particle has
			been moved once, the flag must be removed again.
		*/
		addEventFlags(Particle::PEF_EMITTED);

		// Reset freeze flag
		mFreezed = false;

		// Iterate through all Behaviour objects that are registered at the particle.
		if (mBehaviours.empty()) {
			return;
		}

		for (ParticleBehaviourIterator it = mBehaviours.begin(); it != mBehaviours.end(); ++it) {
			(*it)->_initParticleForEmission(this);
		}
	}
	
	bool Particle::isEnabled() const {
		return mEnabled;
	}
	
	void Particle::setEnabled(bool enabled) {
		mEnabled = enabled;
		if (!mOriginalEnabledSet) {
			// Only one time is permitted
			mOriginalEnabled = enabled;
			mOriginalEnabledSet = true;
		}
	}
	
	void Particle::_setOriginalEnabled(bool originalEnabled) {
		mOriginalEnabled = originalEnabled;
		mOriginalEnabledSet = true;
	}
	
	bool Particle::_getOriginalEnabled() const {
		return mOriginalEnabled;
	}
	
	bool Particle::isFreezed() const {
		return mFreezed;
	}
	
	void Particle::setFreezed(bool freezed) {
		mFreezed = freezed;
	}
	
	ParticleBehaviour * Particle::getBehaviour(const String & behaviourType) {
		if (behaviourType == BLANK_STRING) {
			return nullptr;
		}

		for (ParticleBehaviourIterator it = mBehaviours.begin(); it != mBehaviours.end(); ++it) {
			if ((*it)->getBehaviourType() == behaviourType) {
				return *it;
			}
		}
		
		return nullptr;
	}
	
	void Particle::copyBehaviours(ParticleBehaviourList & behaviours) {
		for (ParticleBehaviourIterator it = behaviours.begin(); it != behaviours.end(); ++it) {
			mBehaviours.push_back(ParticleSystemManager::getSingletonPtr()->cloneBehaviour(*it));
		}
	}
	
	Real Particle::calculateVelocity() const {
		if (!almostEquals(originalScaledDirectionLength, 0)) {
			return originalVelocity * (direction.length() / originalScaledDirectionLength);
		} else {
			// Assume originalScaledDirectionLength to be 1.0 (unit vector)
			return originalVelocity * direction.length();
		}
	}
	
	void Particle::copyAttributesTo(Particle * particle) {
		// Copy attributes
		particle->position = position;
		particle->originalPosition = originalPosition;
		particle->mDerivedPosition = mDerivedPosition;
		particle->direction = direction;
		particle->originalDirection = originalDirection;
		particle->originalDirectionLength = originalDirectionLength;
		particle->originalScaledDirectionLength = originalScaledDirectionLength;
		particle->originalVelocity = originalVelocity;
		particle->mass = mass;
		particle->timeToLive = timeToLive;
		particle->totalTimeToLive = totalTimeToLive;
		particle->mEventFlags = mEventFlags;
		particle->mMarkedForEmission = mMarkedForEmission;
		particle->mEnabled = mEnabled;
		particle->mOriginalEnabled = mOriginalEnabled;
		particle->mOriginalEnabledSet = mOriginalEnabledSet;
		particle->mFreezed = mFreezed;
		particle->timeFraction = timeFraction;

		// Copy Behaviour
		particle->copyBehaviours(mBehaviours);
	}
	
	void Particle::_process(ParticleTechnique * technique, Real timeElapsed) {
		// Calculate the time fraction, because it is used in different other components
		timeFraction = (totalTimeToLive - timeToLive) / totalTimeToLive;

		if (mBehaviours.empty()) {
			return;
		}

		// Iterate through all Behaviour objects that are registered at the particle.
		for (ParticleBehaviourIterator it = mBehaviours.begin(); it != mBehaviours.end(); ++it) {
			(*it)->_processParticle(technique, this, timeElapsed);
		}
	}
	
	void Particle::_initForExpiration(ParticleTechnique * technique, Real timeElapsed) {
		if (mBehaviours.empty()) {
			return;
		}

		// Iterate through all Behaviour objects that are registered at the particle.
		for (ParticleBehaviourIterator it = mBehaviours.begin(); it != mBehaviours.end(); ++it) {
			(*it)->_initParticleForExpiration(technique, this, timeElapsed);
		}
	}

} /* namespace ParticleUniverse */
