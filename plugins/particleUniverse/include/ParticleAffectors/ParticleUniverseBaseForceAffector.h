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

#ifndef __PU_BASE_ATTRACTOR_H__
#define __PU_BASE_ATTRACTOR_H__

#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** This is a baseclass for other Force Affector classes.
    */
	class _ParticleUniverseExport BaseForceAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		enum ForceApplication {
			FA_AVERAGE,
			FA_ADD
		};

		// Constants
		static const Vector3 DEFAULT_FORCE_VECTOR;
		static const ForceApplication DEFAULT_FORCE_APPL;

		BaseForceAffector();
	    virtual ~BaseForceAffector() {}

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

		/** 
	    */
		const Vector3 & getForceVector() const;
		void setForceVector(const Vector3 & forceVector);

		/** 
	    */
		ForceApplication getForceApplication() const;
		void setForceApplication(ForceApplication forceApplication);

	protected:
		Vector3 mForceVector;
		Vector3 mScaledVector;
		ForceApplication mForceApplication;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BASE_ATTRACTOR_H__ */
