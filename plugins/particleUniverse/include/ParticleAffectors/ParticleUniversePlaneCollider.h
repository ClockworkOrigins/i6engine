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

#ifndef __PU_PLANE_COLLIDER_H__
#define __PU_PLANE_COLLIDER_H__

#include "ParticleUniverseBaseCollider.h"
#include "ParticleUniversePlane.h"

namespace ParticleUniverse {

	/** The PlaneCollider is a plane shape that collides with the particles. The PlaneCollider can only collide 
		with particles that are created within the same ParticleTechnique as where the PlaneCollider is registered.
    */
	class _ParticleUniverseExport PlaneCollider : public BaseCollider {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Vector3 DEFAULT_NORMAL;

		PlaneCollider();
	    virtual ~PlaneCollider() {}

		/** Returns the normal of the plane
		*/
		const Vector3 getNormal() const;

		/** Sets the normal of the plane
		*/
		void setNormal(const Vector3 & normal);

		/** Notify that the Affector is rescaled.
	    */
		virtual void _notifyRescaled(const Vector3 & scale);

		/** 
		*/
		void calculateDirectionAfterCollision(Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		Vector3 mNormal;
		Vector3 mPredictedPosition;
		Plane mPlane;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PLANE_COLLIDER_H__ */
