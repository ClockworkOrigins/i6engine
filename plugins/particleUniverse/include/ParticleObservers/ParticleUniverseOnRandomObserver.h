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

#ifndef __PU_ONRANDOM_OBSERVER_H__
#define __PU_ONRANDOM_OBSERVER_H__

#include "ParticleUniverseObserver.h"

namespace ParticleUniverse {

	/** This class is used to observe whether a random generated number is within a specified interval.
    */
	class _ParticleUniverseExport OnRandomObserver : public ParticleObserver {
	public:
		// Constants
		static const Real DEFAULT_THRESHOLD;

		OnRandomObserver();
	    virtual ~OnRandomObserver() {}

		/** See ParticleObserver::_preProcessParticles()
	    */
		virtual void _preProcessParticles(ParticleTechnique * technique, Real timeElapsed);

		/** See ParticleObserver::_processParticle()
	    */
		virtual void _processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle);

		/** 
	    */
		virtual bool _observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		Real getThreshold() const { return mThreshold; }
		void setThreshold(Real threshold) { mThreshold = threshold; }

		/** Copy attributes to another observer.
	    */
		virtual void copyAttributesTo(ParticleObserver * observer);

	protected:
		Real mThreshold; // Value between 0..1
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ONRANDOM_OBSERVER_H__ */
