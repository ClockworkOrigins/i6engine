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

#ifndef __PU_PARTICLE_POOL_H__
#define __PU_PARTICLE_POOL_H__

#include "ParticleUniversePool.h"
#include "ParticleUniversePoolMap.h"
#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	/** The ParticlePool is a container class that includes other pools. The ParticlePool acts as one pool
		with different types of particles.
    */
	class _ParticleUniverseExport ParticlePool {
	public:
		ParticlePool() : mLatestParticle(nullptr) {}
		virtual ~ParticlePool();

		/** 
		*/
		void setParentTechnique(ParticleTechnique * parentTechnique);

		/** 
		*/
		bool isEmpty();

		/** 
		*/
		bool isEmpty(const Particle::ParticleType particleType);

		/** 
		*/
		size_t getSize();

		/** 
		*/
		size_t getSize(const Particle::ParticleType particleType);

		/** 
		*/
		void initialisePool();

		/** 
		*/
		void increasePool(const Particle::ParticleType particleType, size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique);

		/** Destroy particles of a certain type
		*/
		void destroyParticles(const Particle::ParticleType particleType);
		void destroyAllVisualParticles();
		void destroyAllEmitterParticles();
		void destroyAllTechniqueParticles();
		void destroyAllAffectorParticles();
		void destroyAllSystemParticles();

		/** Releases a particle from the pool
		*/
		Particle * releaseParticle(const Particle::ParticleType particleType, const String & name);

		/** Releases all particles from the pool
		*/
		void releaseAllParticles ();

		/** 
		*/
		void lockLatestParticle ();

		/** Lock all particles in the pool
		*/
		void lockAllParticles ();

		/** 
		*/
		void resetIterator();

		/** 
		*/
		Particle * getFirst();


		/** 
		*/
		Particle * getNext();

		/** 
		*/
		Particle * getFirst(const Particle::ParticleType particleType);


		/** 
		*/
		Particle * getNext(const Particle::ParticleType particleType);

		/** 
		*/
		bool end();

		/** 
		*/
		bool end(const Particle::ParticleType particleType);

		/** 
		*/
		Pool<VisualParticle>::PoolList & getVisualParticlesList() {
			return mVisualParticlesPool.getActiveElementsList();
		}

	protected:
		ParticleTechnique * mParentTechnique;

		/** The Pools
		*/
		Pool<VisualParticle> mVisualParticlesPool;
		PoolMap<ParticleEmitter> mParticleEmitterPool;
		PoolMap<ParticleAffector> mParticleAffectorPool;
		PoolMap<ParticleSystem> mParticleSystemPool;
		PoolMap<ParticleTechnique> mParticleTechniquePool;

		/** Vectors used for deletion of created objects
		*/
		vector<VisualParticle *> mVisualParticles;
		vector<ParticleEmitter *> mEmitters;
		vector<ParticleAffector *> mAffectors;
		vector<ParticleTechnique *> mTechniques;
		vector<String> mSystems; // Use names instead of pointers to avoid problems during cleanup.

		/** Protected methods to increase the pools
		*/
		void _increaseVisualParticlePool(size_t size, Particle::ParticleBehaviourList & behaviours);
		void _increaseParticleEmitterPool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique);
		void _increaseParticleAffectorPool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique);
		void _increaseParticleTechniquePool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleSystem * system);
		void _increaseParticleSystemPool(size_t size, Particle::ParticleBehaviourList & behaviours, ParticleTechnique * technique);

		Particle * mLatestParticle;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PARTICLE_POOL_H__ */
