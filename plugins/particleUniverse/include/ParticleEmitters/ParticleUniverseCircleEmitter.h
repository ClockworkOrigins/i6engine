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

#ifndef __PU_CIRCLE_EMITTER_H__
#define __PU_CIRCLE_EMITTER_H__

#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse {

	/** The CircleEmitter is a ParticleEmitter that emits particles on a circle shape. Particle can be emitted
		random on the circle, but it can also follow the circles' contours.
    */
	class _ParticleUniverseExport CircleEmitter : public ParticleEmitter {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_RADIUS;
		static const Real DEFAULT_STEP;
		static const Real DEFAULT_ANGLE;
		static const bool DEFAULT_RANDOM;
		static const Vector3 DEFAULT_NORMAL;

		CircleEmitter();
	    virtual ~CircleEmitter() {}

		/** 
	    */
		Real getRadius() const;
		void setRadius(const Real radius);

		/** 
	    */
		Real getCircleAngle() const;
		void setCircleAngle(const Real circleAngle);

		/** 
	    */
		Real getStep() const;
		void setStep(const Real step);

		/** 
	    */
		bool isRandom() const;
		void setRandom(const bool random);

		/* 
		*/ 
		const Quaternion & getOrientation() const;
		const Vector3 & getNormal() const;
		void setNormal(const Vector3 & normal); 

		/** See ParticleEmiter
	    */
		void _notifyStart();

		/** Determine a particle position on the circle.
	    */
		virtual void _initParticlePosition(Particle * particle);

		/** Determine the particle direction.
	    */
		virtual void _initParticleDirection(Particle * particle);

		/** 
	    */
		virtual void copyAttributesTo(ParticleEmitter * emitter);

	protected:
		Real mRadius;
		Real mCircleAngle;
		Real mOriginalCircleAngle;
		Real mStep;
		Real mX;
		Real mZ;
		bool mRandom;
		Quaternion mOrientation;
		Vector3 mNormal;
	};

}

#endif /* __PU_CIRCLE_EMITTER_H__ */
