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

#ifndef __PU_PATH_FOLLOWER_H__
#define __PU_PATH_FOLLOWER_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseSimpleSpline.h"

namespace ParticleUniverse {

	/** This affector let particles move along a line. The line is derived from a number of points in 3D space.
		Using a spline interpolation, the line becomes smooth.
    */
	class _ParticleUniverseExport PathFollower : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		PathFollower() : ParticleAffector() {
		}
	    virtual ~PathFollower() {}

		/** 
	    */
		void addPoint(const Vector3 & point);

		/** Clear all points
	    */
		void clearPoints();

		/** 
	    */
		unsigned short getNumPoints() const { return mSpline.getNumPoints(); }
				
		/** 
	    */
		const Vector3 & getPoint(unsigned short index) const { return mSpline.getPoint(index); }
			
		/** 
	    */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		SimpleSpline mSpline;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PATH_FOLLOWER_H__ */
