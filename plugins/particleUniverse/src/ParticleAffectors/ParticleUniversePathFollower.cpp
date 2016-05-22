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

#include "ParticleAffectors/ParticleUniversePathFollower.h"

namespace ParticleUniverse {
	
	void PathFollower::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);
		PathFollower * pathFollower = static_cast<PathFollower *>(affector);
		unsigned short numPoints = mSpline.getNumPoints();
		pathFollower->mSpline.clear();
		for (unsigned short i = 0; i < numPoints; ++i) {
			pathFollower->mSpline.addPoint(mSpline.getPoint(i));
		}
	}
	
	void PathFollower::addPoint(const Vector3 & point) {
		mSpline.addPoint(point);
	}
	
	void PathFollower::clearPoints(void) {
		mSpline.clear();
	}
	
	void PathFollower::_affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (mSpline.getNumPoints() > 0) {
			// PU 1.4
			Real timeLeft = particle->totalTimeToLive - particle->timeToLive;
			Real tf = timeLeft / particle->totalTimeToLive;
			Ogre::Real timeFractionPlusDelta = (timeLeft + timeElapsed ) / particle->totalTimeToLive;
			timeFractionPlusDelta = timeFractionPlusDelta < 1.0 ? timeFractionPlusDelta : 1.0;
			particle->position += _mAffectorScale * (mSpline.interpolate(timeFractionPlusDelta) - mSpline.interpolate(tf));
		}
	}

} /* namespace ParticleUniverse */
