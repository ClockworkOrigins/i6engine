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

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.h"

#include "ParticleUniverseParticle.h"

namespace ParticleUniverse {

	// Constants
	const DoScaleEventHandler::ScaleType DoScaleEventHandler::DEFAULT_SCALE_TYPE = DoScaleEventHandler::ST_TIME_TO_LIVE;
	const Real DoScaleEventHandler::DEFAULT_SCALE_FRACTION = 0.2;
	
	DoScaleEventHandler::DoScaleEventHandler() : ParticleEventHandler(), mScaleFraction(DEFAULT_SCALE_FRACTION), mScaleType(DEFAULT_SCALE_TYPE) {
	}
	
	const DoScaleEventHandler::ScaleType & DoScaleEventHandler::getScaleType() const {
		return mScaleType;
	}
	
	void DoScaleEventHandler::setScaleType(const DoScaleEventHandler::ScaleType & scaleType) {
		mScaleType = scaleType;
	}
	
	Real DoScaleEventHandler::getScaleFraction() const {
		return mScaleFraction;
	}
	
	void DoScaleEventHandler::setScaleFraction(const Real scaleFraction) {
		mScaleFraction = scaleFraction;
	}
	
	void DoScaleEventHandler::_handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (!particle) {
			return;
		}

		switch (mScaleType) {
		case ST_TIME_TO_LIVE: {
			particle->timeToLive -= timeElapsed * mScaleFraction * particle->timeToLive;
			break;
		}
		case ST_VELOCITY: {
			particle->direction += timeElapsed * mScaleFraction * particle->direction;
			break;
		}
		default: {
			break;
		}
		}
	}
	
	void DoScaleEventHandler::copyAttributesTo(ParticleEventHandler * eventHandler) {
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoScaleEventHandler * doScaleEventHandler = static_cast<DoScaleEventHandler *>(eventHandler);
		doScaleEventHandler->setScaleFraction(mScaleFraction);
		doScaleEventHandler->setScaleType(mScaleType);
	}

} /* namespace ParticleUniverse */
