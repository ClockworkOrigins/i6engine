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

#include "ParticleAffectors/ParticleUniverseRandomiser.h"

#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Vector3 Randomiser::DEFAULT_MAX_DEVIATION(0, 0, 0);
	const Real Randomiser::DEFAULT_TIME_STEP = 0.0;
	const bool Randomiser::DEFAULT_RANDOM_DIRECTION = true;

	Randomiser::Randomiser() : ParticleAffector(), mMaxDeviationX(DEFAULT_MAX_DEVIATION.x), mMaxDeviationY(DEFAULT_MAX_DEVIATION.y), mMaxDeviationZ(DEFAULT_MAX_DEVIATION.z), mTimeSinceLastUpdate(0.0), mTimeStep(DEFAULT_TIME_STEP), mRandomDirection(DEFAULT_RANDOM_DIRECTION), mUpdate(true) {
	}
	
	Real Randomiser::getMaxDeviationX() const {
		return mMaxDeviationX;
	}
	
	void Randomiser::setMaxDeviationX(Real maxDeviationX) {
		mMaxDeviationX = maxDeviationX;
	}
	
	Real Randomiser::getMaxDeviationY() const {
		return mMaxDeviationY;
	}
	
	void Randomiser::setMaxDeviationY(Real maxDeviationY) {
		mMaxDeviationY = maxDeviationY;
	}
	
	Real Randomiser::getMaxDeviationZ() const {
		return mMaxDeviationZ;
	}
	
	void Randomiser::setMaxDeviationZ(Real maxDeviationZ) {
		mMaxDeviationZ = maxDeviationZ;
	}
	
	Real Randomiser::getTimeStep() const {
		return mTimeStep;
	}
	
	void Randomiser::setTimeStep(Real timeStep) {
		mTimeStep = timeStep;
		mTimeSinceLastUpdate = timeStep;
	}
	
	bool Randomiser::isRandomDirection() const {
		return mRandomDirection;
	}
	
	void Randomiser::setRandomDirection(bool randomDirection) {
		mRandomDirection = randomDirection;
	}
	
	void Randomiser::_preProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		if (technique->getNumberOfEmittedParticles() > 0) {
			mTimeSinceLastUpdate += timeElapsed;
			if (mTimeSinceLastUpdate > mTimeStep) {
				mTimeSinceLastUpdate -= mTimeStep;
				mUpdate = true;
			}
		}
	}
	
	void Randomiser::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (mUpdate) {
			if (mRandomDirection) {
				// Random direction: Change the direction after each update
				particle->direction += Vector3(Math::RangeRandom(-mMaxDeviationX, mMaxDeviationX), Math::RangeRandom(-mMaxDeviationY, mMaxDeviationY), Math::RangeRandom(-mMaxDeviationZ, mMaxDeviationZ));
			} else {
				// Explicitly check on 'freezed', because it passes the techniques' validation.
				if (particle->isFreezed()) {
					return;
				}

				// Random position: Add the position deviation after each update
				particle->position += _mAffectorScale * Vector3(Math::RangeRandom(-mMaxDeviationX, mMaxDeviationX), Math::RangeRandom(-mMaxDeviationY, mMaxDeviationY), Math::RangeRandom(-mMaxDeviationZ, mMaxDeviationZ));
			}
		}
	}
	
	void Randomiser::_postProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		mUpdate = false;
	}
	
	void Randomiser::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		Randomiser * randomiser = static_cast<Randomiser *>(affector);
		randomiser->mMaxDeviationX = mMaxDeviationX;
		randomiser->mMaxDeviationY = mMaxDeviationY;
		randomiser->mMaxDeviationZ = mMaxDeviationZ;
		randomiser->setTimeStep(mTimeStep); // Also sets time since last update to appropriate value
		randomiser->mRandomDirection = mRandomDirection;
	}

} /* namespace ParticleUniverse */
