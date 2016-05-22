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

#ifndef __PU_ONEVENTFLAG_OBSERVER_H__
#define __PU_ONEVENTFLAG_OBSERVER_H__

#include "ParticleUniverseObserver.h"

namespace ParticleUniverse {

	/** The OnEventFlagObserver looks at each particle is one or more eventflags are set.
    */
	class _ParticleUniverseExport OnEventFlagObserver : public ParticleObserver {
	public:
		// Constants
		static const uint32 DEFAULT_EVENT_FLAG;

		OnEventFlagObserver();
	    virtual ~OnEventFlagObserver() {}

		/** 
	    */
		virtual bool _observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
	    */
		uint32 getEventFlag() const { return mEventFlag; }
		void setEventFlag(uint32 eventFlag) { mEventFlag = eventFlag; }

		/** Copy attributes to another observer.
	    */
		virtual void copyAttributesTo(ParticleObserver * observer);

	protected:
		uint32 mEventFlag;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ONEVENTFLAG_OBSERVER_H__ */
