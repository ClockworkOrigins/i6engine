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

#include "ParticleObservers/ParticleUniverseOnVelocityObserver.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real OnVelocityObserver::DEFAULT_VELOCITY_THRESHOLD = 0.0;
	
	OnVelocityObserver::OnVelocityObserver() : ParticleObserver(), mThreshold(DEFAULT_VELOCITY_THRESHOLD), mCompare(CO_LESS_THAN) {
	}
	
	bool OnVelocityObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (!particle) {
			return false;
		}

		// Compensate for the scaled velocity
		Real scaleVelocity = particleTechnique->getParentSystem()->getScaleVelocity();
		if (mCompare == CO_GREATER_THAN) {
			// Changed in V 1.3.1
			return (particle->calculateVelocity()) > (scaleVelocity * mThreshold);
		} else if (mCompare == CO_LESS_THAN) {
			return (particle->calculateVelocity()) < (scaleVelocity * mThreshold);
		} else {
			// Equals
			return almostEquals(particle->calculateVelocity(), (scaleVelocity * mThreshold), 0.01f);
		}
	}
	
	void OnVelocityObserver::copyAttributesTo(ParticleObserver * observer) {
		ParticleObserver::copyAttributesTo(observer);

		OnVelocityObserver * onVelocityObserver = static_cast<OnVelocityObserver *>(observer);
		onVelocityObserver->mThreshold = mThreshold;
		onVelocityObserver->mCompare = mCompare;
	}

} /* namespace ParticleUniverse */
