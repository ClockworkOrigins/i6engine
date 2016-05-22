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

#include "Externs/ParticleUniverseBoxColliderExtern.h"

#include "OgreNode.h"

namespace ParticleUniverse {
	
	void BoxColliderExtern::_preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed) {
		if (isAttached()) {
			// Use the position of the parent node in this case.
			position = getParentNode()->_getDerivedPosition();
			mDerivedPosition = position;
			populateAlignedBox(mBox, mDerivedPosition, mWidth, mHeight, mDepth);
			_calculateBounds();
		}
	}
	
	void BoxColliderExtern::_interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed) {
		if (isAttached()) {
			if (mDistanceThresholdSet && mDerivedPosition.distance(particle->position) > mDistanceThreshold) {
				return;
			}

			// Use the derived BoxCollider functionality here
			_affect(technique, particle, timeElapsed);
		}
	}
	
	void BoxColliderExtern::copyAttributesTo(Extern * externObject) {
		Attachable::copyAttributesTo(externObject);

		BoxColliderExtern * boxColliderExtern = static_cast<BoxColliderExtern *>(externObject);
		BoxCollider * boxCollider = static_cast<BoxCollider *>(boxColliderExtern);
		BoxCollider::copyAttributesTo(boxCollider);
	}

} /* namespace ParticleUniverse */
