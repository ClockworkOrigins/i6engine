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

#ifndef __PU_PHYSX_EXTERN_H__
#define __PU_PHYSX_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "ParticleUniversePhysicsExtern.h"
	#include "NxPhysics.h"
	#include "NxActor.h"
	#include "NxScene.h"

	namespace ParticleUniverse
	{
		class _ParticleUniverseExport PhysXActor : public PhysicsActor
		{
			public:
				// Constructor
				PhysXActor(void) : 
					PhysicsActor(),
					nxActor(0),
					nxParticleData(0){};

				// Destructor
				virtual ~PhysXActor(void) {};

				NxActor* nxActor;
				NxParticleData* nxParticleData;
		};
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		class ParticleStructure
		{
			public:
				// Constructor and Destructor
				ParticleStructure(void) : 
					lifetime(0),
					id(0),
					density(0),
					bufferFlag(0),
					particle(0){};
				virtual ~ParticleStructure(void) {};

				// Public attributes
				NxVec3	position;
				NxVec3  velocity;
				NxReal  lifetime;
				NxVec3	collisionNormal;
				NxU32	id;
				NxU32   bufferFlag;
				NxReal	density;
				Particle* particle;
		};
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		/** The PhysXExtern class is a base class for PhysX.
		*/
		class _ParticleUniverseExport PhysXExtern : public PhysicsExtern
		{
			public:
				PhysXExtern(void) : mCollisionGroup(0){};
				virtual ~PhysXExtern(void){};

				/** This function is called by the PhysXBridge. Every time it is called, the physics are updated.
				*/
				virtual void synchronize(Real timeElapsed) = 0;

				/** Get the collision group.
				*/
				virtual uint16 getCollisionGroup(void) const
				{
					return mCollisionGroup;
				};
				
				/** Set the collision group.
				*/
				virtual void setCollisionGroup(uint16 collisionGroup)
				{
					mCollisionGroup = collisionGroup;
				};

			protected:
				uint16 mCollisionGroup;
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
