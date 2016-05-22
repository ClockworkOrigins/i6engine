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

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseObserver.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {
	
	DoEnableComponentEventHandler::DoEnableComponentEventHandler() : ParticleEventHandler(), mComponentName(BLANK_STRING), mComponentType(CT_EMITTER), mComponentEnabled(true) {
	}
	
	void DoEnableComponentEventHandler::_handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		/** Search for the component.
		*/
		ParticleTechnique * technique = nullptr;
		switch (mComponentType) {
		case CT_EMITTER: {
			ParticleEmitter * emitter = particleTechnique->getEmitter(mComponentName);
			if (!emitter) {
				// Search all techniques in this ParticleSystem for an emitter with the correct name
				ParticleSystem * system = particleTechnique->getParentSystem();
				size_t size = system->getNumTechniques();
				for (size_t i = 0; i < size; ++i) {
					technique = system->getTechnique(i);
					emitter = technique->getEmitter(mComponentName);
					if (emitter) {
						break;
					}
				}
			}
			if (emitter) {
				emitter->setEnabled(mComponentEnabled);
			}
			break;
		}
		case CT_AFFECTOR: {
			ParticleAffector * affector = particleTechnique->getAffector(mComponentName);
			if (!affector) {
				// Search all techniques in this ParticleSystem for an affector with the correct name
				ParticleSystem * system = particleTechnique->getParentSystem();
				size_t size = system->getNumTechniques();
				for (size_t i = 0; i < size; ++i) {
					technique = system->getTechnique(i);
					affector = technique->getAffector(mComponentName);
					if (affector) {
						break;
					}
				}
			}
			if (affector) {
				affector->setEnabled(mComponentEnabled);
			}
			break;
		}
		case CT_OBSERVER: {
			ParticleObserver * observer = particleTechnique->getObserver(mComponentName);
			if (!observer) {
				// Search all techniques in this ParticleSystem for an observer with the correct name
				ParticleSystem * system = particleTechnique->getParentSystem();
				size_t size = system->getNumTechniques();
				for (size_t i = 0; i < size; ++i) {
					technique = system->getTechnique(i);
					observer = technique->getObserver(mComponentName);
					if (observer) {
						break;
					}
				}
			}
			if (observer) {
				observer->setEnabled(mComponentEnabled);
			}
			break;
		}
		case CT_TECHNIQUE: {
			// Search in this ParticleSystem for a technique with the correct name
			ParticleSystem * system = particleTechnique->getParentSystem();
			technique = system->getTechnique(mComponentName);
			if (technique) {
				technique->setEnabled(mComponentEnabled);
			}
		}
		break;
		default: {
			break;
		}
		}
	}
	
	void DoEnableComponentEventHandler::copyAttributesTo(ParticleEventHandler * eventHandler) {
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoEnableComponentEventHandler * doEnableComponentEventHandler = static_cast<DoEnableComponentEventHandler *>(eventHandler);
		doEnableComponentEventHandler->setComponentType(mComponentType);
		doEnableComponentEventHandler->setComponentName(mComponentName);
		doEnableComponentEventHandler->setComponentEnabled(mComponentEnabled);
	}

} /* namespace ParticleUniverse */
