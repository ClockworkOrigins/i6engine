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

#include "ParticleUniverseVisualParticle.h"

#include <float.h>

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseIVisualData.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	VisualParticle::VisualParticle() : Particle(), colour(ColourValue::White), originalColour(ColourValue::White), zRotation(Radian(0)), zRotationSpeed(Radian(0)), orientation(Quaternion::IDENTITY), originalOrientation(Quaternion::IDENTITY), rotationSpeed(0.0), rotationAxis(Vector3::UNIT_Z), ownDimensions(false), width(1.0), height(1.0), depth(1.0), radius(0.87), textureAnimationTimeStep(0.1), textureAnimationTimeStepCount(0.0), textureCoordsCurrent(0), textureAnimationDirectionUp(true) {
		particleType = PT_VISUAL;
		mMarkedForEmission  = true; // Default is false, but visual particles are always emitted.
	}
	
	void VisualParticle::setOwnDimensions(Real newWidth, Real newHeight, Real newDepth) {
		ownDimensions = true;
		if (std::abs(newWidth) > DBL_EPSILON) {
			width = newWidth;
		}
		if (std::abs(newHeight) > DBL_EPSILON) {
			height = newHeight;
		}
		if (std::abs(newDepth) > DBL_EPSILON) {
			depth = newDepth;
		}
		_calculateBoundingSphereRadius();
		parentEmitter->getParentTechnique()->_notifyParticleResized();
	}
	
	void VisualParticle::_initForEmission() {
		Particle::_initForEmission();
		textureAnimationTimeStep = 0.1;
		textureAnimationTimeStepCount = 0.0;
		textureCoordsCurrent = 0;
		textureAnimationDirectionUp = true;
	}
	
	void VisualParticle::_initForExpiration(ParticleTechnique * technique, Real timeElapsed) {
		Particle::_initForExpiration(technique, timeElapsed);
		if (visualData) {
			visualData->setVisible(false);
		}
	}
	
	void VisualParticle::_calculateBoundingSphereRadius() {
		//radius = 0.5 * Math::Sqrt(width*width + height*height + depth*depth);
		radius = 0.5 * std::max(depth, std::max(width, height)); // approximation
	}

} /* namespace ParticleUniverse */
