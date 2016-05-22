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

#ifndef __PU_COLOUR_AFFECTOR_H__
#define __PU_COLOUR_AFFECTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** This affector is typically used to change the colour of a particle during its lifetime.
    */
	class _ParticleUniverseExport ColourAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		typedef map<Real, ColourValue> ColourMap;
		typedef map<Real, ColourValue>::iterator ColourMapIterator;
		enum ColourOperation {
			CAO_MULTIPLY,
			CAO_SET
		};

		// Constants
		static const ColourOperation DEFAULT_COLOUR_OPERATION;

		// Constructor
		ColourAffector();

		// Destructor
	    virtual ~ColourAffector() {}

		/** 
	    */
		void addColour(Real timeFraction, const ColourValue & colour);

		/** 
	    */
		const ColourMap & getTimeAndColour() const;

		/** 
	    */
		void clearColourMap();
				
		/** 
	    */
		inline ColourMapIterator _findNearestColourMapIterator(Real timeFraction);

		/** 
	    */
		const ColourOperation & getColourOperation() const;

		/** 
	    */
		void setColourOperation (const ColourOperation & colourOperation);

		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		ColourMap mColourMap;
		ColourOperation mColourOperation;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_COLOUR_AFFECTOR_H__ */
