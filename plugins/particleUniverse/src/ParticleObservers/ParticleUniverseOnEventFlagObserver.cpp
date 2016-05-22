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

#include "ParticleObservers/ParticleUniverseOnEventFlagObserver.h"

namespace ParticleUniverse {

	// Constants
	const uint32 OnEventFlagObserver::DEFAULT_EVENT_FLAG = 0;

	OnEventFlagObserver::OnEventFlagObserver() : ParticleObserver(), mEventFlag(DEFAULT_EVENT_FLAG) {
	}
	
	bool OnEventFlagObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (!particle) {
			return false;
		}

		return (particle->hasEventFlags(mEventFlag));
	}
	
	void OnEventFlagObserver::copyAttributesTo(ParticleObserver * observer) {
		ParticleObserver::copyAttributesTo(observer);

		OnEventFlagObserver * onEventFlagObserver = static_cast<OnEventFlagObserver *>(observer);
		onEventFlagObserver->mEventFlag = mEventFlag;
	}

} /* namespace ParticleUniverse */
