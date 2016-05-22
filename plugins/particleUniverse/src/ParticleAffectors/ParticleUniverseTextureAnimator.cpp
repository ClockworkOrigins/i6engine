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

#include "ParticleAffectors/ParticleUniverseTextureAnimator.h"

#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const Real TextureAnimator::DEFAULT_TIME_STEP = 0.0;
	const uint16 TextureAnimator::DEFAULT_TEXCOORDS_START = 0;
	const uint16 TextureAnimator::DEFAULT_TEXCOORDS_END = 0;
	const TextureAnimator::TextureAnimationType TextureAnimator::DEFAULT_ANIMATION_TYPE = TextureAnimator::TAT_LOOP;
	const bool TextureAnimator::DEFAULT_START_RANDOM = true;

	TextureAnimator::TextureAnimator() : ParticleAffector(), mAnimationTimeStep(DEFAULT_TIME_STEP), mAnimationTimeStepCount(0.0), mStartRandom(DEFAULT_START_RANDOM), mAnimationTimeStepSet(false), mNextIndex(false), mTextureAnimationType(DEFAULT_ANIMATION_TYPE), mTextureCoordsStart(DEFAULT_TEXCOORDS_START), mTextureCoordsEnd(DEFAULT_TEXCOORDS_END) {
	}
	
	TextureAnimator::~TextureAnimator() {
	}
	
	Real TextureAnimator::getAnimationTimeStep() const {
		return mAnimationTimeStep;
	}
	
	void TextureAnimator::setAnimationTimeStep(Real animationTimeStep) {
		mAnimationTimeStep = animationTimeStep;
		mAnimationTimeStepSet = true;
	}
	
	TextureAnimator::TextureAnimationType TextureAnimator::getTextureAnimationType() const {
		return mTextureAnimationType;
	}
	
	void TextureAnimator::setTextureAnimationType(TextureAnimator::TextureAnimationType textureAnimationType) {
		mTextureAnimationType = textureAnimationType;
	}
	
	uint16 TextureAnimator::getTextureCoordsStart() const {
		return mTextureCoordsStart;
	}
	
	void TextureAnimator::setTextureCoordsStart(uint16 textureCoordsStart) {
		mTextureCoordsStart = textureCoordsStart;
	}
	
	uint16 TextureAnimator::getTextureCoordsEnd() const {
		return mTextureCoordsEnd;
	}
	
	void TextureAnimator::setTextureCoordsEnd(uint16 textureCoordsEnd) {
		mTextureCoordsEnd = textureCoordsEnd;
	}
	
	bool TextureAnimator::isStartRandom() const {
		return mStartRandom;
	}
	
	void TextureAnimator::setStartRandom(bool startRandom) {
		mStartRandom = startRandom;
	}
	
	void TextureAnimator::_initParticleForEmission(Particle * particle) {
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL) {
			return;
		}

		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);

		// Set first image
		if (mStartRandom) {
			visualParticle->textureCoordsCurrent = uint16(Math::RangeRandom(Real(mTextureCoordsStart), Real(mTextureCoordsEnd) + 0.999f));
		} else {
			visualParticle->textureCoordsCurrent = mTextureCoordsStart;
		}

		// Calculate the animationTimeStep
		if (!mAnimationTimeStepSet) {
			// Set the animation time step for each particle
			switch (mTextureAnimationType) {
			case TAT_LOOP: {
				visualParticle->textureAnimationTimeStep = visualParticle->timeToLive / (mTextureCoordsEnd - mTextureCoordsStart + 1);
				break;
			}
			case TAT_UP_DOWN: {
				visualParticle->textureAnimationTimeStep = visualParticle->timeToLive / (2 * (mTextureCoordsEnd - mTextureCoordsStart) + 1);
				break;
			}
			case TAT_RANDOM: {
				visualParticle->textureAnimationTimeStep = visualParticle->timeToLive;
				break;
			}
			default: {
				break;
			}
			}
		}
	}
	
	void TextureAnimator::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Determine the next texture coords index (global)
		if (mAnimationTimeStepSet) {
			mNextIndex = false;
			mAnimationTimeStepCount += timeElapsed;
			if (mAnimationTimeStepCount > mAnimationTimeStep) {
				mAnimationTimeStepCount -= mAnimationTimeStep;
				mNextIndex = true;
			}
		}
	}
	
	void TextureAnimator::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL) {
			return;
		}

		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);

		// Determine the next texture coords index
		if (mAnimationTimeStepSet) {
			if (mNextIndex) {
				// Use the global one for all particles
				_determineNextTextureCoords(visualParticle);
			}
		} else {
			visualParticle->textureAnimationTimeStepCount += timeElapsed;
			if (visualParticle->textureAnimationTimeStepCount > visualParticle->textureAnimationTimeStep) {
				visualParticle->textureAnimationTimeStepCount -= visualParticle->textureAnimationTimeStep;
				_determineNextTextureCoords(visualParticle);
			}
		}
	}
	
	void TextureAnimator::_determineNextTextureCoords(VisualParticle * visualParticle) {
		switch (mTextureAnimationType) {
		case TAT_LOOP: {
			if (visualParticle->textureCoordsCurrent >= mTextureCoordsEnd) {
				visualParticle->textureCoordsCurrent = mTextureCoordsStart;
			} else {
				(visualParticle->textureCoordsCurrent)++;
			}
			break;
		}
		case TAT_UP_DOWN: {
			if (visualParticle->textureAnimationDirectionUp == true) {
				// Going up
				if (visualParticle->textureCoordsCurrent >= mTextureCoordsEnd) {
					(visualParticle->textureCoordsCurrent)--;
					visualParticle->textureAnimationDirectionUp = false;
				} else {
					(visualParticle->textureCoordsCurrent)++;
				}
			} else {
				// Going down
				if (visualParticle->textureCoordsCurrent <= mTextureCoordsStart) {
					(visualParticle->textureCoordsCurrent)++;
					visualParticle->textureAnimationDirectionUp = true;
				} else {
					(visualParticle->textureCoordsCurrent)--;
				}
			}
			break;
		}
		case TAT_RANDOM: {
			// Generate a random texcoord index
			visualParticle->textureCoordsCurrent = uint16(Math::RangeRandom(Real(mTextureCoordsStart), Real(mTextureCoordsEnd) + 0.999f));
			break;
		}
		default: {
			break;
		}
		}
	}

	void TextureAnimator::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);
		TextureAnimator * textureAnimator = static_cast<TextureAnimator *>(affector);
		textureAnimator->mAnimationTimeStep = mAnimationTimeStep;
		textureAnimator->mAnimationTimeStepSet = mAnimationTimeStepSet;
		textureAnimator->mTextureAnimationType = mTextureAnimationType;
		textureAnimator->mTextureCoordsStart = mTextureCoordsStart;
		textureAnimator->mTextureCoordsEnd = mTextureCoordsEnd;
		textureAnimator->mStartRandom = mStartRandom;
	}

} /* namespace ParticleUniverse */
