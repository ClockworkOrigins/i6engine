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

#ifndef __PU_SPHERE_COLLIDER_EXTERN_H__
#define __PU_SPHERE_COLLIDER_EXTERN_H__

#include "ParticleUniverseAttachable.h"

#include "ParticleAffectors/ParticleUniverseSphereCollider.h"

namespace ParticleUniverse {

	/** The SphereColliderExtern is a wrapper of the SphereCollider, adding the functionality of a MovableObject.
		This makes it possible to let particles collide with a SphereCollider that is attached to a different 
		SceneNode, than the ParticleSystem with which particles it collides.
    */
	class _ParticleUniverseExport SphereColliderExtern : public Attachable, public SphereCollider {
		using SphereCollider::copyAttributesTo;

	public:
		SphereColliderExtern() : Attachable(), SphereCollider() {}
	    virtual ~SphereColliderExtern() {}

		/** see Extern::_preProcessParticles */
		virtual void _preProcessParticles(ParticleTechnique * technique, Real timeElapsed);

		/** see Extern::_interface */
		virtual void _interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed);

		/** Copy both the Extern and the derived SphereCollider properties.
	    */
		virtual void copyAttributesTo(Extern * externObject);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SPHERE_COLLIDER_EXTERN_H__ */
