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

#include "ParticleAffectors/ParticleUniverseLinearForceAffector.h"

namespace ParticleUniverse {
	
	void LinearForceAffector::copyAttributesTo(ParticleAffector * affector) {
		BaseForceAffector::copyAttributesTo(affector);
	}
	
	void LinearForceAffector::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Scale force
		mScaledVector = mForceVector * timeElapsed;
	}
	
	void LinearForceAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Affect the direction and take the specialisation into account
		if (mForceApplication == FA_ADD) {
			particle->direction += mScaledVector * _calculateAffectSpecialisationFactor(particle);
		} else {
			particle->direction = (particle->direction + mForceVector) / 2;
		}
	}

} /* namespace ParticleUniverse */
