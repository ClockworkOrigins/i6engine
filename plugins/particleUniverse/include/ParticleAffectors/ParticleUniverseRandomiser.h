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

#ifndef __PU_RANDOMISER_H__
#define __PU_RANDOMISER_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** Randomises the position or the direction of a particle.
    */
	class _ParticleUniverseExport Randomiser : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Vector3 DEFAULT_MAX_DEVIATION;
		static const Real DEFAULT_TIME_STEP;
		static const bool DEFAULT_RANDOM_DIRECTION;

		Randomiser();
	    virtual ~Randomiser() {}

		/** 
	    */
		Real getMaxDeviationX() const;
		void setMaxDeviationX(const Real maxDeviationX);

		/** 
	    */
		Real getMaxDeviationY() const;
		void setMaxDeviationY(const Real maxDeviationZ);

		/** 
	    */
		Real getMaxDeviationZ() const;
		void setMaxDeviationZ(const Real maxDeviationZ);

		/** 
	    */
		Real getTimeStep() const;
		void setTimeStep(const Real timeStep);

		/** 
	    */
		bool isRandomDirection() const;
		void setRandomDirection(bool randomDirection);

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
		Real mMaxDeviationX;
		Real mMaxDeviationY;
		Real mMaxDeviationZ;
		Real mTimeSinceLastUpdate;
		Real mTimeStep;
		bool mRandomDirection;
		bool mUpdate;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_RANDOMISER_H__ */
