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

#include "ParticleEmitters/ParticleUniverseSlaveEmitter.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

#include "ParticleBehaviours/ParticleUniverseSlaveBehaviour.h"

namespace ParticleUniverse {
	
	SlaveEmitter::SlaveEmitter() :  ParticleEmitter(), TechniqueListener(), mMasterParticle(nullptr), mMasterTechniqueName(BLANK_STRING), mMasterEmitterName(BLANK_STRING), mMasterPosition(Vector3::ZERO), mMasterDirection(Vector3::ZERO), mMasterEmitterNameSet(false) {
	}
	
	const String & SlaveEmitter::getMasterTechniqueName() const {
		return mMasterTechniqueName;
	}
	
	void SlaveEmitter::setMasterTechniqueName(const String & masterTechniqueName) {
		mMasterTechniqueName = masterTechniqueName;
	}
	
	const String & SlaveEmitter::getMasterEmitterName() const {
		return mMasterEmitterName;
	}
	
	void SlaveEmitter::setMasterEmitterName(const String & masterEmitterName) {
		mMasterEmitterName = masterEmitterName;
		mMasterEmitterNameSet = true;
	}
	
	void SlaveEmitter::particleEmitted(ParticleTechnique * particleTechnique, Particle * particle) {
		if (mMasterEmitterNameSet && mMasterEmitterName != particle->parentEmitter->getName()) {
			// Ignore particle
			return;
		}

		/** Keep the data of the emitted particle from the master technique/emitter.
			Emission if a particle in this emitter may NOT be done in the main _update() method of the ParticleSystem,
			but only from here (it´s a slave afterall). That is why the emitter is enabled and disabled again.
		*/
		mMasterPosition = particle->position;
		mMasterDirection = particle->direction;
		mMasterParticle = particle;
		mEnabled = true;
		mParentTechnique->forceEmission(this, 1); // Just emit one, to be in sync with the master.
		mEnabled = false;
	}
	
	void SlaveEmitter::_initParticlePosition(Particle * particle) {
		// Remark: Don't take the orientation of the node into account, because the position of the master particle is leading.
		particle->position = mMasterPosition;
		particle->originalPosition = particle->position;
	}
	
    void SlaveEmitter::_initParticleDirection(Particle * particle) {
		particle->direction = mMasterDirection;
		particle->originalDirection = particle->direction;
		particle->originalDirectionLength = particle->direction.length();

		// Make use of the opportunity to set the master particle in the behaviour object (if available)
		SlaveBehaviour * behaviour = static_cast<SlaveBehaviour *>(particle->getBehaviour("Slave"));
		if (behaviour) {
			behaviour->masterParticle = mMasterParticle;
		}
    }
	
	void SlaveEmitter::_prepare(ParticleTechnique * particleTechnique) {
		ParticleSystem * system = particleTechnique->getParentSystem();
		if (system) {
			ParticleTechnique * masterTechnique = system->getTechnique(mMasterTechniqueName);
			if (masterTechnique) {
				masterTechnique->addTechniqueListener(this);
			}
			mEnabled = false;
		}
	}
	
	void SlaveEmitter::_unprepare(ParticleTechnique * particleTechnique) {
		ParticleSystem * system = particleTechnique->getParentSystem();
		if (system) {
			ParticleTechnique * masterTechnique = system->getTechnique(mMasterTechniqueName);
			if (masterTechnique) {
				masterTechnique->removeTechniqueListener(this);
			}
		}
	}
	
	void SlaveEmitter::_notifyStart() {
		ParticleEmitter::_notifyStart();
		setEnabled(false);
	}
	
	void SlaveEmitter::copyAttributesTo (ParticleEmitter * emitter) {
		ParticleEmitter::copyAttributesTo(emitter);

		SlaveEmitter * slaveEmitter = static_cast<SlaveEmitter *>(emitter);
		slaveEmitter->mMasterTechniqueName = mMasterTechniqueName;
		slaveEmitter->mMasterEmitterName = mMasterEmitterName;
		slaveEmitter->mMasterEmitterNameSet = mMasterEmitterNameSet;
	}

} /* namespace ParticleUniverse */
