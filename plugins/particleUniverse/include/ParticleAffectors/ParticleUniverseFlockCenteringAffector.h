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

#ifndef __PU_FLOCK_CENTER_AFFECTOR_H__
#define __PU_FLOCK_CENTER_AFFECTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** A FlockCenteringAffector determines the center (position) of all particles and affects each particle to go towards that center.
    */
	class _ParticleUniverseExport FlockCenteringAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		FlockCenteringAffector() : ParticleAffector(), mSum(Vector3::ZERO), mAverage(Vector3::ZERO), mCount(0.0) {
		}
	    virtual ~FlockCenteringAffector() {}

		/** @copydoc ParticleAffector::_preProcessParticles */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

	protected:
		Vector3 mSum;
		Vector3 mAverage;
		Real mCount;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_FLOCK_CENTER_AFFECTOR_H__ */
