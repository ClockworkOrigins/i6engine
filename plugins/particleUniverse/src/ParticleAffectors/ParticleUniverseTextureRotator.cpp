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

#include "ParticleAffectors/ParticleUniverseTextureRotator.h"

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const bool TextureRotator::DEFAULT_USE_OWN_SPEED = false;
	const Real TextureRotator::DEFAULT_ROTATION_SPEED = 10.0;
	const Real TextureRotator::DEFAULT_ROTATION = 0.0;

	TextureRotator::TextureRotator() : ParticleAffector(), mUseOwnRotationSpeed(DEFAULT_USE_OWN_SPEED), mScaledRotationSpeed(Radian(0)), twoPiRad(Radian(Math::TWO_PI)) {
		mDynRotation = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		static_cast<DynamicAttributeFixed *>(mDynRotation)->setValue(DEFAULT_ROTATION);
		mDynRotationSpeed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		static_cast<DynamicAttributeFixed *>(mDynRotationSpeed)->setValue(DEFAULT_ROTATION_SPEED);
	}
	
	TextureRotator::~TextureRotator() {
		if (mDynRotation) {
			PU_DELETE_T(mDynRotation, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynRotation = nullptr;
		}

		if (mDynRotationSpeed) {
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynRotationSpeed = nullptr;
		}
	}
	
	bool TextureRotator::useOwnRotationSpeed() const {
		return mUseOwnRotationSpeed;
	}
	
	void TextureRotator::setUseOwnRotationSpeed(bool uors) {
		mUseOwnRotationSpeed = uors;
	}
	
	DynamicAttribute * TextureRotator::getRotation() const {
		return mDynRotation;
	}
	
	void TextureRotator::setRotation(DynamicAttribute * dynRotation) {
		if (mDynRotation) {
			PU_DELETE_T(mDynRotation, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		}

		mDynRotation = dynRotation;
	}
	
	DynamicAttribute * TextureRotator::getRotationSpeed() const {
		return mDynRotationSpeed;
	}
	
	void TextureRotator::setRotationSpeed(DynamicAttribute * dynRotationSpeed) {
		if (mDynRotationSpeed) {
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		}

		mDynRotationSpeed = dynRotationSpeed;
	}
	
	Radian TextureRotator::_calculateRotation() {
		return Radian(mDynamicAttributeHelper.calculate(mDynRotation, mParentTechnique->getParentSystem()->getTimeElapsedSinceStart()));
	}
	
	Radian TextureRotator::_calculateRotationSpeed(Particle * particle) {
		return Radian(mDynamicAttributeHelper.calculate(mDynRotationSpeed, particle->timeFraction));
	}
	
	void TextureRotator::_initParticleForEmission(Particle * particle) {
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL) {
			return;
		}

		// Set initial random zRotation
		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
		visualParticle->zRotation = _calculateRotation();
		if (particle->parentEmitter->getParentTechnique()->getRenderer()) {
			// Assume that all parents exist. That must be the case otherwise particles are not emitted.
			particle->parentEmitter->getParentTechnique()->getRenderer()->_notifyParticleZRotated();
		}

		// Set the zRotationSpeed
		visualParticle->zRotationSpeed = _calculateRotationSpeed(particle);
	}
	
	void TextureRotator::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL) {
			return;
		}

		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
		if (mUseOwnRotationSpeed) {
			// Use scaled rotationspeed and adjust the speed according to the velocity
			mScaledRotationSpeed = visualParticle->zRotationSpeed * timeElapsed;
		} else {
			// Scale speed
			mScaledRotationSpeed = _calculateRotationSpeed(particle) * timeElapsed;
		}

		visualParticle->zRotation += mScaledRotationSpeed;
		visualParticle->zRotation = visualParticle->zRotation > twoPiRad ? visualParticle->zRotation - twoPiRad : visualParticle->zRotation;
		if (particleTechnique->getRenderer()) {
			particleTechnique->getRenderer()->_notifyParticleZRotated();
		}
	}
	
	void TextureRotator::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		TextureRotator * textureRotator = static_cast<TextureRotator *>(affector);
		textureRotator->setRotation(mDynamicAttributeFactory.cloneDynamicAttribute(getRotation()));
		textureRotator->setRotationSpeed(mDynamicAttributeFactory.cloneDynamicAttribute(getRotationSpeed()));
		textureRotator->mUseOwnRotationSpeed = mUseOwnRotationSpeed;
	}

} /* namespace ParticleUniverse */
