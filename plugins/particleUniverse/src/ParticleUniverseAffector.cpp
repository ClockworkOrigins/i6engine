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

#include "ParticleUniverseAffector.h"

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const bool ParticleAffector::DEFAULT_ENABLED = true;
	const Vector3 ParticleAffector::DEFAULT_POSITION(0, 0, 0);
	const ParticleAffector::AffectSpecialisation ParticleAffector::DEFAULT_SPECIALISATION = ParticleAffector::AFSP_DEFAULT;

	ParticleAffector::ParticleAffector() : Particle(), IAlias(), IElement(), mParentTechnique(nullptr), mName(BLANK_STRING), mAffectSpecialisation(ParticleAffector::AFSP_DEFAULT), mExcludedEmitters(), _mAffectorScale(Vector3::UNIT_SCALE) {
		particleType = PT_AFFECTOR;
		mAliasType = AT_AFFECTOR;
	}

	void ParticleAffector::_notifyStart() {
		setEnabled(mOriginalEnabled);
	}

	void ParticleAffector::_notifyRescaled(const Vector3 & scale) {
		_mAffectorScale = scale;
	}

	void ParticleAffector::_initForEmission() {
		// The affector itself is emitted.
		Particle::_initForEmission();

		// Emitting an affector is similar as starting one.
		_notifyStart();
	}

	void ParticleAffector::_initForExpiration(ParticleTechnique * technique, Real timeElapsed) {
		// The affector itself is expired.
		Particle::_initForExpiration(technique, timeElapsed);

		// Expiring an affector is similar as stopping one.
		_notifyStop();
	}

	void ParticleAffector::copyAttributesTo(ParticleAffector * affector) {
		copyParentAttributesTo(affector);
	}

	void ParticleAffector::copyParentAttributesTo(ParticleAffector * affector) {
		// Copy Parent attributes
		Particle::copyAttributesTo(affector);

		affector->setName(mName);
		affector->mParentTechnique = mParentTechnique;
		affector->_mAffectorScale = _mAffectorScale;
		affector->mAffectSpecialisation = mAffectSpecialisation;
		affector->mExcludedEmitters = list<String>(mExcludedEmitters); // Use copy constructor
	}

	void ParticleAffector::setParentTechnique(ParticleTechnique * parentTechnique) {
		mParentTechnique = parentTechnique;
	}

	void ParticleAffector::_processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
		// Call the _firstParticle() function if the first particle in the update loop is encountered.
		if (firstParticle) {
			// Perform a precalculation at the first particle
			_firstParticle(particleTechnique, particle, timeElapsed);
		}

		if (!mExcludedEmitters.empty() && particle->parentEmitter) {
			// Return if the emitter which emits this particle is part of the list
			String emitterName = particle->parentEmitter->getName();
			list<String>::iterator it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
			if (it != mExcludedEmitters.end()) {
				return;
			}
		}

		_affect(particleTechnique, particle, timeElapsed);
	}

	void ParticleAffector::addEmitterToExclude(const String & emitterName) {
		list<String>::iterator it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
		if (it == mExcludedEmitters.end()) {
			mExcludedEmitters.push_back(emitterName);
		}
	}

	void ParticleAffector::removeEmitterToExclude(const String & emitterName) {
		list<String>::iterator it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
		if (it != mExcludedEmitters.end()) {
			mExcludedEmitters.erase(it);
		}
	}

	void ParticleAffector::removeAllEmittersToExclude() {
		mExcludedEmitters.clear();
	}

	const list<String> & ParticleAffector::getEmittersToExclude() const {
		return mExcludedEmitters;
	}

	bool ParticleAffector::hasEmitterToExclude(const String & emitterName) {
		list<String>::iterator it = std::find(mExcludedEmitters.begin(), mExcludedEmitters.end(), emitterName);
		return (it != mExcludedEmitters.end());
	}

	const Vector3 & ParticleAffector::getDerivedPosition() {
		if (mMarkedForEmission) {
			// Use the affector position, because it is emitted
			// If a particle is emitted, position and derived position are the same
			mDerivedPosition = position;
		} else {
			// Add the techniques' derived position
			mDerivedPosition = mParentTechnique->getDerivedPosition() + mParentTechnique->getParentSystem()->getDerivedOrientation() * (_mAffectorScale * position);
		}
		return mDerivedPosition;
	}

	Real ParticleAffector::_calculateAffectSpecialisationFactor(const Particle * particle) {
		// Assume that particle->totalTimeToLive != 0, which is reasonable
		switch (mAffectSpecialisation) {
		case AFSP_DEFAULT: {
			return 1.0;
			break;
		}

		// This means that older particles will be affected MORE than just emitted particles
		case AFSP_TTL_INCREASE: {
			if (particle) {
				return particle->timeFraction;
			} else {
				return 1.0;
			}
			break;
		}

		// This means that older particles will be affected LESS than just emitted particles
		case AFSP_TTL_DECREASE: {
			if (particle) {
				return 1.0 - particle->timeFraction;
			} else {
				return 1.0;
			}
			break;
		}

		default: {
			return 1.0;
			break;
		}
		}
	}

} /* namespace ParticleUniverse */
