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

#include "ParticleAffectors/ParticleUniverseSineForceAffector.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseMath.h"

namespace ParticleUniverse {

	// Constants
	const Real SineForceAffector::DEFAULT_FREQ_MIN = 1.0;
	const Real SineForceAffector::DEFAULT_FREQ_MAX = 1.0;

	SineForceAffector::SineForceAffector() : BaseForceAffector(), mAngle(361), mFrequencyMin(DEFAULT_FREQ_MIN), mFrequencyMax(DEFAULT_FREQ_MAX), mFrequency(1.0) {
	}
	
	void SineForceAffector::copyAttributesTo(ParticleAffector * affector)
	{
		BaseForceAffector::copyAttributesTo(affector);

		SineForceAffector * sineForceAffector = static_cast<SineForceAffector *>(affector);
		sineForceAffector->mFrequencyMin = mFrequencyMin;
		sineForceAffector->mFrequencyMax = mFrequencyMax;
		sineForceAffector->mFrequency = mFrequency;
		sineForceAffector->mAngle = mAngle;
	}
	
	void SineForceAffector::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Scale by time
		mAngle += mFrequency * timeElapsed;
		Real sineValue = Math::Sin(Radian(mAngle));
		mScaledVector = mForceVector * timeElapsed * sineValue;

		if (mAngle > Math::TWO_PI) {
			mAngle = 0.0;

			if (!almostEquals(mFrequencyMin, mFrequencyMax)) {
				mFrequency = Math::RangeRandom(mFrequencyMin, mFrequencyMax);
			}
		}
	}
	
	Real SineForceAffector::getFrequencyMin() const {
		return mFrequencyMin;
	}
	
	void SineForceAffector::setFrequencyMin(const Real frequencyMin) {
		mFrequencyMin = frequencyMin;
		if (frequencyMin > mFrequencyMax) {
			mFrequency = frequencyMin;
		}
	}
	
	Real SineForceAffector::getFrequencyMax() const {
		return mFrequencyMax;
	}
	
	void SineForceAffector::setFrequencyMax(const Real frequencyMax) {
		mFrequencyMax = frequencyMax;
		mFrequency = frequencyMax;
	}
	
	void SineForceAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Affect the direction
		if (mForceApplication == FA_ADD) {
			particle->direction += mScaledVector;
		} else {
			particle->direction = (particle->direction + mForceVector) / 2;
		}
	}

} /* namespace ParticleUniverse */
