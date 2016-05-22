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

#include "ParticleObservers/ParticleUniverseOnCountObserver.h"

namespace ParticleUniverse {

	// Constants
	const uint OnCountObserver::DEFAULT_THRESHOLD = 0;
	const ComparisionOperator OnCountObserver::DEFAULT_COMPARE = CO_LESS_THAN;

	OnCountObserver::OnCountObserver() : ParticleObserver(), mCount(1), mThreshold(DEFAULT_THRESHOLD), mCompare(DEFAULT_COMPARE) {
	}
	
	void OnCountObserver::_notifyStart() {
		ParticleObserver::_notifyStart();

		// At a restart the counter must be reset again.
		mCount = 1;
	}
	
	bool OnCountObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (!particle) {
			return false;
		}

		if (!particle->hasEventFlags(Particle::PEF_EMITTED)) {
			return false;
		}
		
		if (mCompare == CO_GREATER_THAN) {
			if (mCount > mThreshold) {
				return true;
			} else {
				mCount++;
				return false;
			}
		} else if (mCompare == CO_LESS_THAN) {
			if (mCount < mThreshold) {
				mCount++;
				return true;
			} else {
				return false;
			}
		} else {
			if (mCount == mThreshold) {
				mCount++;
				return true;
			} else {
				mCount++;
				return false;
			}
		}
	}
	
	void OnCountObserver::copyAttributesTo(ParticleObserver * observer) {
		ParticleObserver::copyAttributesTo(observer);

		OnCountObserver * onCountObserver = static_cast<OnCountObserver *>(observer);
		onCountObserver->mCount = mCount;
		onCountObserver->mThreshold = mThreshold;
		onCountObserver->mCompare = mCompare;
	}

} /* namespace ParticleUniverse */
