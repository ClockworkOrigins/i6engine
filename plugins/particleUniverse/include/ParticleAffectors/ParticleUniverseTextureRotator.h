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

#ifndef __PU_TEXTURE_ROTATOR_H__
#define __PU_TEXTURE_ROTATOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttributeFactory.h"
#include "ParticleUniverseMath.h"

namespace ParticleUniverse {

	/** The TextureRotator rotates the texture(s) of a particle. In general it is possible to support individual
		rotation of each particle texture-set - the same as in the geometry rotator, setting 
		mUseOwnRotationSpeed to true -, but in practice this isn't really usable, because usually all particles
		share the same material.
    */
	class _ParticleUniverseExport TextureRotator : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const bool DEFAULT_USE_OWN_SPEED;
		static const Real DEFAULT_ROTATION_SPEED;
		static const Real DEFAULT_ROTATION;

		TextureRotator();
	    virtual ~TextureRotator();

		/** Returns an indication whether the 2D rotation speed is the same for all particles in this 
			particle technique, or whether the 2D rotation speed of the particle itself is used.
	    */
		bool useOwnRotationSpeed() const;

		/** Set the indication whether the 2D rotation speed of the particle itself is used.
	    */
		void setUseOwnRotationSpeed(bool useOwnRotationSpeed);

		/** Returns the rotation speed. This is the speed controlled by the affector.
	    */
		DynamicAttribute * getRotationSpeed() const;

		/** 
	    */
		void setRotationSpeed(DynamicAttribute * dynRotationSpeed);

		/** Returns the rotation defined in the the affector.
	    */
		DynamicAttribute * getRotation() const;

		/** 
	    */
		void setRotation(DynamicAttribute * dynRotation);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** Returns a rotation set in the affector, depending on the type of dynamic attribute.
	    */
		Radian _calculateRotation();

		/** Returns a rotation speed value, depending on the type of dynamic attribute.
	    */
		Radian _calculateRotationSpeed(Particle * particle);

		/** @copydoc ParticleAffector::_initParticleForEmission */
		virtual void _initParticleForEmission(Particle * particle);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

	protected:
		bool mUseOwnRotationSpeed;
		Radian mScaledRotationSpeed;
		Radian twoPiRad;
		DynamicAttribute * mDynRotation;
		DynamicAttribute * mDynRotationSpeed;

		/** Helper factory
		*/
		DynamicAttributeFactory mDynamicAttributeFactory;
		DynamicAttributeHelper mDynamicAttributeHelper;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_TEXTURE_ROTATOR_H__ */
