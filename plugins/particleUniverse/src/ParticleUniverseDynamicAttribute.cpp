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

#include "ParticleUniverseDynamicAttribute.h"

#include "ParticleUniverseMath.h"

namespace ParticleUniverse {

	DynamicAttribute::DynamicAttribute() : IElement() {
	}

	DynamicAttribute::~DynamicAttribute() {
	}

	DynamicAttribute::DynamicAttributeType DynamicAttribute::getType() const {
		return mType;
	}

	void DynamicAttribute::setType(DynamicAttribute::DynamicAttributeType type) {
		mType = type;
	}

	DynamicAttributeFixed::DynamicAttributeFixed() : mValue(0) {
		mType = DynamicAttribute::DAT_FIXED;
	}

	DynamicAttributeFixed::DynamicAttributeFixed(const DynamicAttributeFixed & dynamicAttributeFixed) {
		mType = DynamicAttribute::DAT_FIXED;
		mValue = dynamicAttributeFixed.mValue;
	}

	DynamicAttributeFixed::~DynamicAttributeFixed() {
	}

	void DynamicAttributeFixed::setValue(Real value) {
		mValue = value;
	}

	Real DynamicAttributeFixed::getValue(Real x) {
		return mValue;
	}

	void DynamicAttributeFixed::copyAttributesTo(DynamicAttribute * dynamicAttribute) {
		if (!dynamicAttribute || dynamicAttribute->getType() != DynamicAttribute::DAT_FIXED) {
			return;
		}

		DynamicAttributeFixed * dynAttr = static_cast<DynamicAttributeFixed *>(dynamicAttribute);
		dynAttr->mValue = mValue;
	}

	DynamicAttributeRandom::DynamicAttributeRandom() : mMin(0), mMax(0) {
		mType = DynamicAttribute::DAT_RANDOM;
	}

	DynamicAttributeRandom::DynamicAttributeRandom(const DynamicAttributeRandom & dynamicAttributeRandom) {
		mType = DynamicAttribute::DAT_RANDOM;
		mMin = dynamicAttributeRandom.mMin;
		mMax = dynamicAttributeRandom.mMax;
	}

	DynamicAttributeRandom::~DynamicAttributeRandom() {
	}

	Real DynamicAttributeRandom::getMin() const {
		return mMin;
	}

	void DynamicAttributeRandom::setMin(Real min) {
		mMin = min;
	}

	Real DynamicAttributeRandom::getMax() const {
		return mMax;
	}

	void DynamicAttributeRandom::setMax(Real max) {
		mMax = max;
	}

	void DynamicAttributeRandom::setMinMax (Real min, Real max) {
		mMin = min;
		mMax = max;
	}

	Real DynamicAttributeRandom::getValue(Real x) {
		return Math::RangeRandom(mMin, mMax);
	}

	void DynamicAttributeRandom::copyAttributesTo(DynamicAttribute * dynamicAttribute) {
		if (!dynamicAttribute || dynamicAttribute->getType() != DynamicAttribute::DAT_RANDOM) {
			return;
		}

		DynamicAttributeRandom * dynAttr = static_cast<DynamicAttributeRandom *>(dynamicAttribute);
		dynAttr->mMin = mMin;
		dynAttr->mMax = mMax;
	}

	DynamicAttributeCurved::DynamicAttributeCurved() : mRange(0), mInterpolationType(IT_LINEAR) {
		mType = DynamicAttribute::DAT_CURVED;
	}

	DynamicAttributeCurved::DynamicAttributeCurved(InterpolationType interpolationType) : mRange(0), mInterpolationType(interpolationType) {
		mType = DynamicAttribute::DAT_CURVED;
	}

