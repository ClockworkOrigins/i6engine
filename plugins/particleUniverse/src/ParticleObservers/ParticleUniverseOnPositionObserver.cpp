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

#include "ParticleObservers/ParticleUniverseOnPositionObserver.h"

namespace ParticleUniverse {

	// Constants
	const Vector3 OnPositionObserver::DEFAULT_POSITION_THRESHOLD(0, 0, 0);
	
	OnPositionObserver::OnPositionObserver() : ParticleObserver(), mPositionXThreshold(DEFAULT_POSITION_THRESHOLD.x), mPositionYThreshold(DEFAULT_POSITION_THRESHOLD.y), mPositionZThreshold(DEFAULT_POSITION_THRESHOLD.z), mPositionXThresholdSet(false), mPositionYThresholdSet(false), mPositionZThresholdSet(false), mComparePositionX(CO_LESS_THAN), mComparePositionY(CO_LESS_THAN), mComparePositionZ(CO_LESS_THAN) {
	}
	
	bool OnPositionObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (!particle) {
			return false;
		}

		// Added scale in V1.3.1
		if (mPositionXThresholdSet && mComparePositionX == CO_LESS_THAN && particle->position.x < mPositionXThreshold * _mObserverScale.x) {
			return true;
		}
		if (mPositionXThresholdSet && mComparePositionX == CO_GREATER_THAN && particle->position.x > mPositionXThreshold * _mObserverScale.x) {
			return true;
		}
		if (mPositionXThresholdSet && mComparePositionX == CO_EQUALS && almostEquals(particle->position.x, mPositionXThreshold * _mObserverScale.x)) {
			return true;
		}

		if (mPositionYThresholdSet && mComparePositionY == CO_LESS_THAN && particle->position.y < mPositionYThreshold * _mObserverScale.y) {
			return true;
		}
		if (mPositionYThresholdSet && mComparePositionY == CO_GREATER_THAN && particle->position.y > mPositionYThreshold * _mObserverScale.y) {
			return true;
		}
		if (mPositionYThresholdSet && mComparePositionY == CO_EQUALS && almostEquals(particle->position.y, mPositionYThreshold * _mObserverScale.y)) {
			return true;
		}

		if (mPositionZThresholdSet && mComparePositionZ == CO_LESS_THAN && particle->position.z < mPositionZThreshold * _mObserverScale.z) {
			return true;
		}
		if (mPositionZThresholdSet && mComparePositionZ == CO_GREATER_THAN && particle->position.z > mPositionZThreshold * _mObserverScale.z) {
			return true;
		}
		if (mPositionZThresholdSet && mComparePositionZ == CO_EQUALS && !almostEquals(particle->position.z, mPositionZThreshold * _mObserverScale.z)) {
			return true;
		}

		return false;
	}
	
	void OnPositionObserver::copyAttributesTo(ParticleObserver * observer) {
		ParticleObserver::copyAttributesTo(observer);

		OnPositionObserver * onPositionObserver = static_cast<OnPositionObserver *>(observer);
		onPositionObserver->mPositionXThreshold = mPositionXThreshold;
		onPositionObserver->mPositionYThreshold = mPositionYThreshold;
		onPositionObserver->mPositionZThreshold = mPositionZThreshold;
		onPositionObserver->mComparePositionX = mComparePositionX;
		onPositionObserver->mComparePositionY = mComparePositionY;
		onPositionObserver->mComparePositionZ = mComparePositionZ;
		onPositionObserver->mPositionXThresholdSet = mPositionXThresholdSet;
		onPositionObserver->mPositionYThresholdSet = mPositionYThresholdSet;
		onPositionObserver->mPositionZThresholdSet = mPositionZThresholdSet;
	}

} /* namespace ParticleUniverse */
