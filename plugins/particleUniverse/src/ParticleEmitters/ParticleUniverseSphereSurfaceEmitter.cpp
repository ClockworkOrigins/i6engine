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

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real SphereSurfaceEmitter::DEFAULT_RADIUS = 10.0;
	
	SphereSurfaceEmitter::SphereSurfaceEmitter() : ParticleEmitter(), mRadius(DEFAULT_RADIUS), mRandomVector(Vector3::ZERO) {
	}
	
	Real SphereSurfaceEmitter::getRadius() const {
		return mRadius;
	}
	
	void SphereSurfaceEmitter::setRadius(const Real radius) {
		mRadius = radius;
	}
	
	void SphereSurfaceEmitter::_initParticlePosition(Particle * particle) {
		// Generate a random unit vector to calculate a point on the sphere. This unit vector is
		// also used as direction vector if mAutoDirection has been set.
		mRandomVector = Vector3(Math::RangeRandom(-1, 1), Math::RangeRandom(-1, 1), Math::RangeRandom(-1, 1));
		mRandomVector.normalise();
		ParticleSystem * sys = mParentTechnique->getParentSystem();
		if (sys) {
			particle->position = getDerivedPosition() + sys->getDerivedOrientation() * (_mEmitterScale * mRandomVector * mRadius);
		} else {
			particle->position = getDerivedPosition() + (_mEmitterScale * mRandomVector * mRadius);
		}
		particle->originalPosition = particle->position;
	}
	
    void SphereSurfaceEmitter::_initParticleDirection(Particle * particle) {
		if (mAutoDirection) {
			// The value of the direction vector that has been set does not have a meaning for
			// the sphere surface emitter.
			Radian angle;
			_generateAngle(angle);
			if (angle != Radian(0)) {
				particle->direction = mRandomVector.randomDeviant(angle, mUpVector);
				particle->originalDirection = particle->direction;
			} else {
				particle->direction = mRandomVector;
				particle->originalDirection = particle->direction;
			}
		} else {
			// Use the standard way
			ParticleEmitter::_initParticleDirection(particle);
		}
    }
	
	void SphereSurfaceEmitter::copyAttributesTo(ParticleEmitter * emitter) {
		ParticleEmitter::copyAttributesTo(emitter);

		SphereSurfaceEmitter * sphereSurfaceEmitter = static_cast<SphereSurfaceEmitter *>(emitter);
		sphereSurfaceEmitter->mRadius = mRadius;
	}

} /* namespace ParticleUniverse */
