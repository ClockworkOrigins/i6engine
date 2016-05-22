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

#ifndef __PU_PARTICLE_FOLLOWER_H__
#define __PU_PARTICLE_FOLLOWER_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** This affector makes particles follow its predecessor.
    */
	class _ParticleUniverseExport ParticleFollower : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_MAX_DISTANCE;
		static const Real DEFAULT_MIN_DISTANCE;

		ParticleFollower();
	    virtual ~ParticleFollower() {}

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** Validate if first particle.
		*/
		virtual void _firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		Real getMaxDistance() const;
		void setMaxDistance(Real maxDistance);

		/** 
	    */
		Real getMinDistance() const;
		void setMinDistance(Real minDistance);

	protected:
		Real mMinDistance;
		Real mMaxDistance;
		Vector3 mPositionPreviousParticle;
		bool mFirst;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PARTICLE_FOLLOWER_H__ */
