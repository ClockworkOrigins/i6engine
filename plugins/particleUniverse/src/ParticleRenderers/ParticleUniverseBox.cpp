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

#include "ParticleRenderers/ParticleUniverseBox.h"

#include "ParticleRenderers/ParticleUniverseBoxSet.h"

namespace ParticleUniverse {
	
	Box::Box() : mPosition(Vector3::ZERO), mColour(ColourValue::White), mOrientation(Quaternion::IDENTITY), mParentSet(nullptr), mOwnDimensions(false), mWidth(10.0), mHeight(10.0), mDepth(10.0), mHalfWidth(5.0), mHalfHeight(5.0), mHalfDepth(5.0), mDimensionsChanged(false), mTempCorner(Vector3::ZERO) {
		_calculateCorners();
	}
    
	// TODO: (Daniel) take a look on this, why is position not set?
    Box::Box(const Vector3 & position, BoxSet * owner) : mPosition(Vector3::ZERO), mColour(ColourValue::White), mOrientation(Quaternion::IDENTITY), mParentSet(nullptr), mOwnDimensions(false), mWidth(10.0), mHeight(10.0), mDepth(10.0), mHalfWidth(5.0), mHalfHeight(5.0), mHalfDepth(5.0), mDimensionsChanged(false), mTempCorner(Vector3::ZERO) {
		_calculateCorners();
	}
	
	Box::~Box() {
	}
	
	void Box::_calculateCorners() {
		if (mDimensionsChanged) {
			mHalfWidth = 0.5 * mWidth;
			mHalfHeight = 0.5 * mHeight;
			mHalfDepth = 0.5 * mDepth;
			mDimensionsChanged = false;
		}

		mCorners[0].x = -mHalfWidth;
		mCorners[0].y = -mHalfHeight;
		mCorners[0].z = -mHalfDepth;

		mCorners[1].x = mHalfWidth;
		mCorners[1].y = -mHalfHeight;
		mCorners[1].z = -mHalfDepth;

		mCorners[2].x = -mHalfWidth;
		mCorners[2].y = mHalfHeight;
		mCorners[2].z = -mHalfDepth;

		mCorners[3].x = mHalfWidth;
		mCorners[3].y = mHalfHeight;
		mCorners[3].z = -mHalfDepth;

		mCorners[4].x = -mHalfWidth;
		mCorners[4].y = -mHalfHeight;
		mCorners[4].z = mHalfDepth;

		mCorners[5].x = mHalfWidth;
		mCorners[5].y = -mHalfHeight;
		mCorners[5].z = mHalfDepth;

		mCorners[6].x = -mHalfWidth;
		mCorners[6].y = mHalfHeight;
		mCorners[6].z = mHalfDepth;

		mCorners[7].x = mHalfWidth;
		mCorners[7].y = mHalfHeight;
		mCorners[7].z = mHalfDepth;
	}
	
	const Vector3 & Box::getCorner(size_t index) {
		assert(index < 8 && "Corner index out of bounds." );
		if (mOrientation == Quaternion::IDENTITY) {
			return mCorners[index];
		} else {
			mTempCorner = mOrientation * mCorners[index];
			return mTempCorner;
		}
	}
	
	const Vector3 & Box::getWorldspaceCorner(size_t index) {
		assert(index < 8 && "Corner index out of bounds." );
		if (mOrientation == Quaternion::IDENTITY) {
			mTempCorner = mPosition + mCorners[index];
		} else {
			mTempCorner = mPosition + mOrientation * mCorners[index];
		}

		return mTempCorner;
	}
	
	void Box::setPosition(const Vector3 & position) {
		mPosition = position;
		_calculateCorners();
	}
	
	void Box::setPosition(Real x, Real y, Real z) {
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		_calculateCorners();
	}
	
	const Vector3 & Box::getPosition() const {
		return mPosition;
	}
	
	void Box::setDimensions(Real width, Real height, Real depth) {
		mOwnDimensions = true;
		mWidth = width;
		mHeight = height;
		mDepth = depth;
		if (mParentSet) {
			mParentSet->_notifyResized();
		}
		mDimensionsChanged = true;
		_calculateCorners();
    }
	
	bool Box::hasOwnDimensions() const {
		return mOwnDimensions;
	}
	
	void Box::_notifyOwner(BoxSet * owner) {
		mParentSet = owner;
	}
	
	void Box::setColour(const ColourValue & colour) {
		mColour = colour;
	}
	
	const ColourValue & Box::getColour() const {
		return mColour;
	}
	
	Real Box::getOwnWidth() const {
		return mWidth;
	}
	
	Real Box::getOwnHeight() const {
		return mHeight;
	}
	
	Real Box::getOwnDepth() const {
		return mDepth;
	}

} /* namespace ParticleUniverse */
