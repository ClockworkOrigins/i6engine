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

#ifndef __PU_VORTEX_AFFECTOR_H__
#define __PU_VORTEX_AFFECTOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttributeFactory.h"
#include "ParticleUniverseMath.h"

namespace ParticleUniverse {

	/** The VortexAffector rotates particles around a given rotation axis.
    */
	class _ParticleUniverseExport VortexAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Vector3 DEFAULT_ROTATION_VECTOR;
		static const Real DEFAULT_ROTATION_SPEED;
		
		VortexAffector();
	    virtual ~VortexAffector();

		/** 
	    */
		const Vector3 & getRotationVector() const;

		/** 
	    */
		void setRotationVector(const Vector3 & rotationVector);

		/** 
	    */
		DynamicAttribute * getRotationSpeed() const;

		/** 
	    */
		void setRotationSpeed(DynamicAttribute * dynRotationSpeed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** 
	    */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

	protected:
		Vector3 mRotationVector;
		Quaternion mRotation;
		DynamicAttribute * mDynRotationSpeed;
		DynamicAttributeHelper mDynamicAttributeHelper;
		DynamicAttributeFactory mDynamicAttributeFactory;

		/** 
	    */
		Radian _calculateRotationSpeed();
	};

} /* namespace ParticleUniverse */

#endif /* __PU_VORTEX_AFFECTOR_H__ */
