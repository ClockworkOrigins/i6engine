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

#ifndef __PU_INTERPARTICLE_COLLIDER_H__
#define __PU_INTERPARTICLE_COLLIDER_H__

#include "ParticleUniverseBaseCollider.h"

namespace ParticleUniverse {

	/** The InterParticleCollider is used to perform particle-particle collision.
    */
	class _ParticleUniverseExport InterParticleCollider : public BaseCollider {
	public:
		using Particle::copyAttributesTo;

		enum InterParticleCollisionResponse {
			IPCR_AVERAGE_VELOCITY,
			IPCR_ANGLE_BASED_VELOCITY
		};

		// Constants
		static const Real DEFAULT_ADJUSTMENT;
		static const InterParticleCollisionResponse DEFAULT_COLLISION_RESPONSE;

		InterParticleCollider();
	    virtual ~InterParticleCollider() {}

		/** Todo
		*/
		Real getAdjustment() const;

		/** Todo
		*/
		void setAdjustment(Real adjustment);

		/** Todo
		*/
		InterParticleCollisionResponse getInterParticleCollisionResponse() const;

		/** Todo
		*/
		void setInterParticleCollisionResponse(InterParticleCollisionResponse interParticleCollisionResponse);

		/** @copydoc ParticleAffector::_prepare */
		virtual void _prepare(ParticleTechnique * particleTechnique);

		/** @copydoc ParticleAffector::_unprepare */
		virtual void _unprepare(ParticleTechnique * particleTechnique);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);
		
	protected:
		Real mAdjustment;
		InterParticleCollisionResponse mInterParticleCollisionResponse;

		/** Todo
		*/
		bool _validateAndExecuteSphereCollision(Particle * particle1, Particle * particle2, Real timeElapsed);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_INTERPARTICLE_COLLIDER_H__ */
