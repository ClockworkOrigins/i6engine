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

#include "ParticleEmitters/ParticleUniverseCircleEmitter.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real CircleEmitter::DEFAULT_RADIUS = 100.0;
	const Real CircleEmitter::DEFAULT_STEP = 0.1;
	const Real CircleEmitter::DEFAULT_ANGLE = 0.0;
	const bool CircleEmitter::DEFAULT_RANDOM = true;
	const Vector3 CircleEmitter::DEFAULT_NORMAL(0, 0, 0);
	
	CircleEmitter::CircleEmitter() : ParticleEmitter(), mRadius(DEFAULT_RADIUS), mCircleAngle(DEFAULT_ANGLE), mOriginalCircleAngle(DEFAULT_ANGLE), mStep(DEFAULT_STEP), mX(0.0), mZ(0.0), mRandom(DEFAULT_RANDOM), mOrientation(Quaternion::IDENTITY), mNormal(DEFAULT_NORMAL) {
	}
	
	Real CircleEmitter::getRadius() const {
		return mRadius;
	}
	
	void CircleEmitter::setRadius(const Real radius) {
		mRadius = radius;
	}
	
	Real CircleEmitter::getCircleAngle() const {
		return mOriginalCircleAngle;
	}
	
	void CircleEmitter::setCircleAngle(const Real circleAngle) {
		mOriginalCircleAngle = circleAngle;
		mCircleAngle = circleAngle;
	}
	
	Real CircleEmitter::getStep() const {
		return mStep;
	}
	
	void CircleEmitter::setStep(const Real step) {
		mStep = step;
	}
	
	bool CircleEmitter::isRandom() const {
		return mRandom;
	}
	
	void CircleEmitter::setRandom(const bool random) {
		mRandom = random;
	}
	
	const Quaternion & CircleEmitter::getOrientation() const {
		return mOrientation;
	}
	 
	const Vector3 & CircleEmitter::getNormal() const { 
		return mNormal;
	} 
	 
	void CircleEmitter::setNormal(const Vector3 & normal) { 
		mOrientation = Vector3::UNIT_Y.getRotationTo(normal, Vector3::UNIT_X);
		mNormal = normal;
	}
	
	void CircleEmitter::_notifyStart() {
		// Reset the attributes to allow a restart.
		ParticleEmitter::_notifyStart();
		mCircleAngle = mOriginalCircleAngle;
	}
	 
	void CircleEmitter::_initParticlePosition(Particle * particle) {
		Real angle = 0;
		if (mRandom) {
			// Choose a random position on the circle.
			angle = Math::RangeRandom(0, Math::TWO_PI);
		} else {
			// Follow the contour of the circle.
			mCircleAngle += mStep;
			mCircleAngle = mCircleAngle > Math::TWO_PI ? mCircleAngle - Math::TWO_PI : mCircleAngle;
			angle = mCircleAngle;
		}

		mX = Math::Cos(angle);
		mZ = Math::Sin(angle);
		ParticleSystem * sys = mParentTechnique->getParentSystem();
		if (sys) {
			// Take both orientation of the node and its own orientation, based on the normal, into account
			particle->position = getDerivedPosition() + sys->getDerivedOrientation() * mOrientation * (_mEmitterScale * Vector3(mX * mRadius, 0, mZ * mRadius));
		} else {
			particle->position = getDerivedPosition() + _mEmitterScale * (mOrientation * Vector3(mX * mRadius, 0, mZ * mRadius));
		}
		particle->originalPosition = particle->position;
	}
	
    void CircleEmitter::_initParticleDirection(Particle * particle) {
		if (mAutoDirection) {
			// The value of the direction vector that has been set does not have a meaning for
			// the circle emitter.
			Radian angle;
			_generateAngle(angle);
			if (angle != Radian(0)) {
				particle->direction = (mOrientation * Vector3(mX, 0, mZ)).randomDeviant(angle, mUpVector);
				particle->originalDirection = particle->direction;
			} else {
				particle->direction = Vector3(mX, 0, mZ);
				particle->direction = mOrientation * Vector3(mX, 0, mZ);
			}
		} else {
			// Use the standard way
			ParticleEmitter::_initParticleDirection(particle);
		}
    }
	
	void CircleEmitter::copyAttributesTo(ParticleEmitter * emitter) {
		ParticleEmitter::copyAttributesTo(emitter);

		CircleEmitter * circleEmitter = static_cast<CircleEmitter *>(emitter);
		circleEmitter->mRadius = mRadius;
		circleEmitter->mCircleAngle = mCircleAngle;
		circleEmitter->mOriginalCircleAngle = mOriginalCircleAngle;
		circleEmitter->mStep = mStep;
		circleEmitter->mRandom = mRandom;
		circleEmitter->mNormal = mNormal;
		circleEmitter->mOrientation = mOrientation; 
	}

} /* namespace ParticleUniverse */
