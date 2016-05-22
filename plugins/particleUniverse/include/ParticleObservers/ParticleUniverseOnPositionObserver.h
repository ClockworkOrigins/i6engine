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

#ifndef __PU_ONPOSITION_OBSERVER_H__
#define __PU_ONPOSITION_OBSERVER_H__

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseObserver.h"

namespace ParticleUniverse {

	/** This class is used to observe whether a Particle reaches a certain position.
    */
	class _ParticleUniverseExport OnPositionObserver : public ParticleObserver {
	public:
		// Constants
		static const Vector3 DEFAULT_POSITION_THRESHOLD;

		OnPositionObserver();
	    virtual ~OnPositionObserver() {}

		/** 
	    */
		virtual bool _observe(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** 
			*/
		void setPositionXThreshold(Real threshold) {
			mPositionXThreshold = threshold;
			mPositionXThresholdSet = true;
		}
		void setPositionYThreshold(Real threshold) {
			mPositionYThreshold = threshold;
			mPositionYThresholdSet = true;
		}
		void setPositionZThreshold(Real threshold) {
			mPositionZThreshold = threshold;
			mPositionZThresholdSet = true;
		}

		/** 
			*/
		Real getPositionXThreshold() const { return mPositionXThreshold; }
		Real getPositionYThreshold() const { return mPositionYThreshold; }
		Real getPositionZThreshold() const { return mPositionZThreshold; }

		/** 
			*/
		bool isPositionXThresholdSet() const { return mPositionXThresholdSet; }
		bool isPositionYThresholdSet() const { return mPositionYThresholdSet; }
		bool isPositionZThresholdSet() const { return mPositionZThresholdSet; }

		/** 
			*/
		void resetPositionXThreshold() { mPositionXThresholdSet = false; }
		void resetPositionYThreshold() { mPositionYThresholdSet = false; }
		void resetPositionZThreshold() { mPositionZThresholdSet = false; }

		/** 
			*/
		void setComparePositionX(ComparisionOperator op) { mComparePositionX = op; }
		void setComparePositionY(ComparisionOperator op) { mComparePositionY = op; }
		void setComparePositionZ(ComparisionOperator op) { mComparePositionZ = op; }

		/** 
			*/
		ComparisionOperator getComparePositionX() const { return mComparePositionX; }
		ComparisionOperator getComparePositionY() const { return mComparePositionY; }
		ComparisionOperator getComparePositionZ() const { return mComparePositionZ; }

		/** Copy attributes to another observer.
	    */
		virtual void copyAttributesTo(ParticleObserver * observer);

	protected:
		Real mPositionXThreshold;
		Real mPositionYThreshold;
		Real mPositionZThreshold;
		bool mPositionXThresholdSet;
		bool mPositionYThresholdSet;
		bool mPositionZThresholdSet;
		ComparisionOperator mComparePositionX;
		ComparisionOperator mComparePositionY;
		ComparisionOperator mComparePositionZ;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ONPOSITION_OBSERVER_H__ */
