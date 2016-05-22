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

#ifndef __PU_RIBBONTRAIL_RENDERER_H__
#define __PU_RIBBONTRAIL_RENDERER_H__

#include "ParticleUniverseIVisualData.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseTechniqueListener.h"

namespace ParticleUniverse {

	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport RibbonTrailRendererVisualData : public IVisualData {
	public:
		Ogre::SceneNode * node;
		bool addedToTrail;
		Ogre::RibbonTrail * trail;
		size_t index;

		// Constructor
		RibbonTrailRendererVisualData(Ogre::SceneNode * sceneNode, Ogre::RibbonTrail * ribbonTrail) : IVisualData(), node(sceneNode), addedToTrail(false), trail(ribbonTrail), index(0) {}

		virtual void setVisible(bool visible);
	};

	/** The RibbonTrailRenderer class is responsible to render particles as a RibbonTrail.
    */
	class _ParticleUniverseExport RibbonTrailRenderer : public ParticleRenderer, public TechniqueListener {
	public:
		// Constants
		static const bool DEFAULT_USE_VERTEX_COLOURS;
		static const size_t DEFAULT_MAX_ELEMENTS;
		static const Real DEFAULT_LENGTH;
		static const Real DEFAULT_WIDTH;
		static const bool DEFAULT_RANDOM_INITIAL_COLOUR;
		static const ColourValue DEFAULT_INITIAL_COLOUR;
		static const ColourValue DEFAULT_COLOUR_CHANGE;

		RibbonTrailRenderer();
	    virtual ~RibbonTrailRenderer();

		/** Notify that the Particle System is rescaled.
	    */
		virtual void _notifyRescaled(const Vector3 & scale);

		/** Getters and Setters
		*/
		bool isUseVertexColours() const;
		void setUseVertexColours(bool useVertexColours);

		size_t getMaxChainElements() const;
		void setMaxChainElements(size_t maxChainElements);

		Real getTrailLength() const;
		void setTrailLength(Real trailLength);

		Real getTrailWidth() const;
		void setTrailWidth(Real trailWidth);

		bool isRandomInitialColour() const;
		void setRandomInitialColour(bool randomInitialColour);

		const ColourValue & getInitialColour() const;
		void setInitialColour(const ColourValue & initialColour);

		const ColourValue & getColourChange() const;
		void setColourChange(const ColourValue & colourChange);

		/** Deletes all ChildSceneNodes en Entities.
		*/
		void _destroyAll();
			
		/** Enable the RibbonTrail
		*/
//			virtual void _notifyStart();

		/** Disable the RibbonTrail
		*/
//			virtual void _notifyStop();
			
		/** Make all objects visible or invisible.
		*/
		virtual void setVisible(bool visible);

		/** @copydoc ParticleRenderer::_prepare */
		virtual void _prepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_unprepare */
		virtual void _unprepare(ParticleTechnique * technique);

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
		virtual void _notifyCurrentCamera(Camera* cam);
	
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

		/*  See TechniqueListener.
		*/
		virtual void particleEmitted(ParticleTechnique * particleTechnique, Particle * particle);

		/*  See TechniqueListener.
		*/
		virtual void particleExpired(ParticleTechnique * particleTechnique, Particle * particle);

	protected:
		vector<RibbonTrailRendererVisualData *> mAllVisualData;
		vector<RibbonTrailRendererVisualData *> mVisualData;
		size_t mQuota;
		Ogre::RibbonTrail * mTrail;
		String mRibbonTrailName;
		bool mUseVertexColours;
		size_t mMaxChainElements;
		Real mTrailLength;
		Real mTrailWidth;
		bool mRandomInitialColour;
		bool mSetLength;
		bool mSetWidth;
		ColourValue mInitialColour;
		ColourValue mColourChange;
		Ogre::SceneNode * mChildNode;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_RIBBONTRAIL_RENDERER_H__ */
