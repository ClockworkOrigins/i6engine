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

#ifndef __PU_SINE_FORCE_AFFECTOR_H__
#define __PU_SINE_FORCE_AFFECTOR_H__

#include "ParticleUniverseBaseForceAffector.h"

namespace ParticleUniverse {

	/** Applies a sine force to a particle.
    */
	class _ParticleUniverseExport SineForceAffector : public BaseForceAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_FREQ_MIN;
		static const Real DEFAULT_FREQ_MAX;

		SineForceAffector();
	    virtual ~SineForceAffector() {}

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
		Real getFrequencyMin() const;
		void setFrequencyMin(const Real frequencyMin);

		/** 
	    */
		Real getFrequencyMax() const;
		void setFrequencyMax(const Real frequencyMax);

	protected:
		Real mAngle;
		Real mFrequencyMin;
		Real mFrequencyMax;
		Real mFrequency;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SINE_FORCE_AFFECTOR_H__ */
