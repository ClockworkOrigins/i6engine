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

#include "ParticleAffectors/ParticleUniverseVelocityMatchingAffector.h"

#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	// Constants
	const Real VelocityMatchingAffector::DEFAULT_RADIUS = 100.0;

	VelocityMatchingAffector::VelocityMatchingAffector() : ParticleAffector(), mRadius(DEFAULT_RADIUS) {
	}
	
	Real VelocityMatchingAffector::getRadius() const {
		return mRadius;
	}
	
	void VelocityMatchingAffector::setRadius(Real radius) {
		mRadius = radius;
	}
	
	void VelocityMatchingAffector::_prepare(ParticleTechnique * particleTechnique) {
		// Activate spatial hashing
		particleTechnique->setSpatialHashingUsed(true);
	}
	
	void VelocityMatchingAffector::_unprepare(ParticleTechnique * particleTechnique) {
		// Activate spatial hashing
		particleTechnique->setSpatialHashingUsed(false);
	}
	
	void VelocityMatchingAffector::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// Determine neighbouring particles.
		SpatialHashTable<Particle *> * hashtable = particleTechnique->getSpatialHashTable();
		if (hashtable) {
			SpatialHashTable<Particle *>::HashTableCell cell = hashtable->getCell(particle->position);
			if (cell.empty()) {
				return;
			}

			unsigned int size = static_cast<unsigned int>(cell.size());
			Vector3 sumDirection = Vector3::ZERO;
			Vector3 diff = Vector3::ZERO;
			unsigned int count = 0;
			for (unsigned int i = 0; i < size; ++i) {
				Particle * p = cell[i];

				// Don't check if it is the same particle
				if (particle != p) {
					// Validate whether the neighbouring particle is within range
					diff = p->position - particle->position;
					if (diff.length() < mRadius) {
						sumDirection += p->direction;
						count++;
					}
				}
			}

			// Adjust direction
			if (count > 0) {
				sumDirection /= Real(count);
				particle->direction += (sumDirection - particle->direction) * timeElapsed;
			}
		}
	}
	
	void VelocityMatchingAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);
		VelocityMatchingAffector * velocityMatchingAffector = static_cast<VelocityMatchingAffector *>(affector);
		velocityMatchingAffector->mRadius = mRadius;
	}

} /* namespace ParticleUniverse */
