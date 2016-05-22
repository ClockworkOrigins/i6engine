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

#ifndef __PU_ONEXPIRE_OBSERVER_H__
#define __PU_ONEXPIRE_OBSERVER_H__

#include "ParticleUniverseObserver.h"

namespace ParticleUniverse {

	/** This class is used to observe whether a Particle has been expired.
    */
	class _ParticleUniverseExport OnExpireObserver : public ParticleObserver {
	public:
		OnExpireObserver() : ParticleObserver() {
		}
	    virtual ~OnExpireObserver() {}

		/** 
	    */
		virtual bool _observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ONEXPIRE_OBSERVER_H__ */
