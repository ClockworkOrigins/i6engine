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

#ifndef __PU_JET_AFFECTOR_H__
#define __PU_JET_AFFECTOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse {

	/** Adds a non-linear boost to a particle.
    */
	class _ParticleUniverseExport JetAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_ACCELERATION;

		JetAffector();
	    virtual ~JetAffector();

		/** 
	    */
		DynamicAttribute * getDynAcceleration() const { return mDynAcceleration; }
		void setDynAcceleration(DynamicAttribute * dynAcceleration);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		Real mScaled;

		/** Dynamic attribute used to generate the velocity of a particle.
		*/
		DynamicAttribute * mDynAcceleration;

		/** Helper factory
		*/
		DynamicAttributeFactory mDynamicAttributeFactory;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_JET_AFFECTOR_H__ */
