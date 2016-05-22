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

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const bool DoAffectorEventHandler::DEFAULT_PRE_POST = false;

	DoAffectorEventHandler::DoAffectorEventHandler() : ParticleEventHandler(), mAffectorName(BLANK_STRING), mPrePost(DEFAULT_PRE_POST) {
	}
	
	void DoAffectorEventHandler::_handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		/** Search for the affector.
		*/
		ParticleTechnique * technique = nullptr;
		ParticleAffector * affector = particleTechnique->getAffector(mAffectorName);
		if (!affector) {
			// Search all techniques in this ParticleSystem for an affector with the correct name
			ParticleSystem * system = particleTechnique->getParentSystem();
			size_t size = system->getNumTechniques();
			for (size_t i = 0; i < size; ++i) {
				technique = system->getTechnique(i);
				affector = technique->getAffector(mAffectorName);
				if (affector) {
					break;
				}
			}
		}

		if (affector) {
			// Call the affector even if it has enabled set to 'false'.
			if (mPrePost) {
				affector->_preProcessParticles(particleTechnique, timeElapsed);
				affector->_affect(particleTechnique, particle, timeElapsed);
				affector->_postProcessParticles(particleTechnique, timeElapsed);
			} else {
				affector->_affect(particleTechnique, particle, timeElapsed);
			}
		}
	}
	
	void DoAffectorEventHandler::copyAttributesTo(ParticleEventHandler * eventHandler) {
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoAffectorEventHandler * doAffectorEventHandler = static_cast<DoAffectorEventHandler *>(eventHandler);
		doAffectorEventHandler->setAffectorName(mAffectorName);
		doAffectorEventHandler->setPrePost(mPrePost);
	}

} /* namespace ParticleUniverse */
