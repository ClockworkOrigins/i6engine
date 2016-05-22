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

#ifndef __PU_LINE_AFFECTOR_H__
#define __PU_LINE_AFFECTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** Affects a particle depending on a line shape. Particles are getting a new position along the line.
    */
	class _ParticleUniverseExport LineAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_MAX_DEVIATION;
		static const Real DEFAULT_TIME_STEP;
		static const Vector3 DEFAULT_END;
		static const Real DEFAULT_DRIFT;
			
		LineAffector();
	    virtual ~LineAffector() {}

		/** 
	    */
		Real getMaxDeviation() const;
		void setMaxDeviation(Real maxDeviation);

		/** 
	    */
		const Vector3 & getEnd() const;
		void setEnd(const Vector3 & end);

		/** 
	    */
		Real getTimeStep() const;
		void setTimeStep(Real timeStep);

		/** 
	    */
		Real getDrift() const;
		void setDrift(Real drift);

		/**
	    */
		virtual void _notifyRescaled(const Vector3 & scale);

		/** 
	    */
		virtual void _firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);
		/** 
	    */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		virtual void _postProcessParticles(ParticleTechnique * technique, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		Real mMaxDeviation;
		Real _mScaledMaxDeviation;
		Vector3 mEnd;
		Real mTimeSinceLastUpdate;
		Real mTimeStep;
		Real mDrift;
		Real mOneMinusDrift;
		bool mUpdate;
		bool mFirst;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_LINE_AFFECTOR_H__ */
