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

#ifndef __PU_TECHNIQUE_LISTENER_H__
#define __PU_TECHNIQUE_LISTENER_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	/*  TechniqueListener establishes a channel between a ParticleTechnique and some other object, which is a 
	    type of TechniqueListener.
	*/
	class _ParticleUniverseExport TechniqueListener {
	public:
		TechniqueListener() {}
		virtual ~TechniqueListener() {}

		/*  Callback function, which is called as soon as a particle is emitted.
		*/
		virtual void particleEmitted(ParticleTechnique * particleTechnique, Particle * particle) = 0;

		/*  Callback function, which is called as soon as a particle is expired.
		*/
		virtual void particleExpired(ParticleTechnique * particleTechnique, Particle * particle) = 0;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_TECHNIQUE_LISTENER_H__ */
