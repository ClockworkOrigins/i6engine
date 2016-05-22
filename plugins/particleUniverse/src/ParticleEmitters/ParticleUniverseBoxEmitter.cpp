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

#include "ParticleEmitters/ParticleUniverseBoxEmitter.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real BoxEmitter::DEFAULT_WIDTH = 100.0;
	const Real BoxEmitter::DEFAULT_HEIGHT = 100.0;
	const Real BoxEmitter::DEFAULT_DEPTH = 100.0;
	
	BoxEmitter::BoxEmitter() : ParticleEmitter(), mHeight(DEFAULT_HEIGHT), mWidth(DEFAULT_WIDTH), mDepth(DEFAULT_DEPTH), mXRange(0.5 * DEFAULT_WIDTH), mYRange(0.5 * DEFAULT_HEIGHT), mZRange(0.5 * DEFAULT_DEPTH) {
	}
	
	Real BoxEmitter::getHeight() const {
		return mHeight;
	}
	
	void BoxEmitter::setHeight(const Real height) {
		mHeight = height;
		mYRange = 0.5 * height;
	}
	
	Real BoxEmitter::getWidth() const {
		return mWidth;
	}
	
	void BoxEmitter::setWidth(const Real width) {
		mWidth = width;
		mXRange = 0.5 * width;
	}
	
	Real BoxEmitter::getDepth() const {
		return mDepth;
	}
	
	void BoxEmitter::setDepth(const Real depth) {
		mDepth = depth;
		mZRange = 0.5 * depth;
	}
	
	void BoxEmitter::_initParticlePosition(Particle * particle) {
		ParticleSystem * sys = mParentTechnique->getParentSystem();
		if (sys) {
			particle->position = getDerivedPosition() + sys->getDerivedOrientation() * (_mEmitterScale * Vector3(Math::SymmetricRandom() * mXRange, Math::SymmetricRandom() * mYRange, Math::SymmetricRandom() * mZRange));
		} else {
			particle->position = getDerivedPosition() + _mEmitterScale * Vector3(Math::SymmetricRandom() * mXRange, Math::SymmetricRandom() * mYRange, Math::SymmetricRandom() * mZRange);
		}

		particle->originalPosition = particle->position;
	}
	
	void BoxEmitter::copyAttributesTo(ParticleEmitter * emitter) {
		ParticleEmitter::copyAttributesTo(emitter);

		BoxEmitter * boxEmitter = static_cast<BoxEmitter *>(emitter);
		boxEmitter->mHeight = mHeight;
		boxEmitter->mWidth = mWidth;
		boxEmitter->mDepth = mDepth;
		boxEmitter->mXRange = mXRange;
		boxEmitter->mYRange = mYRange;
		boxEmitter->mZRange = mZRange;
	}

} /* namespace ParticleUniverse */
