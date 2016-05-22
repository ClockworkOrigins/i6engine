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

#include "ParticleAffectors/ParticleUniverseJetAffector.h"

namespace ParticleUniverse {

	// Constants
	const Real JetAffector::DEFAULT_ACCELERATION = 1.0;

	JetAffector::JetAffector() : ParticleAffector(), mScaled(0.0) {
		mDynAcceleration = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		static_cast<DynamicAttributeFixed *>(mDynAcceleration)->setValue(DEFAULT_ACCELERATION);
	}
	
	JetAffector::~JetAffector () {
		if (!mDynAcceleration) {
			return;
		}

		PU_DELETE_T(mDynAcceleration, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		mDynAcceleration = nullptr;
	}
	
	void JetAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		JetAffector * jetAffector = static_cast<JetAffector *>(affector);
		jetAffector->setDynAcceleration(mDynamicAttributeFactory.cloneDynamicAttribute(getDynAcceleration()));
	}
	
	void JetAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mScaled = timeElapsed * mDynAcceleration->getValue(particle->timeFraction);
		if (particle->direction == Vector3::ZERO) {
			// Existing direction is zero, so use original direction
			particle->direction += particle->originalDirection * mScaled;
		} else {
			particle->direction += particle->direction * mScaled;
		}
	}
	
	void JetAffector::setDynAcceleration(DynamicAttribute * dynAcceleration) {
		if (mDynAcceleration) {
			PU_DELETE_T(mDynAcceleration, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		}

		mDynAcceleration = dynAcceleration;
	}

} /* namespace ParticleUniverse */
