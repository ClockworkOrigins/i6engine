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

#include "ParticleAffectors/ParticleUniverseBaseCollider.h"

#include "ParticleUniverseAxisAlignedBox.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real BaseCollider::DEFAULT_BOUNCYNESS = 1.0;
	const Real BaseCollider::DEFAULT_FRICTION = 0.0;
	const BaseCollider::IntersectionType BaseCollider::DEFAULT_INTERSECTION_TYPE = BaseCollider::IT_POINT;
	const BaseCollider::CollisionType BaseCollider::DEFAULT_COLLISION_TYPE = BaseCollider::CT_BOUNCE;

	BaseCollider::BaseCollider() : ParticleAffector(), mFriction(DEFAULT_FRICTION), mBouncyness(DEFAULT_BOUNCYNESS), mIntersectionType(DEFAULT_INTERSECTION_TYPE), mCollisionType(DEFAULT_COLLISION_TYPE), mVelocityScale(1.0) {
	}
	
	void BaseCollider::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Take scaled velocity into account
		mVelocityScale = 1.0;
		if (mParentTechnique) {
			mVelocityScale = timeElapsed * mParentTechnique->getParticleSystemScaleVelocity();
		}
	}
	
	BaseCollider::IntersectionType BaseCollider::getIntersectionType() const {
		return mIntersectionType;
	}
	
	void BaseCollider::setIntersectionType(const BaseCollider::IntersectionType & intersectionType) {
		mIntersectionType = intersectionType;
	}
	
	BaseCollider::CollisionType BaseCollider::getCollisionType() const {
		return mCollisionType;
	}
	
	void BaseCollider::setCollisionType(const BaseCollider::CollisionType & collisionType) {
		mCollisionType = collisionType;
	}
	
	Real BaseCollider::getFriction() const {
		return mFriction;
	}
	
	void BaseCollider::setFriction(const Real friction) {
		mFriction = friction;
	}
	
	Real BaseCollider::getBouncyness() const {
		return mBouncyness;
	}
	
	void BaseCollider::setBouncyness(const Real bouncyness) {
		mBouncyness = bouncyness;
	}
	
	void BaseCollider::populateAlignedBox(AxisAlignedBox & box, const Vector3 & pos, const Real width, const Real height, const Real depth) {
		Real halfWidth = 0.5 * width;
		Real halfHeight = 0.5 * height;
		Real halfDepth = 0.5 * depth;
		box.setMinimum(pos.x - halfWidth, pos.y - halfHeight, pos.z - halfDepth);
		box.setMaximum(pos.x + halfWidth, pos.y + halfHeight, pos.z + halfDepth);
	}
	
	void BaseCollider::calculateRotationSpeedAfterCollision(Particle * particle) {
		if (particle->particleType != Particle::PT_VISUAL) {
			return;
		}

		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
		Real signedFriction = Math::UnitRandom() > 0.5 ? -(mFriction - 1) : (mFriction - 1);
		visualParticle->rotationSpeed *= signedFriction;
		visualParticle->zRotationSpeed *= signedFriction;
	}
	
	void BaseCollider::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		BaseCollider * baseCollider = static_cast<BaseCollider *>(affector);
		baseCollider->mBouncyness = mBouncyness;
		baseCollider->mFriction = mFriction;
		baseCollider->mIntersectionType = mIntersectionType;
		baseCollider->mCollisionType = mCollisionType;
	}

} /* namespace ParticleUniverse */
