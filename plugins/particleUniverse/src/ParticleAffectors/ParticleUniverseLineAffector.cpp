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

#include "ParticleAffectors/ParticleUniverseLineAffector.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real LineAffector::DEFAULT_MAX_DEVIATION = 1.0;
	const Real LineAffector::DEFAULT_TIME_STEP = 0.1;
	const Vector3 LineAffector::DEFAULT_END(0, 0, 0);
	const Real LineAffector::DEFAULT_DRIFT = 0.0;
	
	LineAffector::LineAffector() : ParticleAffector(), mMaxDeviation(DEFAULT_MAX_DEVIATION), _mScaledMaxDeviation(1.0), mEnd(DEFAULT_END), mTimeSinceLastUpdate(0.0), mTimeStep(DEFAULT_TIME_STEP), mDrift(DEFAULT_DRIFT), mOneMinusDrift(1.0), mUpdate(true), mFirst(true) {
	}
	
	Real LineAffector::getMaxDeviation() const {
		return mMaxDeviation;
	}
	
	void LineAffector::setMaxDeviation(Real maxDeviation) {
		mMaxDeviation = maxDeviation;
		_mScaledMaxDeviation = mMaxDeviation * _mAffectorScale.length();
	}
	
	const Vector3 & LineAffector::getEnd() const {
		return mEnd;
	}
	
	void LineAffector::setEnd(const Vector3 & end) {
		mEnd = end;
	}
	
	Real LineAffector::getTimeStep() const {
		return mTimeStep;
	}
	
	void LineAffector::setTimeStep(Real timeStep) {
		mTimeStep = timeStep;
	}
	
	Real LineAffector::getDrift() const {
		return mDrift;
	}
	
	void LineAffector::setDrift(Real drift) {
		mDrift = drift;
		mOneMinusDrift = 1.0 - drift;
	}
	
	void LineAffector::_notifyRescaled(const Vector3 & scale) {
		ParticleAffector::_notifyRescaled(scale);
		_mScaledMaxDeviation = mMaxDeviation * scale.length();
	}
	
	void LineAffector::_firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// The first particle should stay on its place
		mFirst = true;
	}
	
	void LineAffector::_preProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		if (technique->getNumberOfEmittedParticles() > 0) {
			mTimeSinceLastUpdate += timeElapsed;
			while (mTimeSinceLastUpdate > mTimeStep) {
				mTimeSinceLastUpdate -= mTimeStep;
				mUpdate = true;
			}
		}
		mParentTechnique->getParentSystem()->rotationOffset(mEnd); // Always update
	}
	
	void LineAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mParentTechnique->getParentSystem()->rotationOffset(particle->originalPosition); // Always update
		if (mUpdate && Math::UnitRandom() > 0.5 && !mFirst) {
			// Generate a random vector perpendicular on the line
			Vector3 perpendicular = mEnd.crossProduct(Vector3(Math::RangeRandom(-1, 1), Math::RangeRandom(-1, 1), Math::RangeRandom(-1, 1)));
			perpendicular.normalise();

			// Determine a random point near the line.
			Vector3 targetPosition = particle->originalPosition + _mScaledMaxDeviation * Math::UnitRandom() * perpendicular;

			/** Set the new position.
			@remarks
				This affector already takes rotational offset of the particle system into account. This means that there is no need
				to set the particle system to keep_local to 'true'. The reason is that this is a specialized affector that calculates
				a new particle position instead of a direction.
			*/
			particle->position = mDrift * targetPosition + mOneMinusDrift * particle->position;
			mParentTechnique->getParentSystem()->rotationOffset(particle->position);
		}
		mFirst = false;
	}
	
	void LineAffector::_postProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		mUpdate = false;
	}
	
	void LineAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		LineAffector * lineAffector = static_cast<LineAffector *>(affector);

		lineAffector->setMaxDeviation(mMaxDeviation);
		lineAffector->mEnd = mEnd;
		lineAffector->mTimeStep = mTimeStep;
		lineAffector->mDrift = mDrift;
		lineAffector->mOneMinusDrift = mOneMinusDrift;
	}

} /* namespace ParticleUniverse */
