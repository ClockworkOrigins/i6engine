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
