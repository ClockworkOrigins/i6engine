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

#include "ParticleAffectors/ParticleUniverseAlignAffector.h"

#include "ParticleUniverseVisualParticle.h"

namespace ParticleUniverse {

	// Constants
	const bool AlignAffector::DEFAULT_RESIZE = false;
	
	AlignAffector::AlignAffector() : ParticleAffector(), mPreviousParticle(nullptr), mResize(DEFAULT_RESIZE) {
	}
	
	void AlignAffector::_firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		mPreviousParticle = particle;
	}
	
	bool AlignAffector::isResize() const {
		return mResize;
	}
	
	void AlignAffector::setResize(bool resize) {
		mResize = resize;
	}
	
	void AlignAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (particle->particleType == Particle::PT_VISUAL) {
			// Set the orientation towards the previous particle, but rotation is undetermined.
			VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);

			// Get difference
			Vector3 diff = (mPreviousParticle->position - particle->position);
			if (mResize) {
				visualParticle->setOwnDimensions (visualParticle->width, diff.length(), visualParticle->depth);
			}
			diff.normalise();
			visualParticle->orientation.x = diff.x;
			visualParticle->orientation.y = diff.y;
			visualParticle->orientation.z = diff.z;
		}

		mPreviousParticle = particle;
	}
	
	void AlignAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		AlignAffector * alignAffector = static_cast<AlignAffector *>(affector);
		alignAffector->mResize = mResize;
	}

} /* namespace ParticleUniverse */