	DynamicAttributeCurved::DynamicAttributeCurved(const DynamicAttributeCurved & dynamicAttributeCurved) {
		mType = DynamicAttribute::DAT_CURVED;
		mInterpolationType = dynamicAttributeCurved.mInterpolationType;
		mSpline = dynamicAttributeCurved.mSpline;
		mRange = dynamicAttributeCurved.mRange;

		// Copy controlpoints
		for (DynamicAttributeCurved::ControlPointList::const_iterator it = dynamicAttributeCurved.mControlPoints.begin(); it != dynamicAttributeCurved.mControlPoints.end(); ++it) {
			Vector2 controlPoint = *it;
			mControlPoints.push_back (controlPoint);
		}
		processControlPoints();
	}

	DynamicAttributeCurved::~DynamicAttributeCurved() {
	}

	void DynamicAttributeCurved::setInterpolationType(InterpolationType interpolationType) {
		if (interpolationType != mInterpolationType) {
			// If switched to another InterpolationType, the already existing ControlPoints will be removed.
			removeAllControlPoints();
			mInterpolationType = interpolationType;
		}
	}

	InterpolationType DynamicAttributeCurved::getInterpolationType() const {
		return mInterpolationType;
	}

	Real DynamicAttributeCurved::getValue(Real x) {
		switch (mInterpolationType)
		{
		case IT_LINEAR: {
			// Search the interval in which 'x' resides and apply linear interpolation
			if (mControlPoints.empty()) {
				return 0;
			}

			ControlPointList::iterator it1 = _findNearestControlPointIterator(x);
			ControlPointList::iterator it2 = it1 + 1;
			if (it2 != mControlPoints.end()) {
				// Calculate fraction: y = y1 + ((y2 - y1) * (x - x1)/(x2 - x1))
				return (*it1).y + (((*it2).y - (*it1).y) * (x - (*it1).x)/((*it2).x - (*it1).x));
			} else {
				return (*it1).y;
			}
		}

		case IT_SPLINE: {
			// Fit using spline
			if (mSpline.getNumPoints() < 1) {
				return 0;
			}

			Real fraction = x / mRange;
			return (mSpline.interpolate(fraction < 1.0f ? fraction : 1.0f)).y;
		}
		default: {
			break;
		}
		}

		return 0;
	}

	void DynamicAttributeCurved::addControlPoint(Real x, Real y) {
		mControlPoints.push_back(Vector2(x, y));
	}

	const DynamicAttributeCurved::ControlPointList & DynamicAttributeCurved::getControlPoints() const {
		return mControlPoints;
	}

	void DynamicAttributeCurved::processControlPoints() {
		if (mControlPoints.empty()) {
			return;
		}

		std::sort(mControlPoints.begin(), mControlPoints.end(), ControlPointSorter());
		mRange = (*_getLastValidIterator()).x - (*_getFirstValidIterator()).x;

		if (mInterpolationType == IT_SPLINE) {
			// Add all sorted control points to the spline
			mSpline.clear();
			for (DynamicAttributeCurved::ControlPointList::iterator it = mControlPoints.begin(); it != mControlPoints.end(); ++it) {
				mSpline.addPoint(Vector3((*it).x, (*it).y, 0));
			}
		}
	}

	size_t DynamicAttributeCurved::getNumControlPoints() const {
		return mControlPoints.size();
	}

	void DynamicAttributeCurved::removeAllControlPoints() {
		mControlPoints.clear();
	}

	DynamicAttributeCurved::ControlPointList::iterator DynamicAttributeCurved::_findNearestControlPointIterator(Real x) {
		// Assume that the ControlPointList is not empty
		ControlPointList::iterator it;
		ControlPointList::iterator itEnd = mControlPoints.end();
		for (it = mControlPoints.begin(); it != itEnd; ++it) {
			if (x < (*it).x) {
				if (it == mControlPoints.begin()) {
					return it;
				} else {
					return --it;
				}
			}
		}

		// If not found return the last valid iterator
		return --it;
	}

	DynamicAttributeCurved::ControlPointList::iterator DynamicAttributeCurved::_getFirstValidIterator() {
		return mControlPoints.begin();
	}

	DynamicAttributeCurved::ControlPointList::iterator DynamicAttributeCurved::_getLastValidIterator() {
		return mControlPoints.end() - 1;
	}

