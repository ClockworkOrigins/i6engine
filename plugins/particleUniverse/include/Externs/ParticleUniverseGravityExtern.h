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

#ifndef __PU_GRAVITY_EXTERN_H__
#define __PU_GRAVITY_EXTERN_H__

#include "ParticleUniverseAttachable.h"

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

namespace ParticleUniverse {

	/** The GravityExtern is a wrapper of the GravityAffector, adding the functionality of a MovableObject.
		The GravityExtern can be attached to another SceneNode than the one where the ParticleSystem at 
		which the GravityExtern is registered, is attached. This makes it possible to affect the particles 
		in the Particle System, while both SceneNodes moves different from each other. This approach makes
		it possible to simulate something like a helicopter (SceneNode to which the GravityExtern is 
		attached) that flies over a certain area and moves the leaves on the ground (Particle System attached
		to another SceneNode).
    */
	class _ParticleUniverseExport GravityExtern : public Attachable, public GravityAffector {
		using GravityAffector::copyAttributesTo;

	public:
		GravityExtern() : Attachable(), GravityAffector() {}
	    virtual ~GravityExtern() {}

		/** The _preProcessParticles() function sets the position and mDerivedPosition attributes to
			the actual world position of the node to which it is attached.
	    */
		virtual void _preProcessParticles(ParticleTechnique * technique, Real timeElapsed);

		/** Processes a particle.
	    */
		virtual void _interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed);

		/** Copy both the Extern and the derived GravityAffector properties.
	    */
		virtual void copyAttributesTo(Extern * externObject);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_GRAVITY_EXTERN_H__ */
