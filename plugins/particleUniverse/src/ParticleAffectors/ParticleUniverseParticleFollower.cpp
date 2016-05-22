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

#include "ParticleAffectors/ParticleUniverseParticleFollower.h"

namespace ParticleUniverse {

	// Constants
	const Real ParticleFollower::DEFAULT_MAX_DISTANCE = 3.40282e+038;
	const Real ParticleFollower::DEFAULT_MIN_DISTANCE = 10.0;

	
	ParticleFollower::ParticleFollower() : ParticleAffector(), mMinDistance(DEFAULT_MIN_DISTANCE), mMaxDistance(DEFAULT_MAX_DISTANCE), mPositionPreviousParticle(Vector3::ZERO), mFirst(false) {
	}
	
	void ParticleFollower::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		ParticleFollower * particleFollower = static_cast<ParticleFollower *>(affector);
		particleFollower->mMaxDistance = mMaxDistance;
		particleFollower->mMinDistance = mMinDistance;
	}
	
	void ParticleFollower::_firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// The first particle should go its own way
		mFirst = true;
	}
	
	void ParticleFollower::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (!mFirst) {
			// Change in V 1.3.1
			// Only proceed if it isn´t the first one. Compensate for scaling.
//			Real distance = (particle->position / _mAffectorScale).distance(mPositionPreviousParticle / _mAffectorScale);
			Real distance = particle->position.distance(mPositionPreviousParticle);
			Real avgScale = 0.3333 * (_mAffectorScale.x + _mAffectorScale.y + _mAffectorScale.z);
			Real scaledMinDistance = avgScale * mMinDistance;
			if (distance > scaledMinDistance && distance < avgScale * mMaxDistance) {
				// This particle drifts too much from the previous one; correct it!
				Real f = scaledMinDistance/distance;
				particle->position = mPositionPreviousParticle + f * (particle->position - mPositionPreviousParticle);
			}
		}

		mPositionPreviousParticle = particle->position;
		mFirst = false;
	}
	
	Real ParticleFollower::getMaxDistance() const {
		return mMaxDistance;
	}
	
	void ParticleFollower::setMaxDistance(Real maxDistance) {
		mMaxDistance = maxDistance;
	}
	
	Real ParticleFollower::getMinDistance() const {
		return mMinDistance;
	}
	
	void ParticleFollower::setMinDistance(Real minDistance) {
		mMinDistance = minDistance;
	}

} /* namespace ParticleUniverse */
