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

#ifndef __PU_SPHERE_SURFACE_EMITTER_H__
#define __PU_SPHERE_SURFACE_EMITTER_H__

#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse {

	/** The SphereSurfaceEmitter emits particles from the surface of a sphere (instead within the sphere�s
		volume). The particles are emitted in a direction perpendicular to the tangentvector where 
		the particle emits. Using the angle attribute, the direction can be influenced.
    */
	class _ParticleUniverseExport SphereSurfaceEmitter : public ParticleEmitter {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_RADIUS;

		SphereSurfaceEmitter();
	    virtual ~SphereSurfaceEmitter() {}

		/** 
	    */
		Real getRadius() const;
		void setRadius(const Real radius);

		/** 
	    */
		virtual void _initParticlePosition(Particle * particle);

		/** 
	    */
		virtual void _initParticleDirection(Particle * particle);
			
		/** 
	    */
		virtual void copyAttributesTo(ParticleEmitter * emitter);

	protected:
		Real mRadius;
		Vector3 mRandomVector;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SPHERE_SURFACE_EMITTER_H__ */
