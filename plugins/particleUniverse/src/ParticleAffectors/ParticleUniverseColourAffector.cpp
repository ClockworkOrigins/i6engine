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

#include "ParticleAffectors/ParticleUniverseColourAffector.h"

#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const ColourAffector::ColourOperation ColourAffector::DEFAULT_COLOUR_OPERATION = ColourAffector::CAO_SET;

	ColourAffector::ColourAffector() : ParticleAffector(), mColourOperation(DEFAULT_COLOUR_OPERATION) {
	}
	
	void ColourAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		ColourAffector * colourAffector = static_cast<ColourAffector *>(affector);
		colourAffector->mColourMap = mColourMap;
		colourAffector->mColourOperation = mColourOperation;
	}
	
	const ColourAffector::ColourOperation & ColourAffector::getColourOperation() const {
		return mColourOperation;
	}
	
	void ColourAffector::setColourOperation(const ColourAffector::ColourOperation & colourOperation) {
		mColourOperation = colourOperation;
	}
	
	void ColourAffector::addColour(Real tf, const ColourValue & colour) {
		mColourMap[tf] = colour;
	}
    
	const ColourAffector::ColourMap& ColourAffector::getTimeAndColour() const {
		return mColourMap;
	}
	
	void ColourAffector::clearColourMap() {
		mColourMap.clear();
	}
	
	void ColourAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Fast rejection
		if (mColourMap.empty()) {
			return;
		}

		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL) {
			return;
		}

		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
		
		// Linear interpolation of the colour
		ColourValue colour = ColourValue::White;
		Real tf = (visualParticle->totalTimeToLive - visualParticle->timeToLive) / visualParticle->totalTimeToLive;
		ColourMapIterator it1 = _findNearestColourMapIterator(tf);
		ColourMapIterator it2 = it1;
		it2++;
		if (it2 != mColourMap.end()) {
			// Interpolate colour
			colour = it1->second + ((it2->second - it1->second) * ((tf - it1->first)/(it2->first - it1->first)));
		} else {
			colour = it1->second;
		}

		// Determine operation
		if (mColourOperation == CAO_SET) {
			// No operation, so just set the colour
			visualParticle->colour = colour;
		} else {
			// Multiply
			visualParticle->colour = colour * visualParticle->originalColour;
		}
	}
    
    ColourAffector::ColourMapIterator ColourAffector::_findNearestColourMapIterator(Real tf) {
		ColourMapIterator it;
		for (it = mColourMap.begin(); it != mColourMap.end(); ++it) {
			if (tf < it->first) {
				if (it == mColourMap.begin()) {
					return it;
				} else {
					return --it;
				}
			}
		}

		// If not found return the last valid iterator
		return --it;
    }

} /* namespace ParticleUniverse */
