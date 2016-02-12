/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
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
