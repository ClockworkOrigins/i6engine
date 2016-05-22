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

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real GravityAffector::DEFAULT_GRAVITY = 1.0;

	GravityAffector::GravityAffector() : ParticleAffector(), mGravity(DEFAULT_GRAVITY) {
	}
	
	void GravityAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		GravityAffector * gravityAffector = static_cast<GravityAffector *>(affector);
		gravityAffector->mGravity = mGravity;
	}
	
	void GravityAffector::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Calculate the affectors' position so it is also safe to use mDerivedPosition.
		getDerivedPosition();
	}
	
	inline void GravityAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		 // Applied scaling in V1.3.1
		/** Applying Newton's law of universal gravitation.	*/
		Vector3 distance = mDerivedPosition - particle->position;
		Real length = distance.squaredLength();
		Real scaleVelocity = 1.0;
		if (mParentTechnique) {
			scaleVelocity = mParentTechnique->getParticleSystemScaleVelocity();
		}
		if (length > 0 && mParentTechnique) {
			//Real force = (mGravity * particle->mass * mass) / length;
			Real force = (scaleVelocity * mGravity * particle->mass * mass) / length;
			particle->direction += force * distance * timeElapsed * _calculateAffectSpecialisationFactor(particle);
		}
	}
	
	Real GravityAffector::getGravity() const {
		return mGravity;
	}
	
	void GravityAffector::setGravity(Real gravity) {
		mGravity = gravity;
	}

} /* namespace ParticleUniverse */
