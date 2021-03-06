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

#include "ParticleAffectors/ParticleUniverseBoxCollider.h"

#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const Real BoxCollider::DEFAULT_WIDTH = 100.0;
	const Real BoxCollider::DEFAULT_HEIGHT = 100.0;
	const Real BoxCollider::DEFAULT_DEPTH = 100.0;

	BoxCollider::BoxCollider() : BaseCollider(), mWidth(DEFAULT_WIDTH), mHeight(DEFAULT_HEIGHT), mDepth(DEFAULT_DEPTH), mXmin(0.0), mXmax(0.0), mYmin(0.0), mYmax(0.0), mZmin(0.0), mZmax(0.0), mPredictedPosition(Vector3::ZERO), mInnerCollision(false) {
	}
	
	Real BoxCollider::getWidth() const {
		return mWidth;
	}
	
	void BoxCollider::setWidth(const Real width) {
		mWidth = width;
	}
	
	Real BoxCollider::getHeight() const {
		return mHeight;
	}
	
	void BoxCollider::setHeight(const Real height) {
		mHeight = height;
	}
	
	Real BoxCollider::getDepth() const {
		return mDepth;
	}
	
	void BoxCollider::setDepth(const Real depth) {
		mDepth = depth;
	}
	
	bool BoxCollider::isInnerCollision() const {
		return mInnerCollision;
	}
	
	void BoxCollider::setInnerCollision(bool innerCollision) {
		mInnerCollision = innerCollision;
	}
	
	void BoxCollider::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Call parent
		BaseCollider::_preProcessParticles(particleTechnique, timeElapsed);

		// Calculate the affectors' center position in worldspace, set the box and calculate the bounds
		// Applied scaling in V 1.3.1.
		populateAlignedBox(mBox, getDerivedPosition(), _mAffectorScale.x * mWidth, _mAffectorScale.y * mHeight, _mAffectorScale.z * mDepth);
		_calculateBounds();
	}
	
	void BoxCollider::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mPredictedPosition = particle->position + mVelocityScale * particle->direction;
		bool collision = false;

		/** Collision detection is a two-step. First, we determine whether the particle is now colliding.
			If it is, the particle is re-positioned. However, a timeElapsed value is used, which is not the same
			as the one that was used at the moment before the particle was colliding. Therefore, we rather 
			want to predict particle collision in front. This probably isn't the fastest solution.
			The same approach was used for the other colliders.
		*/
		switch(mIntersectionType) {
		case BaseCollider::IT_POINT: {
			// Validate for a point-box intersection
			if (mInnerCollision != mBox.intersects(particle->position)) {
				// Collision detected (re-position the particle)
				particle->position -= mVelocityScale * particle->direction;
				collision = true;
			} else if (mInnerCollision != mBox.intersects(mPredictedPosition)) {
				// Collision detected
				collision = true;
			}
			break;
		}
		case BaseCollider::IT_BOX: {
			// Validate for a box-box intersection
			if (particle->particleType != Particle::PT_VISUAL) {
				break;
			}

			VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
			AxisAlignedBox box;
			populateAlignedBox(box, visualParticle->position, visualParticle->width, visualParticle->height, visualParticle->depth);

			if (mInnerCollision != box.intersects(mBox)) {
				// Collision detected (re-position the particle)
				particle->position -= mVelocityScale * particle->direction;
				collision = true;
			} else {
				AxisAlignedBox b;
				populateAlignedBox(b, mPredictedPosition, visualParticle->width, visualParticle->height, visualParticle->depth);
				if (mInnerCollision != b.intersects(mBox)) {
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
			calculateDirectionAfterCollision(particle);
			calculateRotationSpeedAfterCollision(particle);
			particle->addEventFlags(Particle::PEF_COLLIDED);
		}
	}
	
	void BoxCollider::calculateDirectionAfterCollision(Particle * particle) {
		switch (mCollisionType) {
		case BaseCollider::CT_BOUNCE: {
			// Determine the nearest side and reverse the direction
			if (_isSmallestValue (particle->position.x - mXmin, particle->position)) {		
				particle->direction.x *= -1;
			} else if (_isSmallestValue (mXmax - particle->position.x, particle->position)) {
				particle->direction.x *= -1;
			} else if (_isSmallestValue (particle->position.y - mYmin, particle->position)) {
				particle->direction.y *= -1;
			} else if (_isSmallestValue (mYmax - particle->position.y, particle->position)) {
				particle->direction.y *= -1;
			} else if (_isSmallestValue (particle->position.z - mZmin, particle->position)) {
				particle->direction.z *= -1;
			} else if (_isSmallestValue (mZmax - particle->position.z, particle->position)) {
				particle->direction.z *= -1;
			}
			particle->direction *= mBouncyness;
			break;
		}
		case BaseCollider::CT_FLOW: {
			if (_isSmallestValue (particle->position.x - mXmin, particle->position)) {		
				particle->direction.x = 0;
			} else if (_isSmallestValue (mXmax - particle->position.x, particle->position)) {
				particle->direction.x = 0;
			} else if (_isSmallestValue (particle->position.y - mYmin, particle->position)) {
				particle->direction.y = 0;
			} else if (_isSmallestValue (mYmax - particle->position.y, particle->position)) {
				particle->direction.y = 0;
			} else if (_isSmallestValue (particle->position.z - mZmin, particle->position)) {
				particle->direction.z = 0;
			} else if (_isSmallestValue (mZmax - particle->position.z, particle->position)) {
				particle->direction.z = 0;
			}
			particle->direction *= -mFriction;
			break;
		}
		default: {
			break;
		}
		}
	}
	
	void BoxCollider::_calculateBounds() {
		Real scaledWidth = _mAffectorScale.x * mWidth;
		Real scaledHeight = _mAffectorScale.y * mHeight;
		Real scaledDepth = _mAffectorScale.z * mDepth;

		mXmin = mDerivedPosition.x - 0.5 * scaledWidth;
		mXmax = mDerivedPosition.x + 0.5 * scaledWidth;
		mYmin = mDerivedPosition.y - 0.5 * scaledHeight;
		mYmax = mDerivedPosition.y + 0.5 * scaledHeight;
		mZmin = mDerivedPosition.z - 0.5 * scaledDepth;
		mZmax = mDerivedPosition.z + 0.5 * scaledDepth;
	}
    
	bool BoxCollider::_isSmallestValue(Real value, const Vector3 & particlePosition) {
		Real value1 = particlePosition.x - mXmin;
		Real value2 = mXmax - particlePosition.x;
		Real value3 = particlePosition.y - mYmin;
		Real value4 = mYmax - particlePosition.y;
		Real value5 = particlePosition.z - mZmin;
		Real value6 = mZmax - particlePosition.z;

		return value <= value1 && value <= value2 && value <= value3 && value <= value4 && value <= value5 && value <= value6;
    }
	
	void BoxCollider::copyAttributesTo(ParticleAffector * affector) {
		BaseCollider::copyAttributesTo(affector);

		BoxCollider * boxCollider = static_cast<BoxCollider *>(affector);
		boxCollider->mWidth = mWidth;
		boxCollider->mHeight = mHeight;
		boxCollider->mDepth = mDepth;
		boxCollider->mInnerCollision = mInnerCollision;
	}

} /* namespace ParticleUniverse */
