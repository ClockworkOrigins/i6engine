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

#ifndef __PU_SCALE_VELOCITY_AFFECTOR_H__
#define __PU_SCALE_VELOCITY_AFFECTOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse {

	/** Scales the velocity of a particle. This can be a linear scale, but scaling that changes over time alos is possible.
    */
	class _ParticleUniverseExport ScaleVelocityAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_VELOCITY_SCALE;

		ScaleVelocityAffector();
	    virtual ~ScaleVelocityAffector();

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		DynamicAttribute * getDynScaleVelocity() const { return mDynScaleVelocity; }
		void setDynScaleVelocity(DynamicAttribute * dynScaleVelocity);
		void resetDynScaleVelocity(bool resetToDefault = true);

		/** 
	    */
		bool isSinceStartSystem() const { return mSinceStartSystem; }
		void setSinceStartSystem(bool sinceStartSystem) { mSinceStartSystem = sinceStartSystem; }

		/** 
	    */
		bool isStopAtFlip() const { return mStopAtFlip; }
		void setStopAtFlip(bool stopAtFlip) { mStopAtFlip = stopAtFlip; }

	protected:
		DynamicAttribute * mDynScaleVelocity;
		DynamicAttributeFactory mDynamicAttributeFactory;
		DynamicAttributeHelper mDynamicAttributeHelper;
		bool mSinceStartSystem;
		bool mStopAtFlip;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SCALE_VELOCITY_AFFECTOR_H__ */
