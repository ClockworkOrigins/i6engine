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

#ifndef __PU_PHYSICS_ACTOR_H__
#define __PU_PHYSICS_ACTOR_H__

#include "ParticleUniversePhysicsShape.h"

namespace ParticleUniverse {

	/** PhysicsActorDesc contains the properties of the actor and is used to create a actor.
	*/
	class _ParticleUniverseExport PhysicsActorDesc {
	public:
		// Constructor
		PhysicsActorDesc() : position(Vector3::ZERO), direction(Vector3::ZERO), orientation(Quaternion::IDENTITY), mass(1.0), collisionGroup(0) {}

		// Destructor
		virtual ~PhysicsActorDesc() {}

		// Public attributes
		Vector3 position;
		Vector3 direction;
		Quaternion orientation;
		Real mass;
		uint16 collisionGroup;
	};

	/** A PhysicsActor is an element of the physical world and has no visual representation by itself. It only has physical properties. 
		The PhysicsActor is an abstraction class of the real underlying physics engine, such as PhysX, Havok or Bullet.
	*/
	class _ParticleUniverseExport PhysicsActor {
	public:
		// Constructor
		PhysicsActor() : position(Vector3::ZERO), direction(Vector3::ZERO), orientation(Quaternion::IDENTITY), mass(1.0), collisionGroup(0), contactPoint(Vector3::ZERO), contactNormal(Vector3::ZERO) {}

		// Destructor
		virtual ~PhysicsActor() {}

		// Public attributes
		Vector3 position;
		Vector3 direction;
		Quaternion orientation;
		Real mass;
		uint16 collisionGroup;
		Vector3 contactPoint;
		Vector3 contactNormal;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PHYSICS_ACTOR_H__ */
