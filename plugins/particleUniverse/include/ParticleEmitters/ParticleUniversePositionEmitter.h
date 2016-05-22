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

#ifndef __PU_POSITION_EMITTER_H__
#define __PU_POSITION_EMITTER_H__

#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse {

	/** The PositionEmitter is an emitter that emits particles from one or more given positions.
		The PositionEmitter enables building up a predefined structure out of particles. Imaging building up a
		wall that is made up from stone blocks. The particles are emitted on the positions that are added to this
		emitter. Adding some physical behaviour to the particles and you have your stone wall that collapses if a 
		force is applied to it.
    */
	class _ParticleUniverseExport PositionEmitter : public ParticleEmitter {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const bool DEFAULT_RANDOMIZE;

		PositionEmitter();
	    virtual ~PositionEmitter() {}

		/** 
	    */
		bool isRandomized() const;

		/** 
	    */
		void setRandomized(bool randomized);

		/** Returns a list with positions
	    */
		const vector<Vector3> & getPositions() const;

		/** Add a new position to this emitter
	    */
		void addPosition(const Vector3 & position);

		/** Remove all positions from this emitter
	    */
		void removeAllPositions();

		/** See ParticleEmitter
		*/
		void _notifyStart();

		/** See ParticleEmitter
		*/
		virtual unsigned short _calculateRequestedParticles(Real timeElapsed);

		/** Generate a particle position based on the added positions.
	    */
		virtual void _initParticlePosition(Particle * particle);

		/** 
	    */
		virtual void copyAttributesTo(ParticleEmitter * emitter);

	protected:
		vector<Vector3> mPositionList;
		bool mRandomized;
		size_t mIndex;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_POSITION_EMITTER_H__ */
