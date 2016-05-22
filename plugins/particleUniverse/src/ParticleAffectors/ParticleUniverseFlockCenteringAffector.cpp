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

#include "ParticleAffectors/ParticleUniverseFlockCenteringAffector.h"

#include "ParticleUniverseCommon.h"

namespace ParticleUniverse {
	
	void FlockCenteringAffector::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		if (!almostEquals(mCount, 0)) {
			// Calculate the average of the previous update
			mAverage = mSum / mCount;
		} else {
			mAverage = getDerivedPosition(); // Set to position of the affector
		}
		mSum = Vector3::ZERO;
		mCount = 0;
	}
	
	void FlockCenteringAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mSum += particle->position;
		mCount++;
		particle->direction += (mAverage - particle->position) * timeElapsed; // use average of the previous update
	}
	
	void FlockCenteringAffector::copyAttributesTo (ParticleAffector* affector) {
		ParticleAffector::copyAttributesTo(affector);
	}

} /* namespace ParticleUniverse */
