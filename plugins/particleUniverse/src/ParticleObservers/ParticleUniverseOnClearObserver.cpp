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

#include "ParticleObservers/ParticleUniverseOnClearObserver.h"

#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {
	
	void OnClearObserver::_notifyStart() {
		ParticleObserver::_notifyStart();
		mContinue = false;
	}
	
	bool OnClearObserver::_observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		// This function always returns false, but is never called (but it must be implemented).
		return false;
	}
	
	void OnClearObserver::_processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
		// Override the _processParticle() function, because we don't observe individual particles.
		if (firstParticle) {
			// Only continue to validate if there was at least 1 particle emitted, otherwise the event-
			// handlers are already called at the very start (when there are no particle yet).
			mContinue = true;
		}
	}
	
	void OnClearObserver::_postProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		if (mContinue) {
			if (!(technique->getNumberOfEmittedParticles() > 0)) {
				/** Handle the event. Use 0 as the particle pointer. This means that not all eventhandlers
					are suitable. If they expect a particle (and most eventhandlers do), it could
					result in an exception. Handlers such as the DoEnableComponentEventHandler and the
					DoStopSystemEventHandler however can be used without any problem.
				*/
				_handleEvent(technique, 0, timeElapsed);
				mContinue = false;
			}
		}
	}

} /* namespace ParticleUniverse */
