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

#ifndef __PU_COLLISION_AVOIDANCE_AFFECTOR_H__
#define __PU_COLLISION_AVOIDANCE_AFFECTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** The CollisionAvoidanceAffector is used to prevent particles from colliding with each other.
	@remarks
		The current implementation doesn�t take avoidance of colliders (box, sphere, plane) into account (yet).
    */
	class _ParticleUniverseExport CollisionAvoidanceAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_RADIUS;

		CollisionAvoidanceAffector();
	    virtual ~CollisionAvoidanceAffector() {}

		/** Todo
		*/
		Real getRadius() const;

		/** Todo
		*/
		void setRadius(Real radius);

		/** @copydoc ParticleAffector::_prepare */
		virtual void _prepare(ParticleTechnique * particleTechnique);

		/** @copydoc ParticleAffector::_unprepare */
		virtual void _unprepare(ParticleTechnique * particleTechnique);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);
		
	protected:
		Real mRadius;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_COLLISION_AVOIDANCE_AFFECTOR_H__ */
