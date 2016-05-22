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

#include "Externs/ParticleUniverseGravityExtern.h"

#include "OgreNode.h"

namespace ParticleUniverse {
	
	void GravityExtern::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		// Use the position of the parent node in this case.
		if (isAttached()) {
			position = getParentNode()->_getDerivedPosition();
			mDerivedPosition = position;
		}
	}
	
	void GravityExtern::_interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed) {
		if (isAttached()) {
			if (mDistanceThresholdSet && mDerivedPosition.distance(particle->position) > mDistanceThreshold) {
				return;
			}

			// Use the derived GravityAffector functionality here
			_affect(technique, particle, timeElapsed);
		}
	}
	
	void GravityExtern::copyAttributesTo(Extern * externObject) {
		Attachable::copyAttributesTo(externObject);

		GravityExtern * gravityExtern = static_cast<GravityExtern *>(externObject);
		GravityAffector * gravityAffector = static_cast<GravityAffector *>(gravityExtern);
		GravityAffector::copyAttributesTo(gravityAffector);
	}

} /* namespace ParticleUniverse */
