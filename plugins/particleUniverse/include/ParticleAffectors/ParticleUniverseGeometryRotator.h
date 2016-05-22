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

#ifndef __PU_GEOMETRY_ROTATOR_H__
#define __PU_GEOMETRY_ROTATOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse {

	/** The GeometryRotator rotates particles around its orientation axis. The rotation speed can be 
		adjusted and can be set to a 'global' rotation speed, which affects all particles in the
		Particle Technique the same way. It is also possible to use the rotation speed of the particles
		itself.
    */
	class _ParticleUniverseExport GeometryRotator : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const bool DEFAULT_USE_OWN;
		static const Real DEFAULT_ROTATION_SPEED;
		static const Vector3 DEFAULT_ROTATION_AXIS;

		GeometryRotator();
	    virtual ~GeometryRotator();

		/** Returns the rotation speed. This is the speed controlled by the affector. Besides
			the default rotation speed, it is also possible to use the particles own rotation speed.
	    */
		DynamicAttribute * getRotationSpeed() const;

		/** 
	    */
		void setRotationSpeed(DynamicAttribute * dynRotationSpeed);

		/** Returns an indication whether the rotation speed is the same for all particles in this 
			particle technique, or whether the rotation speed of the particle itself is used.
	    */
		bool useOwnRotationSpeed() const;

		/** Set the indication whether rotation speed of the particle itself is used.
	    */
		void setUseOwnRotationSpeed(bool useOwnRotationSpeed);

		/** 
	    */
		const Vector3 & getRotationAxis() const;

		/** 
	    */
		void setRotationAxis(const Vector3 & rotationAxis);

		/** 
	    */
		void resetRotationAxis();

		/** Returns a rotation speed value, depending on the type of dynamic attribute.
	    */
		Real _calculateRotationSpeed(Particle * particle);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** @copydoc ParticleAffector::_initParticleForEmission */
		virtual void _initParticleForEmission(Particle * particle);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

	protected:
		Real mScaledRotationSpeed;
		bool mUseOwnRotationSpeed;
		DynamicAttribute * mDynRotationSpeed;
		Quaternion mQ;
		Vector3 mRotationAxis;
		bool mRotationAxisSet;

		/** Helper factory
		*/
		DynamicAttributeFactory mDynamicAttributeFactory;
		DynamicAttributeHelper mDynamicAttributeHelper;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_GEOMETRY_ROTATOR_H__ */
