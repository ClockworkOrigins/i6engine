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

#ifndef __PU_BOX_H__
#define __PU_BOX_H__

#include "ParticleUniversePrerequisites.h"

#include "OGRE/OgreMemoryAllocatorConfig.h"

namespace ParticleUniverse {

	class BoxSet;

	/** 
	 */
	class _ParticleUniverseExport Box : public FXAlloc {
		friend class BoxSet;
		friend class BoxRenderer;

	public:
		Vector3 mPosition;
		ColourValue mColour;
		Quaternion mOrientation;
		BoxSet * mParentSet;
			
		/** Default constructor.
		 */
		Box();

		/** Default destructor.
		 */
		virtual ~Box();

		/** Constructor as called by BoxSet.
		*/
		Box(const Vector3 & position, BoxSet * owner);

		/** Return the boxcorner in local space of a specified corner.
		*/
		const Vector3 & getCorner(size_t index);

		/** Return the boxcorner in world space of a specified corner.
		*/
		const Vector3 & getWorldspaceCorner(size_t index);

		/** 
		*/
		void setPosition(const Vector3 & position);

		/** 
		*/
		void setPosition(Real x, Real y, Real z);

		/** 
		*/
		const Vector3 & getPosition() const;

		/** Sets the colour of this box.
		*/
		void setColour(const ColourValue & colour);

		/** Get the colour of this box.
		*/
		const ColourValue & getColour() const;
		
		/** Resets this box to use the parent BoxSet's dimensions instead of it's own.
		*/
		void resetDimensions() { mOwnDimensions = false; }

		/** Sets the width, height and depth for this box.
		*/
		void setDimensions(Real width, Real height, Real depth);

		/** Returns true if this box deviates from the BoxSet's default dimensions.
		@see
			Box::setDimensions
		*/
		bool hasOwnDimensions() const;

		/** Retrieves the box's personal width, if hasOwnDimensions is true.
		*/
		Real getOwnWidth() const;

		/** Retrieves the box's personal height, if hasOwnDimensions is true.
		*/
		Real getOwnHeight() const;

		/** Retrieves the box's personal depth, if hasOwnDimensions is true.
		*/
		Real getOwnDepth() const;

		/** Internal method for notifying the box of it's owner.
		*/
		void _notifyOwner(BoxSet * owner);

	protected:
		bool mOwnDimensions;
		Real mWidth;
		Real mHeight;
		Real mDepth;
		Real mHalfWidth;
		Real mHalfHeight;
		Real mHalfDepth;
		bool mDimensionsChanged;

		/** Corners of the box in local space.
		*/
		Vector3 mCorners[8];
		Vector3 mTempCorner;

		/** Calculates the corners.
		*/
		void _calculateCorners();
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BOX_H__ */
