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

#include "ParticleUniverseExtern.h"

namespace ParticleUniverse {
	
	void Extern::_processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
		// Call the _firstParticle() function if the first particle in the update loop is encountered.
		if (firstParticle) {
			// Perform a precalculation at the first particle
			_firstParticle(particleTechnique, particle, timeElapsed);
		}

		_interface(particleTechnique, particle, timeElapsed);
	}
	
	void Extern::_notifyRescaled(const Vector3 & scale) {
		_mExternScale = scale;
	}
	
	void Extern::copyAttributesTo(Extern * externObject) {
		copyParentAttributesTo(externObject);
	}
	
	void Extern::copyParentAttributesTo(Extern * externObject) {
		externObject->setName(mName);
		externObject->mParentTechnique = mParentTechnique;
		externObject->_mExternScale = _mExternScale;
	}

} /* namespace ParticleUniverse */
