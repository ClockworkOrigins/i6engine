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

#ifndef __PU_ONVELOCITY_OBSERVER_H__
#define __PU_ONVELOCITY_OBSERVER_H__

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseObserver.h"

namespace ParticleUniverse {

	/** The OnVelocityObserver determines whether the velocity of a particles is lower or higher than a certain
		threshold value.
    */
	class _ParticleUniverseExport OnVelocityObserver : public ParticleObserver {
	public:
		// Constants
		static const Real DEFAULT_VELOCITY_THRESHOLD;

		OnVelocityObserver();
	    virtual ~OnVelocityObserver() {}

		/** 
	    */
		virtual bool _observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		Real getThreshold() const { return mThreshold; }
		void setThreshold(Real threshold) { mThreshold = threshold; }

		/** 
	    */
		ComparisionOperator getCompare() const { return mCompare; }
		void setCompare(ComparisionOperator op) { mCompare = op; }

		/** Copy attributes to another observer.
	    */
		virtual void copyAttributesTo(ParticleObserver * observer);

	protected:
		Real mThreshold;
		ComparisionOperator mCompare;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ONVELOCITY_OBSERVER_H__ */
