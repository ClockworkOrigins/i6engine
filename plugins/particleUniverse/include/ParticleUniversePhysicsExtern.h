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

#ifndef __PU_PHYSICS_EXTERN_H__
#define __PU_PHYSICS_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExtern.h"
#include "ParticleUniversePhysicsActor.h"

namespace ParticleUniverse {

	/** PhysicsExtern is a pure virtual class that must be subclassed. The child class forms the connection between Particle Universe and
		the physics engine.
	*/
	class _ParticleUniverseExport PhysicsExtern : public Extern {
		public:
			PhysicsExtern() : Extern() {}
			virtual ~PhysicsExtern() {}

		/** As soon as a particle is emitted it can be processed by the PhysicsExtern. One of the actions is to assign a PhysicsActor and
			shape to the particle.
		*/
		virtual void _initParticleForEmission(Particle * particle) = 0;

		/** As soon as a particle is expired it can be processed by the PhysicsExtern. One of the actions is to remove a PhysicsActor from
			the particle.
		*/
		virtual void _initParticleForExpiration(Particle * particle) = 0;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PHYSICS_EXTERN_H__ */
