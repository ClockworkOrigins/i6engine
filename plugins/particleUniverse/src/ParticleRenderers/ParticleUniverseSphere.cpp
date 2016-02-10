/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleRenderers/ParticleUniverseSphere.h"

#include "ParticleRenderers/ParticleUniverseSphereSet.h"

namespace ParticleUniverse {
	
	Sphere::Sphere() : mOwnRadius(false), mRadius(100.0), mRadiusChanged(false), mPosition(Vector3::ZERO), mColour(ColourValue::White), mParentSet(nullptr) {
		mOrientation = Quaternion::IDENTITY;
	}
    
    Sphere::Sphere(const Vector3 & position, SphereSet * owner) : mOwnRadius(false), mRadius(10.0), mRadiusChanged(false), mPosition(Vector3::ZERO), mColour(ColourValue::White), mParentSet(nullptr) {
		mOrientation = Quaternion::IDENTITY;
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
