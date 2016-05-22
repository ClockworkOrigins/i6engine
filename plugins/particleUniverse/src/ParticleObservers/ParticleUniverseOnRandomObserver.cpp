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

#include "ParticleObservers/ParticleUniverseOnRandomObserver.h"

#include "ParticleUniverseMath.h"

namespace ParticleUniverse {

	// Constants
	const Real OnRandomObserver::DEFAULT_THRESHOLD = 0.5;

	OnRandomObserver::OnRandomObserver() : ParticleObserver(), mThreshold(DEFAULT_THRESHOLD) {
	}
	
	void OnRandomObserver::_preProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		// Call parent
		ParticleObserver::_preProcessParticles(technique, timeElapsed);

		// Handle the _observe here
		_handleObserve(technique, 0, timeElapsed);
	}
	
	void OnRandomObserver::_processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
		// No need to determine per particle
	}
	
	bool OnRandomObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		return (Math::UnitRandom() > mThreshold);
	}
	
	void OnRandomObserver::copyAttributesTo(ParticleObserver * observer) {
		ParticleObserver::copyAttributesTo(observer);
		OnRandomObserver * onRandomObserver = static_cast<OnRandomObserver *>(observer);
		onRandomObserver->mThreshold = mThreshold;
	}

} /* namespace ParticleUniverse */
