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

#ifndef __PU_ALIGN_AFFECTOR_H__
#define __PU_ALIGN_AFFECTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** Aligns the orientation of a particle towards the previous particle and adjusts the height of
		the particle, which becomes the length between the two particles. And how do we benefit from 
		this? Well, with the right renderer settings you could get a chain of particles, each connected
		to the previous, making use of the particle orientation.
		
		We get good results with a billboard renderer (which - btw - doesn't take the particle 
		orientation into account by default). Use the billboard type 'oriented shape', which is a type
		that isn't a standard billboard type of Ogre. It has been added to allow the billboard renderer
		take advantage of the particles' orientation. Also use for instance 'bottom center' as the 
		billboard origin, which neatly connects the billboards.

		Using the AlignAffector is a step in the direction to generate electric beams.
    */
	class _ParticleUniverseExport AlignAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const bool DEFAULT_RESIZE;

		AlignAffector();
	    virtual ~AlignAffector() {}

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** See setResize().
	    */
		bool isResize() const;

		/** Set resize. This attribute determines whether the size of the particle must be changed
			according to its alignment with the previous particle.
	    */
		void setResize(bool resize);

		/** @copydoc ParticleAffector::_firstParticle */
		virtual void _firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

	protected:
		Particle * mPreviousParticle;
		bool mResize;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ALIGN_AFFECTOR_H__ */
