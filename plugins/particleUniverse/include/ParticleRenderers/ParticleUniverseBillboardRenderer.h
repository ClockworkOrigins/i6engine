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

#ifndef __PU_BILLBOARD_RENDERER_H__
#define __PU_BILLBOARD_RENDERER_H__

#include "ParticleUniverseRenderer.h"

#include "OGRE/OgreBillboard.h"
#include "OGRE/OgreBillboardSet.h"

namespace ParticleUniverse {

	/** The BillboardRenderer class is responsible to render particles as a billboard.
    */
	class _ParticleUniverseExport BillboardRenderer : public ParticleRenderer {
	public:
		/** Alternative billboard type. 
		@remarks
			This is an extended version of Ogre's billboard type. It has BBT_ORIENTED_SHAPE added
			which basicly means that the billboard orientation is derived from the particle 
			orientation.
		*/
		enum BillboardType {
			BBT_POINT,
			BBT_ORIENTED_COMMON,
			BBT_ORIENTED_SELF,
			BBT_ORIENTED_SHAPE,
			BBT_PERPENDICULAR_COMMON,
			BBT_PERPENDICULAR_SELF
		};

		// Constants
		static const BillboardType DEFAULT_BILLBOARD_TYPE;
		static const bool DEFAULT_ACCURATE_FACING;
		static const Ogre::BillboardOrigin DEFAULT_ORIGIN;
		static const Ogre::BillboardRotationType DEFAULT_ROTATION_TYPE;
		static const Vector3 DEFAULT_COMMON_DIRECTION;
		static const Vector3 DEFAULT_COMMON_UP_VECTOR;
		static const bool DEFAULT_POINT_RENDERING;

		BillboardRenderer();
	    virtual ~BillboardRenderer();

		/** @copydoc ParticleRenderer::_prepare */
		virtual void _prepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_unprepare */
		virtual void _unprepare(ParticleTechnique * technique);

		/** 
		*/
		void setBillboardType(BillboardType bbt);

		/** 
		*/
		BillboardType getBillboardType() const;

		/** 
		*/
		void setUseAccurateFacing(bool acc);

		/** 
		*/
		bool isUseAccurateFacing() const;

		/** 
		*/
		void setBillboardOrigin(Ogre::BillboardOrigin origin) { mBillboardSet->setBillboardOrigin(origin); }

		/** 
		*/
		Ogre::BillboardOrigin getBillboardOrigin() const { return mBillboardSet->getBillboardOrigin(); }

		/** 
		*/
		void setBillboardRotationType(Ogre::BillboardRotationType rotationType);

		/** 
		*/
		Ogre::BillboardRotationType getBillboardRotationType() const;

		/** 
		*/
		void setCommonDirection(const Vector3 & vec);

		/** 
		*/
		const Vector3 & getCommonDirection() const;

		/** 
		*/
		void setCommonUpVector(const Vector3 & vec);

		/** 
		*/
		const Vector3 & getCommonUpVector() const;

		/** 
		*/
		void setPointRenderingEnabled(bool enabled);

		/** 
		*/
		bool isPointRenderingEnabled() const;

		/** 
		*/
		virtual void _updateRenderQueue(Ogre::RenderQueue * queue, ParticlePool * pool);

		/** 
		*/
		virtual void _notifyAttached(Ogre::Node * parent, bool isTagPoint = false);

		/** @copydoc ParticleRenderer::_setMaterialName */
		virtual void _setMaterialName(const String & materialName);

		/** 
		*/
		virtual void _notifyCurrentCamera(Camera * cam);
	
		/** 
		*/
		virtual void _notifyParticleQuota(size_t quota);

		/** 
		*/
		virtual void _notifyDefaultDimensions(Real width, Real height, Real depth);

		/** 
		*/
		virtual void _notifyParticleResized();

		/** 
		*/
		virtual void _notifyParticleZRotated();

		/** 
		*/
		virtual void setRenderQueueGroup(uint8 queueId);

		/** 
		*/
		virtual SortMode _getSortMode() const;

		/** 
		*/
		virtual void copyAttributesTo(ParticleRenderer * renderer);

		/** 
		*/
		Ogre::BillboardSet * getBillboardSet() const { return mBillboardSet; }

		/** @copydoc ParticleRenderer::setVisible */
		virtual void setVisible(bool visible);

	protected:
		/** 
		*/
		Ogre::BillboardSet * mBillboardSet;

		/** 
		*/
		BillboardType mBillboardType;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BILLBOARD_RENDERER_H__ */
