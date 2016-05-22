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

#ifndef __PU_CAMERA_DEPENDENCY_H__
#define __PU_CAMERA_DEPENDENCY_H__

#include "ParticleUniverseIDependency.h"

namespace ParticleUniverse {

	/** The CameraDependency class is used to define a relation between an attribute (for example the emission
		rate of a ParticleEmitter) and the camera. The camera distance influences the value of the attribute.
	@remarks
		In case of the emission rate for example, it can be defined that that number of emitted particles 
		decreases if the camera gets further away.
    */
	class _ParticleUniverseExport CameraDependency : public IDependency {
	public:
		// Constants
		static const Real DEFAULT_DISTANCE_THRESHOLD;
		static const bool DEFAULT_INCREASE;

		CameraDependency();
		CameraDependency(Real threshold, bool inc);
		virtual ~CameraDependency() {}

		/** Todo
	    */
		virtual bool affect(Real & baseValue, Real dependencyValue);

		/** Todo
	    */
		Real getThreshold() const { return mThreshold; }
		void setThreshold(const Real threshold) { mThreshold = threshold; }

		/** Todo
	    */
		bool isIncrease() const { return mIncrease; }
		void setIncrease(bool increase) { mIncrease = increase; }

		/** Copy attributes to another camera dependency.
	    */
		virtual void copyAttributesTo(CameraDependency * cameraDependency);

	protected:
		Real mThreshold;
		bool mIncrease;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_CAMERA_DEPENDENCY_H__ */
