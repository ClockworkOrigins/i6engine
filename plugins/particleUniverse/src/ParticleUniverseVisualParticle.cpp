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
