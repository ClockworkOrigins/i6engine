/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
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
