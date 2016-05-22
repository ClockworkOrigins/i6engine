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

#include "Externs/ParticleUniverseVortexExtern.h"

#include "OgreNode.h"

namespace ParticleUniverse {
	
	void VortexExtern::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		if (isAttached()) {
			position = getParentNode()->_getDerivedPosition();
			mDerivedPosition = position;
		}
	}
	
	void VortexExtern::_interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed) {
		// Setting the distance threshold is mandatory
		if (isAttached() && mDistanceThresholdSet) {
			Real distance = mDerivedPosition.distance(particle->position);
			if (distance > mDistanceThreshold) {
				return;
			} else {
				// Rotate position, direction and orientation (visible particle only) based on distance between particle
				// and VortexExtern
				Real scalar = 1 - (distance / mDistanceThreshold);
				if (mParentNode) {
					mRotation.FromAngleAxis((_calculateRotationSpeed() * timeElapsed * scalar), mParentNode->_getDerivedOrientation() * mRotationVector);
				} else {
					mRotation.FromAngleAxis((_calculateRotationSpeed() * timeElapsed * scalar), mRotationVector);
				}
				_affect(technique, particle, timeElapsed);
			}
		}
	}
	
	void VortexExtern::copyAttributesTo(Extern * externObject) {
		Attachable::copyAttributesTo(externObject);
		VortexExtern * vortexExtern = static_cast<VortexExtern *>(externObject);
		VortexAffector * vortexAffector = static_cast<VortexAffector *>(vortexExtern);
		VortexAffector::copyAttributesTo(vortexAffector);
	}

} /* namespace ParticleUniverse */
