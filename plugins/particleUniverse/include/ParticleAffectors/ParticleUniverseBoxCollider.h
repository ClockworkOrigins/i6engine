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

#ifndef __PU_BOX_COLLIDER_H__
#define __PU_BOX_COLLIDER_H__

#include "ParticleUniverseAxisAlignedBox.h"
#include "ParticleUniverseBaseCollider.h"

namespace ParticleUniverse {

	/** The BoxCollider is a box shape that collides with the particles. The BoxCollider can only collide with
		particles that are created within the same ParticleTechnique as where the BoxCollider is registered.
    */
	class _ParticleUniverseExport BoxCollider : public BaseCollider {
	public:
		using Particle::copyAttributesTo;

		static const Real DEFAULT_WIDTH;
		static const Real DEFAULT_HEIGHT;
		static const Real DEFAULT_DEPTH;

		BoxCollider();
	    virtual ~BoxCollider() {}

		/** Returns the width of the box
		*/
		Real getWidth() const;

		/** Sets the width of the box
		*/
		void setWidth(const Real width);

		/** Returns the height of the box
		*/
		Real getHeight() const;

		/** Sets the height of the box
		*/
		void setHeight(const Real height);

		/** Returns the depth of the box
		*/
		Real getDepth() const;

		/** Sets the depth of the box
		*/
		void setDepth(const Real depth);

		/** Returns indication whether the collision is inside or outside of the box
		@remarks
			If value is true, the collision is inside of the box.
		*/
		bool isInnerCollision() const;

		/** Set indication whether the collision is inside or outside of the box
		@remarks
			If value is set to true, the collision is inside of the box.
		*/
		void setInnerCollision(bool innerCollision);

		/** 
		*/
		void calculateDirectionAfterCollision(Particle * particle);

		/** @copydoc ParticleAffector::_preProcessParticles */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo (ParticleAffector * affector);

	protected:
		Real mWidth;
		Real mHeight;
		Real mDepth;
		Real mXmin;
		Real mXmax;
		Real mYmin;
		Real mYmax;
		Real mZmin;
		Real mZmax;
		AxisAlignedBox mBox;
		Vector3 mPredictedPosition;
		bool mInnerCollision;

		/** 
		*/
		void _calculateBounds();

		/** 
		*/
		bool _isSmallestValue(Real value, const Vector3 & particlePosition);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BOX_COLLIDER_H__ */
