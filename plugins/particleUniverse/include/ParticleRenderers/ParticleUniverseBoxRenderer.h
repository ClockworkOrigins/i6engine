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

#ifndef __PU_BOX_RENDERER_H__
#define __PU_BOX_RENDERER_H__

#include "ParticleUniverseRenderer.h"

namespace ParticleUniverse {

	class BoxSet;

	/** The BoxRenderer class is responsible to render particles as a box.
    */
	class _ParticleUniverseExport BoxRenderer : public ParticleRenderer {
	public:
		BoxRenderer();
	    virtual ~BoxRenderer();

		/** @copydoc ParticleRenderer::_prepare */
		virtual void _prepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_unprepare */
		virtual void _unprepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_updateRenderQueue */
		virtual void _updateRenderQueue(Ogre::RenderQueue * queue, ParticlePool * pool);

		/** @copydoc ParticleRenderer::_notifyAttached */
		virtual void _notifyAttached(Ogre::Node * parent, bool isTagPoint = false);

		/** @copydoc ParticleRenderer::_setMaterialName */
		void _setMaterialName(const String & materialName);

		/** @copydoc ParticleRenderer::_notifyCurrentCamera */
		virtual void _notifyCurrentCamera(Camera * cam);
	
		/** @copydoc ParticleRenderer::_notifyParticleQuota */
		virtual void _notifyParticleQuota(size_t quota);

		/** @copydoc ParticleRenderer::_notifyDefaultDimensions */
		virtual void _notifyDefaultDimensions(Real width, Real height, Real depth);

		/** @copydoc ParticleRenderer::_notifyParticleResized */
		virtual void _notifyParticleResized();

		/** @copydoc ParticleRenderer::_notifyParticleZRotated */
		virtual void _notifyParticleZRotated();

		/** @copydoc ParticleRenderer::setRenderQueueGroup */
		virtual void setRenderQueueGroup(uint8 queueId);

		/** @copydoc ParticleRenderer::_getSortMode */
		virtual SortMode _getSortMode() const { return SM_DISTANCE; }

		/** @copydoc ParticleRenderer::copyAttributesTo */
		virtual void copyAttributesTo(ParticleRenderer * renderer);

		/** Access BoxSet in use */
		BoxSet * getBoxSet() const { return mBoxSet; }

		/** @copydoc ParticleRenderer::setVisible */
		virtual void setVisible(bool visible);

	protected:
		/** The Box set that's doing the rendering */
		BoxSet * mBoxSet;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BOX_RENDERER_H__ */
