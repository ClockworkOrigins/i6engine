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

#ifndef __PU_TEXTURE_ANIMATION_H__
#define __PU_TEXTURE_ANIMATION_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse {

	/** The TextureAnimator makes it possible to have an animated texture for each individual particle. It relies on the uv coordinate 
	    settings in the ParticleRenderer.
    */
	class _ParticleUniverseExport TextureAnimator : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		enum TextureAnimationType {
			TAT_LOOP,
			TAT_UP_DOWN,
			TAT_RANDOM
		};

		// Constants
		static const Real DEFAULT_TIME_STEP;
		static const uint16 DEFAULT_TEXCOORDS_START;
		static const uint16 DEFAULT_TEXCOORDS_END;
		static const TextureAnimationType DEFAULT_ANIMATION_TYPE;
		static const bool DEFAULT_START_RANDOM;

		TextureAnimator();
	    virtual ~TextureAnimator();

		/** Returns the AnimationTimeStep. The AnimationTimeStep defines the time between each animation frame. */
		Real getAnimationTimeStep() const;

		/** Set the  AnimationTimeStep */
		void setAnimationTimeStep(Real animationTimeStep);

		/** Returns the type of texture animation. */
		TextureAnimationType getTextureAnimationType() const;

		/** Set the type of texture animation. */
		void setTextureAnimationType(TextureAnimationType textureAnimationType);

		/** Todo */
		uint16 getTextureCoordsStart() const;

		/** Todo */
		void setTextureCoordsStart(uint16 textureCoordsStart);

		/** Todo */
		uint16 getTextureCoordsEnd() const;

		/** Todo */
		void setTextureCoordsEnd(uint16 textureCoordsEnd);

		/** Todo */
		bool isStartRandom() const;

		/** Todo */
		void setStartRandom(bool startRandom);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** @copydoc ParticleAffector::_preProcessParticles */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** @copydoc ParticleAffector::_initParticleForEmission */
		virtual void _initParticleForEmission(Particle * particle);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

	protected:
		Real mAnimationTimeStep;
		Real mAnimationTimeStepCount;
		bool mStartRandom;
		bool mAnimationTimeStepSet;
		bool mNextIndex;
		TextureAnimationType mTextureAnimationType;
		uint16 mTextureCoordsStart;
		uint16 mTextureCoordsEnd;

		/** Todo.
		*/
		void _determineNextTextureCoords(VisualParticle * visualParticle);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_TEXTURE_ANIMATION_H__ */