	void DynamicAttributeCurved::copyAttributesTo(DynamicAttribute * dynamicAttribute) {
		if (!dynamicAttribute || dynamicAttribute->getType() != DynamicAttribute::DAT_CURVED) {
			return;
		}

		DynamicAttributeCurved * dynAttr = static_cast<DynamicAttributeCurved *>(dynamicAttribute);
		dynAttr->mInterpolationType = mInterpolationType;
		dynAttr->mSpline = mSpline;
		dynAttr->mRange = mRange;

		// Copy controlpoints
		for (DynamicAttributeCurved::ControlPointList::const_iterator it = mControlPoints.begin(); it != mControlPoints.end(); ++it) {
			Vector2 controlPoint = *it;
			dynAttr->mControlPoints.push_back(controlPoint);
		}
		dynAttr->processControlPoints();
	}

	DynamicAttributeOscillate::DynamicAttributeOscillate() : mOscillationType(DynamicAttributeOscillate::OSCT_SINE), mFrequency(1.0), mPhase(0.0), mBase(0.0), mAmplitude(1.0) {
		mType = DynamicAttribute::DAT_OSCILLATE;
	}

	DynamicAttributeOscillate::DynamicAttributeOscillate(const DynamicAttributeOscillate & dynamicAttributeOscillate) {
		mType = DynamicAttribute::DAT_OSCILLATE;
	}

	DynamicAttributeOscillate::~DynamicAttributeOscillate() {
	}

	DynamicAttributeOscillate::OscillationType DynamicAttributeOscillate::getOscillationType() const {
		return mOscillationType;
	}

	void DynamicAttributeOscillate::setOscillationType(DynamicAttributeOscillate::OscillationType oscillationType) {
		mOscillationType = oscillationType;
	}

	Real DynamicAttributeOscillate::getFrequency() const {
		return mFrequency;
	}

	void DynamicAttributeOscillate::setFrequency(Real frequency) {
		mFrequency = frequency;
	}

	Real DynamicAttributeOscillate::getPhase() const {
		return mPhase;
	}

	void DynamicAttributeOscillate::setPhase(Real phase) {
		mPhase = phase;
	}

	Real DynamicAttributeOscillate::getBase() const {
		return mBase;
	}

	void DynamicAttributeOscillate::setBase(Real base) {
		mBase = base;
	}

	Real DynamicAttributeOscillate::getAmplitude() const {
		return mAmplitude;
	}

	void DynamicAttributeOscillate::setAmplitude(Real amplitude) {
		mAmplitude = amplitude;
	}

	Real DynamicAttributeOscillate::getValue(Real x) {
		switch (mOscillationType) {
		case OSCT_SINE: {
			return mBase + mAmplitude * Math::Sin(mPhase + mFrequency * x * Math::TWO_PI);
		}
		case OSCT_SQUARE: {
			return mBase + mAmplitude * Math::Sign(Math::Sin(mPhase + mFrequency * x * Math::TWO_PI));
		}
		default: {
			break;
		}
		}

		return 0;
	}

	void DynamicAttributeOscillate::copyAttributesTo(DynamicAttribute * dynamicAttribute) {
		if (!dynamicAttribute || dynamicAttribute->getType() != DynamicAttribute::DAT_OSCILLATE) {
			return;
		}

		DynamicAttributeOscillate * dynAttr = static_cast<DynamicAttributeOscillate *>(dynamicAttribute);
		dynAttr->mOscillationType = mOscillationType;
		dynAttr->mFrequency = mFrequency;
		dynAttr->mPhase = mPhase;
		dynAttr->mBase = mBase;
		dynAttr->mAmplitude = mAmplitude;
	}

	Real DynamicAttributeHelper::calculate(DynamicAttribute * dyn, Real x, Real defaultValue) {
		if (dyn) {
			return dyn->getValue(x);
		}

		return defaultValue;
	}

} /* namespace ParticleUniverse */
