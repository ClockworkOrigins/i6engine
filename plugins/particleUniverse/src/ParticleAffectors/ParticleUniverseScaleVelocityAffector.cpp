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

#include "ParticleAffectors/ParticleUniverseScaleVelocityAffector.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real ScaleVelocityAffector::DEFAULT_VELOCITY_SCALE = 1.0;

	ScaleVelocityAffector::ScaleVelocityAffector() : ParticleAffector(), mSinceStartSystem(false), mStopAtFlip(false) {
		mDynScaleVelocity = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		static_cast<DynamicAttributeFixed *>(mDynScaleVelocity)->setValue(DEFAULT_VELOCITY_SCALE);
	}
	
	ScaleVelocityAffector::~ScaleVelocityAffector() {
		if (mDynScaleVelocity) {
			PU_DELETE_T(mDynScaleVelocity, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynScaleVelocity = nullptr;
		}
	}
	
	void ScaleVelocityAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		Real ds = 0;
		if (mSinceStartSystem) {
			// If control points are used (curved type), the first value of each control point is seconds from the start of the system
			ds = timeElapsed * mDynamicAttributeHelper.calculate(mDynScaleVelocity, mParentTechnique->getParentSystem()->getTimeElapsedSinceStart());
		} else {
			// If control points are used (curved type), the first value of each control point is the fraction of the particle lifetime [0..1]
			ds = timeElapsed * mDynamicAttributeHelper.calculate(mDynScaleVelocity, particle->timeFraction);
		}
		Real length = particle->direction.length(); // Use length for a better delta direction value
		Vector3 calculated = particle->direction;
		//particle->direction.x += ds * (particle->direction.x / length);
		//particle->direction.y += ds * (particle->direction.y / length);
		//particle->direction.z += ds * (particle->direction.z / length);
		calculated.x += ds * (particle->direction.x / length);
		calculated.y += ds * (particle->direction.y / length);
		calculated.z += ds * (particle->direction.z / length);

		if (mStopAtFlip) {
			if ((calculated.x > 0.0 && particle->direction.x < 0.0) || (calculated.y > 0.0 && particle->direction.y < 0.0) || (calculated.z > 0.0 && particle->direction.z < 0.0) || (calculated.x < 0.0 && particle->direction.x > 0.0) || (calculated.y < 0.0 && particle->direction.y > 0.0) || (calculated.z < 0.0 && particle->direction.z > 0.0)) {
				return;
			}
		}

		particle->direction = calculated;
	}
	
	void ScaleVelocityAffector::setDynScaleVelocity(DynamicAttribute * dynScaleVelocity) {
		if (mDynScaleVelocity) {
			PU_DELETE_T(mDynScaleVelocity, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		}

		mDynScaleVelocity = dynScaleVelocity;
	}
	
	void ScaleVelocityAffector::resetDynScaleVelocity(bool resetToDefault) {
		if (resetToDefault) {
			PU_DELETE_T(mDynScaleVelocity, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynScaleVelocity = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
			static_cast<DynamicAttributeFixed *>(mDynScaleVelocity)->setValue(DEFAULT_VELOCITY_SCALE);
		}
	}
	
	void ScaleVelocityAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		ScaleVelocityAffector * scaleVelocityAffector = static_cast<ScaleVelocityAffector *>(affector);
		scaleVelocityAffector->setDynScaleVelocity(mDynamicAttributeFactory.cloneDynamicAttribute(getDynScaleVelocity()));
		scaleVelocityAffector->mSinceStartSystem = mSinceStartSystem;
		scaleVelocityAffector->mStopAtFlip = mStopAtFlip;
	}

} /* namespace ParticleUniverse */
