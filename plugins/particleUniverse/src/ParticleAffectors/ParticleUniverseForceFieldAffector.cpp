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

#include "ParticleAffectors/ParticleUniverseForceFieldAffector.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const ForceField::ForceFieldType ForceFieldAffector::DEFAULT_FORCEFIELD_TYPE = ForceField::FF_REALTIME_CALC;
	const Real ForceFieldAffector::DEFAULT_DELTA = 1.0;
	const Real ForceFieldAffector::DEFAULT_FORCE = 400.0;
	const ushort ForceFieldAffector::DEFAULT_OCTAVES = 2;
	const double ForceFieldAffector::DEFAULT_FREQUENCY = 1.0;
	const double ForceFieldAffector::DEFAULT_AMPLITUDE = 1.0;
	const double ForceFieldAffector::DEFAULT_PERSISTENCE = 3.0;
	const unsigned int ForceFieldAffector::DEFAULT_FORCEFIELDSIZE = 64;
	const Vector3 ForceFieldAffector::DEFAULT_WORLDSIZE = Vector3(500.0, 500.0, 500.0);
	const Vector3 ForceFieldAffector::DEFAULT_MOVEMENT = Vector3(500.0, 0.0, 0.0);
	const Real ForceFieldAffector::DEFAULT_MOVEMENT_FREQUENCY = 5.0;
	
	ForceFieldAffector::ForceFieldAffector() : mForceFieldType(ForceField::FF_REALTIME_CALC), mDelta(DEFAULT_DELTA), mForce(Vector3::ZERO), mScaleForce(DEFAULT_FORCE), mOctaves(DEFAULT_OCTAVES), mFrequency(DEFAULT_FREQUENCY), mAmplitude(DEFAULT_AMPLITUDE), mPersistence(DEFAULT_PERSISTENCE), mForceFieldSize(DEFAULT_FORCEFIELDSIZE), mWorldSize(DEFAULT_WORLDSIZE), mPrepared(true), mIgnoreNegativeX(false), mIgnoreNegativeY(false), mIgnoreNegativeZ(false), mMovement(DEFAULT_MOVEMENT), mMovementSet(false), mMovementFrequency(DEFAULT_MOVEMENT_FREQUENCY), mMovementFrequencyCount(0.0), mDisplacement(Vector3::ZERO), mBasePosition(Vector3::ZERO), mSuppressGeneration(false) {
	}
	
	ForceFieldAffector::~ForceFieldAffector() {
	}
	
	void ForceFieldAffector::_prepare(ParticleTechnique * particleTechnique) {
		if (particleTechnique->getParentSystem()) {
			// Forcefield position is same position as particle system position
			mForceField.initialise(mForceFieldType, particleTechnique->getParentSystem()->getDerivedPosition(), mForceFieldSize, mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);

			mBasePosition = mForceField.getForceFieldPositionBase();
			mPrepared = true;
		}
	}
	
	void ForceFieldAffector::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		if (mMovementSet) {
			if (timeElapsed > mMovementFrequency) {
				// Ignore too large times, because it just blows things up
				return;
			}

			mMovementFrequencyCount += timeElapsed;
			if (mMovementFrequencyCount > mMovementFrequency) {
				mMovementFrequencyCount -= mMovementFrequency;
			}

			mDisplacement = Math::Sin(Math::TWO_PI * mMovementFrequencyCount/mMovementFrequency) * mMovement;
			mForceField.setForceFieldPositionBase(mBasePosition + mDisplacement);
		}
	}
	
	void ForceFieldAffector::_notifyStart() {
		ParticleAffector::_notifyStart();
		mMovementFrequencyCount = 0.0;
	}
	
	void ForceFieldAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mForceField.determineForce(particle->position, mForce, mDelta);

		// If negative values are ignored, set the force to 0.
		if (mIgnoreNegativeX) {
			mForce.x = 0.0;
		}
		if (mIgnoreNegativeY) {
			mForce.y = 0.0;
		}
		if (mIgnoreNegativeZ) {
			mForce.z = 0.0;
		}
		particle->direction += timeElapsed * mScaleForce * mForce;
	}
	
	ForceField::ForceFieldType ForceFieldAffector::getForceFieldType() const {
		return mForceFieldType;
	}
	
	void ForceFieldAffector::setForceFieldType(const ForceField::ForceFieldType forceFieldType) {
		mForceFieldType = forceFieldType;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setForceFieldType(forceFieldType);
	}
	
	Real ForceFieldAffector::getDelta() const {
		return mDelta;
	}
	
	void ForceFieldAffector::setDelta(Real delta) {
		mDelta = delta;
	}
	
	Real ForceFieldAffector::getScaleForce() const {
		return mScaleForce;
	}
	
	void ForceFieldAffector::setScaleForce(Real scaleForce) {
		mScaleForce = scaleForce;
	}
	
	ushort ForceFieldAffector::getOctaves() const {
		return mOctaves;
	}
	
	void ForceFieldAffector::setOctaves(ushort octaves) {
		mOctaves = octaves;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setOctaves(octaves);
	}
	
	double ForceFieldAffector::getFrequency() const {
		return mFrequency;
	}
	
	void ForceFieldAffector::setFrequency(double frequency) {
		mFrequency = frequency;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setFrequency(frequency);
	}
	
	double ForceFieldAffector::getAmplitude() const {
		return mAmplitude;
	}
	
	void ForceFieldAffector::setAmplitude(double amplitude) {
		mAmplitude = amplitude;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setAmplitude(amplitude);
	}
	
	double ForceFieldAffector::getPersistence() const {
		return mPersistence;
	}
	
	void ForceFieldAffector::setPersistence(double persistence) {
		mPersistence = persistence;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setPersistence(persistence);
	}
	
	unsigned int ForceFieldAffector::getForceFieldSize() const {
		return mForceFieldSize;
	}
	
	void ForceFieldAffector::setForceFieldSize(unsigned int forceFieldSize) {
		mForceFieldSize = forceFieldSize;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setForceFieldSize(forceFieldSize);
	}
	
	Vector3 ForceFieldAffector::getWorldSize() const {
		return mWorldSize;
	}
	
	void ForceFieldAffector::setWorldSize(const Vector3 & worldSize) {
		mWorldSize = worldSize;
		if (mSuppressGeneration) {
			return;
		}

		mForceField.setWorldSize(worldSize);
	}
	
	bool ForceFieldAffector::getIgnoreNegativeX() const {
		return mIgnoreNegativeX;
	}
	
	void ForceFieldAffector::setIgnoreNegativeX(bool ignoreNegativeX) {
		mIgnoreNegativeX = ignoreNegativeX;
	}
	
	bool ForceFieldAffector::getIgnoreNegativeY() const {
		return mIgnoreNegativeY;
	}
	
	void ForceFieldAffector::setIgnoreNegativeY(bool ignoreNegativeY) {
		mIgnoreNegativeY = ignoreNegativeY;
	}
	
	bool ForceFieldAffector::getIgnoreNegativeZ() const {
		return mIgnoreNegativeZ;
	}
	
	void ForceFieldAffector::setIgnoreNegativeZ(bool ignoreNegativeZ) {
		mIgnoreNegativeZ = ignoreNegativeZ;
	}
	
	Real ForceFieldAffector::getMovementFrequency() const {
		return mMovementFrequency;
	}
	
	void ForceFieldAffector::setMovementFrequency(Real movementFrequency) {
		mMovementFrequency = movementFrequency;
		mMovementSet = (movementFrequency > 0.0);
	}
	
	const Vector3 & ForceFieldAffector::getMovement() const {
		return mMovement;
	}
	
	void ForceFieldAffector::setMovement(const Vector3 & movement) {
		mMovement = movement;
		mMovementSet = (movement != Vector3::ZERO);
	}
	
	void ForceFieldAffector::suppressGeneration(bool suppress) {
		mSuppressGeneration = suppress;
	}
	
	void ForceFieldAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		ForceFieldAffector * forceFieldAffector = static_cast<ForceFieldAffector *>(affector);
		forceFieldAffector->mForceFieldType = mForceFieldType;
		forceFieldAffector->mDelta = mDelta;
		forceFieldAffector->mScaleForce = mScaleForce;
		forceFieldAffector->mOctaves = mOctaves;
		forceFieldAffector->mFrequency = mFrequency;
		forceFieldAffector->mAmplitude = mAmplitude;
		forceFieldAffector->mPersistence = mPersistence;
		forceFieldAffector->mForceFieldSize = mForceFieldSize;
		forceFieldAffector->mWorldSize = mWorldSize;
		forceFieldAffector->mIgnoreNegativeX = mIgnoreNegativeX;
		forceFieldAffector->mIgnoreNegativeY = mIgnoreNegativeY;
		forceFieldAffector->mIgnoreNegativeZ = mIgnoreNegativeZ;
		forceFieldAffector->mMovementSet = mMovementSet;
		forceFieldAffector->mMovementFrequency = mMovementFrequency;
		forceFieldAffector->mMovement = mMovement;
	}

} /* namespace ParticleUniverse */
