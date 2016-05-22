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

#ifndef __PU_LINEAR_FORCE_AFFECTOR_H__
#define __PU_LINEAR_FORCE_AFFECTOR_H__

#include "ParticleUniverseBaseForceAffector.h"

namespace ParticleUniverse {

	/** Adds a force to particles.
    */
	class _ParticleUniverseExport LinearForceAffector : public BaseForceAffector {
	public:
		using Particle::copyAttributesTo;

		LinearForceAffector() : BaseForceAffector() {
		}
	    virtual ~LinearForceAffector() {}

		/** 
	    */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** 
	    */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_LINEAR_FORCE_AFFECTOR_H__ */
