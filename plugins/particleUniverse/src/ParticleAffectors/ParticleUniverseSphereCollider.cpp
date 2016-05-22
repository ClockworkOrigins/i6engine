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

#include "ParticleAffectors/ParticleUniverseSphereCollider.h"

#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const Real SphereCollider::DEFAULT_RADIUS = 100.0;

	SphereCollider::SphereCollider() : BaseCollider(), mRadius(DEFAULT_RADIUS), mPredictedPosition(Vector3::ZERO), mInnerCollision(false) {
	}
	
	Real SphereCollider::getRadius() const {
		return mRadius;
	}
	
	void SphereCollider::setRadius(const Real radius) {
		mRadius = radius;
		mSphere.setRadius(mRadius);
	}
	
	bool SphereCollider::isInnerCollision() const {
		return mInnerCollision;
	}
	
	void SphereCollider::setInnerCollision(bool innerCollision) {
		mInnerCollision = innerCollision;
	}
	
	void SphereCollider::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Call parent
		BaseCollider::_preProcessParticles(particleTechnique, timeElapsed);

		// Calculate the affectors' center position.
		mSphere.setCenter(getDerivedPosition());
	}
	
	void SphereCollider::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mPredictedPosition = particle->position + mVelocityScale * particle->direction;
		bool collision = false;
		Vector3 distance = particle->position - mDerivedPosition;
		Real distanceLength = distance.length();
		Real scaledRadius = 0.3333 * (_mAffectorScale.x + _mAffectorScale.y + _mAffectorScale.z) * mRadius; // Scaling changed in V 1.3.1

		switch (mIntersectionType) {
		case BaseCollider::IT_POINT: {
			// Validate for a point-sphere intersection
			//if (mInnerCollision == (distanceLength > mRadius))
			if (mInnerCollision == (distanceLength > scaledRadius)) {
				// Collision detected (re-position the particle)
				particle->position -= mVelocityScale * particle->direction;
				collision = true;
			} else {
				distance = mPredictedPosition - mDerivedPosition;
				distanceLength = distance.length();
				//if (mInnerCollision == (distanceLength > mRadius))
				if (mInnerCollision == (distanceLength > scaledRadius)) {
					// Collision detected
					collision = true;
				}
			}
			break;
		}
		case BaseCollider::IT_BOX: {
			// Validate for a box-sphere intersection
			if (particle->particleType != Particle::PT_VISUAL) {
				break;
			}

			VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
			AxisAlignedBox box;
			populateAlignedBox(box, visualParticle->position, visualParticle->width, visualParticle->height, visualParticle->depth);

				if (mInnerCollision != box.intersects(mSphere)) {
					// Collision detected (re-position the particle)
					particle->position -= mVelocityScale * particle->direction;
					collision = true;
				} else {
					AxisAlignedBox b;
					populateAlignedBox(b, mPredictedPosition, visualParticle->width, visualParticle->height, visualParticle->depth);
					if (mInnerCollision != b.intersects(mSphere)) {
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
			calculateDirectionAfterCollision(particle, distance, distanceLength);
			calculateRotationSpeedAfterCollision(particle);
			particle->addEventFlags(Particle::PEF_COLLIDED);
		}
	}
	
	void SphereCollider::calculateDirectionAfterCollision(Particle * particle, Vector3 distance, Real distanceLength) {
		switch (mCollisionType) {
		case BaseCollider::CT_BOUNCE: {
			/** If the particle is on the surface (or just inside the sphere); bounce it
				Make use of formula R = 2 * (-I dot N) * N + I, where
				R = the new direction vector
				I = the old (unit) direction vector before the collision
				N = the Normal at the collision point
			*/
			Real directionLength = particle->direction.length();
			particle->direction.normalise();
			distance.normalise();
			particle->direction = 2 * (-particle->direction.dotProduct(distance)) * distance + particle->direction;

			// Adjust to original speed
			particle->direction *= directionLength;

			// Accelerate/slow down, using the bounce value
			particle->direction *= mBouncyness;
			break;
		}
		case BaseCollider::CT_FLOW: {
			/** Reset the position (on the sphere), but keep the direction.
				This doesn't really work good for box-type collisions, because it doesn't take the particle
				dimensions into account.
			*/
			//particle->position = mDerivedPosition + distance * (mRadius / distanceLength);
			Real scaledRadius = 0.3333f * (_mAffectorScale.x + _mAffectorScale.y + _mAffectorScale.z) * mRadius;
			particle->position = mDerivedPosition + distance * (scaledRadius / distanceLength);
			break;
		}
		default: {
			break;
		}
		}
	}
	
	void SphereCollider::copyAttributesTo(ParticleAffector * affector) {
		BaseCollider::copyAttributesTo(affector);

		SphereCollider * sphereCollider = static_cast<SphereCollider *>(affector);
		sphereCollider->mRadius = mRadius;
		sphereCollider->mSphere = mSphere;
		sphereCollider->mInnerCollision = mInnerCollision;
	}

} /* namespace ParticleUniverse */
