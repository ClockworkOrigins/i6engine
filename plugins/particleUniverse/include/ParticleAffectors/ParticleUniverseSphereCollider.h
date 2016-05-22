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

#ifndef __PU_SPHERE_COLLIDER_H__
#define __PU_SPHERE_COLLIDER_H__

#include "ParticleUniverseBaseCollider.h"
#include "ParticleUniverseSimpleSphere.h"

namespace ParticleUniverse {

	/** The SphereCollider is a sphere shape that collides with the particles. The SphereCollider can only collide 
		with particles that are created within the same ParticleTechnique as where the SphereCollider is registered.
    */
	class _ParticleUniverseExport SphereCollider : public BaseCollider {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_RADIUS;

		SphereCollider();
	    virtual ~SphereCollider() {}

		/** Returns the radius of the sphere
		*/
		Real getRadius() const;

		/** Sets the radius of the sphere
		*/
		void setRadius(const Real radius);

		/** Returns indication whether the collision is inside or outside of the box
		@remarks
			If value is true, the collision is inside of the box.
		*/
		bool isInnerCollision() const;

		/** Set indication whether the collision is inside or outside of the box
		@remarks
			If value is set to true, the collision is inside of the box.
		*/
		void setInnerCollision(bool innerCollision);

		/** 
		*/
		void calculateDirectionAfterCollision(Particle * particle, Vector3 distance, Real distanceLength);

		/** @copydoc ParticleAffector::_preProcessParticles */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		Real mRadius;
		SimpleSphere mSphere;
		Vector3 mPredictedPosition;
		bool mInnerCollision;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SPHERE_COLLIDER_H__ */
