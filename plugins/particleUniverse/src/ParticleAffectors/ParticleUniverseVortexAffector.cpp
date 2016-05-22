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

#include "ParticleAffectors/ParticleUniverseVortexAffector.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"
#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const Vector3 VortexAffector::DEFAULT_ROTATION_VECTOR(0, 0, 0);
	const Real VortexAffector::DEFAULT_ROTATION_SPEED = 1.0;

	VortexAffector::VortexAffector() : ParticleAffector(), mRotationVector(DEFAULT_ROTATION_VECTOR), mRotation(Quaternion::IDENTITY) {
		mDynRotationSpeed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		static_cast<DynamicAttributeFixed *>(mDynRotationSpeed)->setValue(DEFAULT_ROTATION_SPEED);
	}
	
	VortexAffector::~VortexAffector() {
		if (mDynRotationSpeed) {
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynRotationSpeed = nullptr;
		}
	}
	
	const Vector3 & VortexAffector::getRotationVector() const {
		return mRotationVector;
	}
	
	void VortexAffector::setRotationVector(const Vector3 & rotationVector) {
		mRotationVector = rotationVector;
	}
	
	DynamicAttribute * VortexAffector::getRotationSpeed() const {
		return mDynRotationSpeed;
	}
	
	void VortexAffector::setRotationSpeed(DynamicAttribute * dynRotationSpeed) {
		if (mDynRotationSpeed) {
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		}

		mDynRotationSpeed = dynRotationSpeed;
	}
	
	void VortexAffector::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		ParticleSystem* sys = mParentTechnique->getParentSystem();
		if (sys) {
			mRotation.FromAngleAxis(Radian(_calculateRotationSpeed() * timeElapsed), sys->getDerivedOrientation() * mRotationVector);
		} else {
			mRotation.FromAngleAxis(Radian(_calculateRotationSpeed() * timeElapsed), mRotationVector);
		}
		getDerivedPosition();
	}
	
	Radian VortexAffector::_calculateRotationSpeed() {
		return Radian(mDynamicAttributeHelper.calculate(mDynRotationSpeed, mParentTechnique->getParentSystem()->getTimeElapsedSinceStart()));
	}
	
	void VortexAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Explicitly check on 'freezed', because it passes the techniques' validation.
		if (particle->isFreezed()) {
			return;
		}

		// Rotate position, direction and orientation (visible particle only) and compensate for the affector position
		// Also take the affect specialisation into account
		Vector3 local = particle->position - mDerivedPosition;
		particle->position = mDerivedPosition + mRotation * local;
		particle->direction = mRotation * particle->direction;

		if (particle->particleType == Particle::PT_VISUAL) {
			VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
			visualParticle->orientation = mRotation * visualParticle->orientation;
		}
	}
	
	void VortexAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		VortexAffector * vortexAffector = static_cast<VortexAffector *>(affector);
		vortexAffector->mRotation = mRotation;
		vortexAffector->mRotationVector = mRotationVector;
		vortexAffector->setRotationSpeed(mDynamicAttributeFactory.cloneDynamicAttribute(getRotationSpeed()));
	}

} /* namespace ParticleUniverse */
