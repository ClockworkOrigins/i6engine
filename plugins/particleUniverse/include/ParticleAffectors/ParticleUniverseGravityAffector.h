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

#ifndef __PU_GRAVITY_AFFECTOR_H__
#define __PU_GRAVITY_AFFECTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** This affector applies Newton's law of universal gravitation. The distance between a particle
		and the GravityAffector is important in the calculation of the gravity. Therefor, this affector needs
		to have its position set.
    */
	class _ParticleUniverseExport GravityAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_GRAVITY;
			
		GravityAffector();
	    virtual ~GravityAffector() {}

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** 
	    */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		Real getGravity() const;
		void setGravity(Real gravity);

	protected:
		Real mGravity;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_GRAVITY_AFFECTOR_H__ */
