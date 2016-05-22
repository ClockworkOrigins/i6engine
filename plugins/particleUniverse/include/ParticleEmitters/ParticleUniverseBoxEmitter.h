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

#ifndef __PU_BOX_EMITTER_H__
#define __PU_BOX_EMITTER_H__

#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse {

	/** The BoxEmitter is a ParticleEmitter that emits particles within a box shape.
    */
	class _ParticleUniverseExport BoxEmitter : public ParticleEmitter {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const Real DEFAULT_WIDTH;
		static const Real DEFAULT_HEIGHT;
		static const Real DEFAULT_DEPTH;

		BoxEmitter();
	    virtual ~BoxEmitter() {}

		/** 
	    */
		Real getHeight() const;
		void setHeight(const Real height);

		/** 
	    */
		Real getWidth() const;
		void setWidth(const Real width);

		/** 
	    */
		Real getDepth() const;
		void setDepth(const Real depth);

		/** 
	    */
		virtual void _initParticlePosition(Particle * particle);

		/** 
	    */
		virtual void copyAttributesTo(ParticleEmitter * emitter);

	protected:
		Real mHeight;
		Real mWidth;
		Real mDepth;

		Real mXRange;
		Real mYRange;
		Real mZRange;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BOX_EMITTER_H__ */
