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

#ifndef __PU_PHYSX_ACTOR_EXTERN_H__
#define __PU_PHYSX_ACTOR_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "ParticleUniversePhysXExtern.h"
	#include "NxPhysics.h"
	#include "NxActor.h"
	#include "NxScene.h"

	namespace ParticleUniverse
	{
		/** The PhysXActorExtern class is responsible for connecting a physical object to a particle that was emitted by Particle Universe.
		*/
		class _ParticleUniverseExport PhysXActorExtern : public PhysXExtern
		{
			public:
				PhysXActorExtern(void);
				virtual ~PhysXActorExtern(void);

				/** See PhysicsExtern
				*/
				virtual void _initParticleForEmission(Particle* particle);

				/** See PhysicsExtern
				*/
				virtual void _initParticleForExpiration(Particle* particle);

				/** See PhysicsExtern
				*/
				virtual PhysicsActor* createPhysicsActor(PhysicsActorDesc* physicsActorDesc, PhysicsShapeDesc* physicsShapeDesc);

				/** See PhysicsExtern
				*/
				virtual void destroyPhysicsActor(PhysicsActor* physicsActor);

				/** See Extern
				*/
				virtual void _prepare(ParticleTechnique* technique);

				/** See Extern
				*/
				virtual void _unprepare(ParticleTechnique* technique);

				/** See Extern: Override the implementation of Extern
				*/
				virtual void _processParticle(
					ParticleTechnique* particleTechnique, 
					Particle* particle, 
					Real timeElapsed, 
					bool firstParticle);

				/** See Extern: Ignore
				*/
				virtual void _interface(ParticleTechnique* technique, 
					Particle* particle, 
					Real timeElapsed){};

				/** See Extern
				*/
				virtual void _postProcessParticles(ParticleTechnique* technique, Real timeElapsed);

				/** See PhysXEntern
				*/
				virtual void synchronize(Real timeElapsed);

				/** 
				*/
				virtual void copyAttributesTo (Extern* externObject);

				/** Set the physics shape.
				*/
				void setPhysicsShape(PhysicsShapeDesc* physicsShapeDesc);

				/** Public attributes.
				*/
				PhysicsShapeDesc* mPhysicsShapeDesc; // Defines the physical shape of the particles; only one shape is allowed.

			protected:

				NxScene* mScene;
				bool mSynchronize; // Flag that is set by the PhysXBridge if the scene is simulated.
				Real mTimeElapsedSynchronize;
				NxU32 mMaxParticles;
				ParticleStructure* mPhysXParticles;
				NxU32 mPhysXParticlesPointer;
				NxU32 mStart;
				NxParticleData mParticles;
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
