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

#include "ParticleAffectors/ParticleUniversePlaneCollider.h"

#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const Vector3 PlaneCollider::DEFAULT_NORMAL(0, 0, 0);

	
	PlaneCollider::PlaneCollider() : BaseCollider(), mNormal(DEFAULT_NORMAL), mPredictedPosition(Vector3::ZERO) {
	}
	
	const Vector3 PlaneCollider::getNormal() const {
		return mNormal;
	}
	
	void PlaneCollider::setNormal(const Vector3 & normal) {
		mNormal = normal;
		mPlane.redefine(mNormal, getDerivedPosition()); // Changed in 1.3.1
	}
	
	void PlaneCollider::_notifyRescaled(const Vector3 & scale) {
		// Function added in 1.3.1
		ParticleAffector::_notifyRescaled(scale);
		mPlane.redefine(mNormal, getDerivedPosition());
	}
	
	void PlaneCollider::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mPredictedPosition = particle->position + mVelocityScale * particle->direction;
		bool collision = false;

		switch (mIntersectionType) {
		case BaseCollider::IT_POINT: {
			// Validate for a point-plane intersection (on the plane or the back side)
			// First determine whether it is now colliding (some affector made the particle move), else
			// determine whether it WILL be colliding
			if (mPlane.getDistance(particle->position) <= 0.0) {
				// Collision detected (re-position the particle)
				particle->position -= mVelocityScale * particle->direction;
				collision = true;
			} else if (mPlane.getDistance(mPredictedPosition) <= 0.0) {
				// Collision detected
				collision = true;
			}
			break;
		}
		case BaseCollider::IT_BOX: {
			// Validate for a box-plane intersection
			if (particle->particleType != Particle::PT_VISUAL) {
				break;
			}

			VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
			AxisAlignedBox box;
			populateAlignedBox(box, visualParticle->position, visualParticle->width, visualParticle->height, visualParticle->depth);
			if (box.intersects(mPlane)) {
				// Collision detected (re-position the particle)
				particle->position -= mVelocityScale * particle->direction;
				collision = true;
			} else  {
				populateAlignedBox(box, mPredictedPosition, visualParticle->width, visualParticle->height, visualParticle->depth);
				if (box.intersects(mPlane)) {
					// Collision detected
					collision = true;
				}
			}
			break;
		}
		default: {
			break;
		}
		}

		if (collision) {
			calculateDirectionAfterCollision(particle, timeElapsed);
			calculateRotationSpeedAfterCollision(particle);
			particle->addEventFlags(Particle::PEF_COLLIDED);
		}
	}
	
	void PlaneCollider::calculateDirectionAfterCollision(Particle * particle, Real timeElapsed) {
		Real directionLength = particle->direction.length();
		switch (mCollisionType) {
		case BaseCollider::CT_BOUNCE: {
			/** If the particle is on the plane or at the back of the plane, bounce it.
				Make use of the same formula as the sphere collider.
			*/
			particle->direction.normalise();
			particle->direction = 2 * (-particle->direction.dotProduct(-mNormal)) * -mNormal + particle->direction;

			// Adjust to original speed
			particle->direction *= directionLength;

			// Accelerate/slow down, using the bounce value
			particle->direction *= mBouncyness;
			break;
		}
		case BaseCollider::CT_FLOW: {
			/** Reset the position (just in front of the plane), but keep the direction.
			@remarks
				This is not really the correct way, because the particle 'jumps'. Maybe it is better to change 
				the direction parallel to the plane.
			*/
			particle->position += timeElapsed * directionLength * mNormal;
			break;
		}
		default: {
			break;
		}
		}
	}
	
	void PlaneCollider::copyAttributesTo(ParticleAffector * affector) {
		BaseCollider::copyAttributesTo(affector);

		PlaneCollider * planeCollider = static_cast<PlaneCollider *>(affector);
		planeCollider->setNormal(mNormal);
	}

} /* namespace ParticleUniverse */
