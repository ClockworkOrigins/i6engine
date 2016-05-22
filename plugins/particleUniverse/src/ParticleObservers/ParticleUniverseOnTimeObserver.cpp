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

#include "ParticleObservers/ParticleUniverseOnTimeObserver.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real OnTimeObserver::DEFAULT_THRESHOLD = 0.0;
	const bool OnTimeObserver::DEFAULT_SINCE_START_SYSTEM = false;

	OnTimeObserver::OnTimeObserver() : ParticleObserver(), mThreshold(DEFAULT_THRESHOLD), mCompare(CO_GREATER_THAN), mSinceStartSystem(DEFAULT_SINCE_START_SYSTEM) {
	}
	
	void OnTimeObserver::_preProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		// Call parent
		ParticleObserver::_preProcessParticles(technique, timeElapsed);

		// Also observe if there are no particles emitted, because some of the event handlers do not only
		// perform an action on a particle.
		if (technique->getNumberOfEmittedParticles() == 0) {
			_handleObserve(technique, 0, timeElapsed);
		}
	}
	
	bool OnTimeObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (mCompare == CO_GREATER_THAN) {
			if (mSinceStartSystem) {
				// Validate whether time since start of the particle system > threshold
				return (mParentTechnique->getParentSystem()->getTimeElapsedSinceStart() > mThreshold);
			} else {
				// Validate whether time since start of the particle emission > threshold
				return (particle && (particle->totalTimeToLive - particle->timeToLive) > mThreshold);
			}
		} else if (mCompare == CO_LESS_THAN) {
			if (mSinceStartSystem) {
				// Validate whether time since start of the particle system < threshold
				return (mParentTechnique->getParentSystem()->getTimeElapsedSinceStart() < mThreshold);
			} else {
				// Validate whether time since start of the particle emission < threshold
				return (particle && (particle->totalTimeToLive - particle->timeToLive) < mThreshold);
			}
		} else {
			// Equals
			if (mSinceStartSystem) {
				// Validate whether time since start of the particle system == threshold
				return almostEquals(mParentTechnique->getParentSystem()->getTimeElapsedSinceStart(), mThreshold, 0.01f);
			} else {
				// Validate whether time since start of the particle emission == threshold
				return particle && almostEquals((particle->totalTimeToLive - particle->timeToLive), mThreshold, 0.01f);
			}
		}
	}
	
	void OnTimeObserver::copyAttributesTo(ParticleObserver * observer) {
		ParticleObserver::copyAttributesTo(observer);

		OnTimeObserver * onTimeObserver = static_cast<OnTimeObserver *>(observer);
		onTimeObserver->mThreshold = mThreshold;
		onTimeObserver->mCompare = mCompare;
		onTimeObserver->mSinceStartSystem = mSinceStartSystem;
	}

} /* namespace ParticleUniverse */
