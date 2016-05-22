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

#ifndef __PU_SCALE_AFFECTOR_H__
#define __PU_SCALE_AFFECTOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse {

	/** Scales a particle. This can be a linear scale, but scaling that changes over time is possible.
    */
	class _ParticleUniverseExport ScaleAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_X_SCALE;
		static const Real DEFAULT_Y_SCALE;
		static const Real DEFAULT_Z_SCALE;
		static const Real DEFAULT_XYZ_SCALE;

		ScaleAffector();
	    virtual ~ScaleAffector();

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		DynamicAttribute * getDynScaleX() const { return mDynScaleX; }
		void setDynScaleX(DynamicAttribute * dynScaleX);
		void resetDynScaleX(bool resetToDefault = true);

		/** 
	    */
		DynamicAttribute * getDynScaleY() const { return mDynScaleY; }
		void setDynScaleY(DynamicAttribute * dynScaleY);
		void resetDynScaleY(bool resetToDefault = true);

		/** 
	    */
		DynamicAttribute * getDynScaleZ() const { return mDynScaleZ; }
		void setDynScaleZ(DynamicAttribute * dynScaleZ);
		void resetDynScaleZ(bool resetToDefault = true);

		/** 
	    */
		DynamicAttribute * getDynScaleXYZ() const { return mDynScaleXYZ; }
		void setDynScaleXYZ(DynamicAttribute * dynScaleXYZ);
		void resetDynScaleXYZ(bool resetToDefault = true);

		/** 
	    */
		bool isSinceStartSystem() const { return mSinceStartSystem; }
		void setSinceStartSystem(bool sinceStartSystem) { mSinceStartSystem = sinceStartSystem; }

	protected:
		DynamicAttribute * mDynScaleX;
		DynamicAttribute * mDynScaleY;
		DynamicAttribute * mDynScaleZ;
		DynamicAttribute * mDynScaleXYZ;
		bool mDynScaleXSet;
		bool mDynScaleYSet;
		bool mDynScaleZSet;
		bool mDynScaleXYZSet;
		DynamicAttributeFactory mDynamicAttributeFactory;
		DynamicAttributeHelper mDynamicAttributeHelper;
		bool mSinceStartSystem;

		/** Returns the scale value for the dynamic Scale.
	    */
		Real _calculateScale(DynamicAttribute * dynScale, Particle * particle);

		Real mLatestTimeElapsed;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SCALE_AFFECTOR_H__ */
