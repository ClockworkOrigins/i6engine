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

#include "ParticleRenderers/ParticleUniverseSphere.h"

#include "ParticleRenderers/ParticleUniverseSphereSet.h"

namespace ParticleUniverse {
	
	Sphere::Sphere() : mPosition(Vector3::ZERO), mColour(ColourValue::White), mOrientation(Quaternion::IDENTITY), mParentSet(nullptr), mOwnRadius(false), mRadius(100.0), mRadiusChanged(false) {
	}

	// TODO: (Daniel) check this, position not set?
	Sphere::Sphere(const Vector3 & position, SphereSet * owner) : mPosition(Vector3::ZERO), mColour(ColourValue::White), mOrientation(Quaternion::IDENTITY), mParentSet(nullptr), mOwnRadius(false), mRadius(10.0), mRadiusChanged(false) {
	}
	
	Sphere::~Sphere() {
	}
	
	void Sphere::setPosition(const Vector3 & position) {
		mPosition = position;
	}
	
	void Sphere::setPosition(Real x, Real y, Real z) {
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
	}
	
	const Vector3 & Sphere::getPosition() const {
		return mPosition;
	}
	
	void Sphere::setRadius(Real radius) {
		mOwnRadius = true;
		mRadius = radius;
		if (mParentSet) {
			mParentSet->_notifyResized();
		}
		mRadiusChanged = true;
    }
	
	bool Sphere::hasOwnRadius() const {
		return mOwnRadius;
	}
	
	void Sphere::_notifyOwner(SphereSet * owner) {
		mParentSet = owner;
	}
	
	void Sphere::setColour(const ColourValue & colour) {
		mColour = colour;
	}
	
	const ColourValue & Sphere::getColour() const {
		return mColour;
	}
	
	Real Sphere::getOwnRadius() const {
		return mRadius;
	}

} /* namespace ParticleUniverse */
