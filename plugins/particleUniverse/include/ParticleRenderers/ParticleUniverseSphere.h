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

#ifndef __PU_SPHERE_H__
#define __PU_SPHERE_H__

#include "ParticleUniversePrerequisites.h"

#include "OGRE/OgreMemoryAllocatorConfig.h"

namespace ParticleUniverse {

	class SphereSet;

	/** 
	 */
	class _ParticleUniverseExport Sphere : public FXAlloc {
		friend class SphereSet;
		friend class SphereRenderer;

	public:
		Vector3 mPosition;
		ColourValue mColour;
		Quaternion mOrientation;
		SphereSet * mParentSet;
			
		/** Default constructor.
		*/
		Sphere();

		/** Default destructor.
		*/
		virtual ~Sphere();

		/** Constructor as called by SphereSet.
		*/
		Sphere(const Vector3 & position, SphereSet * owner);

		/** 
		*/
		void setPosition(const Vector3 & position);

		/** 
		*/
		void setPosition(Real x, Real y, Real z);

		/** 
		*/
		const Vector3 & getPosition() const;

		/** Sets the colour of this sphere.
		*/
		void setColour(const ColourValue & colour);

		/** Get the colour of this sphere.
		*/
		const ColourValue & getColour() const;
		
		/** Resets this sphere to use the parent SphereSet's radius instead of it's own.
		*/
		void resetRadius() { mOwnRadius = false; }

		/** Sets the radius for this sphere.
		*/
		void setRadius(Real radius);

		/** Returns true if this box deviates from the SphereSet's default radius.
		*/
		bool hasOwnRadius() const;

		/** Retrieves the sphere's personal radius, if hasOwnRadius is true.
		*/
		Real getOwnRadius() const;

		/** Internal method for notifying the box of it's owner.
		*/
		void _notifyOwner(SphereSet * owner);

	protected:
		bool mOwnRadius;
		Real mRadius;
		bool mRadiusChanged;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SPHERE_H__ */
