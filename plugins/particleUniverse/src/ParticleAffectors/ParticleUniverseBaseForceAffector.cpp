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

#include "ParticleAffectors/ParticleUniverseBaseForceAffector.h"

namespace ParticleUniverse {

	// Constants
	const Vector3 BaseForceAffector::DEFAULT_FORCE_VECTOR(0, 0, 0);
	const BaseForceAffector::ForceApplication BaseForceAffector::DEFAULT_FORCE_APPL = BaseForceAffector::FA_ADD;

	BaseForceAffector::BaseForceAffector() : ParticleAffector(), mForceVector(DEFAULT_FORCE_VECTOR), mScaledVector(Vector3::ZERO), mForceApplication(DEFAULT_FORCE_APPL) {
	}
	
	void BaseForceAffector::copyAttributesTo(ParticleAffector * affector) {
		ParticleAffector::copyAttributesTo(affector);

		BaseForceAffector * baseForceAffector = static_cast<BaseForceAffector *>(affector);
		baseForceAffector->mForceVector = mForceVector;
		baseForceAffector->mForceApplication = mForceApplication;
	}
	
	const Vector3 & BaseForceAffector::getForceVector() const {
		return mForceVector;
	}
	
	void BaseForceAffector::setForceVector(const Vector3 & forceVector) {
		mForceVector = forceVector;
	}
	
	BaseForceAffector::ForceApplication BaseForceAffector::getForceApplication() const {
		return mForceApplication;
	}
	
	void BaseForceAffector::setForceApplication(BaseForceAffector::ForceApplication forceApplication) {
		mForceApplication = forceApplication;
	}

} /* namespace ParticleUniverse */
