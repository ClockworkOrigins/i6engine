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

#ifndef __PU_MATH_H__
#define __PU_MATH_H__

#include "OgreMath.h"

namespace ParticleUniverse {

	// If the Ogre renderer is replaced by another renderer, the types below must be re-implemented.
	typedef Ogre::Radian Radian;
	typedef Ogre::Angle Angle;
	typedef Ogre::Math Math;

	// To be re-implemented:
	// ====================
	// Math::Abs
	// Math::Ceil
	// Math::Cos
	// Math::RangeRandom
	// Math::Sin
	// Math::Sign
	// Math::Sqrt
	// Math::UnitRandom
	// Math::SymmetricRandom

	// Math::POS_INFINITY
	// Math::NEG_INFINITY
	// Math::TWO_PI
	// Math::PI

} /* namespace ParticleUniverse */

#endif /* __PU_MATH_H__ */
