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

#include "ParticleObservers/ParticleUniverseOnQuotaObserver.h"

#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {
	
	bool OnQuotaObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		return mResult;
	}
	
	void OnQuotaObserver::_postProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		mResult = false;
		size_t quota = 0;
		if (mParticleTypeToObserveSet) {
			// Type to observe is set, so validate only that one
			switch (mParticleTypeToObserve) {
			case Particle::PT_VISUAL: {
				quota = particleTechnique->getVisualParticleQuota();
				break;
			}
			case Particle::PT_EMITTER: {
				quota = particleTechnique->getEmittedEmitterQuota();
				break;
			}
			case Particle::PT_AFFECTOR: {
				quota = particleTechnique->getEmittedAffectorQuota();
				break;
			}
			case Particle::PT_TECHNIQUE: {
				quota = particleTechnique->getEmittedTechniqueQuota();
				break;
			}
			case Particle::PT_SYSTEM: {
				quota = particleTechnique->getEmittedAffectorQuota();
				break;
			}
			default: {
				break;
			}
			}

			mResult = particleTechnique->getNumberOfEmittedParticles(mParticleTypeToObserve) >= quota;
		} else {
			// Type to observe is not set, so check them all
			quota = particleTechnique->getVisualParticleQuota() + particleTechnique->getEmittedEmitterQuota() + particleTechnique->getEmittedTechniqueQuota() + particleTechnique->getEmittedAffectorQuota() + particleTechnique->getEmittedSystemQuota();
			mResult = particleTechnique->getNumberOfEmittedParticles() >= quota;
		}
	}

} /* namespace ParticleUniverse */
