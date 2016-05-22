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

#ifndef __PU_LIGHT_RENDERER_H__
#define __PU_LIGHT_RENDERER_H__

#include "ParticleUniverseIVisualData.h"
#include "ParticleUniverseMath.h"
#include "ParticleUniverseRenderer.h"

#include "OgreLight.h"

namespace ParticleUniverse {

	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport LightRendererVisualData : public IVisualData {
	public:
		Ogre::SceneNode * node;
		Ogre::Light * light;
		Real flashFrequencyCount;
		Real flashLengthCount;

		LightRendererVisualData(Ogre::SceneNode * sceneNode) : IVisualData(),  node(sceneNode), light(nullptr), flashFrequencyCount(0.0), flashLengthCount(0.0) {}

		virtual void setVisible(bool visible);
	};

	/** The LightRenderer class is responsible to render particles as a light.
	@remarks
		Note, that the diffuse colour cannot be set. This is, because the light inherits its diffuse colour from the particle. This makes
		it possible to manipulate the colour (for instance, using a Colour Affector).
    */
	class _ParticleUniverseExport LightRenderer : public ParticleRenderer {
	public:
		// Constants
		static const Ogre::Light::LightTypes DEFAULT_LIGHT_TYPE;
		static const ColourValue DEFAULT_DIFFUSE;
		static const ColourValue DEFAULT_SPECULAR;
		static const Real DEFAULT_ATT_RANGE;
		static const Real DEFAULT_ATT_CONSTANT;
		static const Real DEFAULT_ATT_LINEAR;
		static const Real DEFAULT_ATT_QUADRATIC;
		static const Radian DEFAULT_SPOT_INNER_ANGLE;
		static const Radian DEFAULT_SPOT_OUTER_ANGLE;
		static const Real DEFAULT_FALLOFF;
		static const Real DEFAULT_POWER_SCALE;

		LightRenderer();
	    virtual ~LightRenderer();

		/** Return the type of light that is emitted.
		*/
		Ogre::Light::LightTypes getLightType() const;

		/** Set the type of light that is emitted.
		*/
		void setLightType(Ogre::Light::LightTypes lightType);

		/** 
		*/
		const ColourValue & getSpecularColour() const;
		void setSpecularColour(const ColourValue & specularColour);

		/** 
		*/
		Real getAttenuationRange() const;
		void setAttenuationRange(Real attenuationRange);

		/** 
		*/
		Real getAttenuationConstant() const;
		void setAttenuationConstant(Real attenuationConstant);

		/** 
		*/
		Real getAttenuationLinear() const;
		void setAttenuationLinear(Real attenuationLinear);

		/** 
		*/
		Real getAttenuationQuadratic() const;
		void setAttenuationQuadratic(Real attenuationQuadratic);

		/** 
		*/
		const Radian & getSpotlightInnerAngle() const;
		void setSpotlightInnerAngle(const Radian & spotlightInnerAngle);

		/** 
		*/
		const Radian & getSpotlightOuterAngle() const;
		void setSpotlightOuterAngle(const Radian & spotlightOuterAngle);

		/** 
		*/
		Real getSpotlightFalloff() const;
		void setSpotlightFalloff(Real spotlightFalloff);

		/** 
		*/
		Real getPowerScale() const;
		void setPowerScale(Real powerScale);

		/** 
		*/
		Real getFlashFrequency() const;
		void setFlashFrequency(Real flashFrequency);

		/** 
		*/
		Real getFlashLength() const;
		void setFlashLength(Real flashLength);

		/** 
		*/
		bool isFlashRandom() const;
		void setFlashRandom(bool flashRandom);

		/** Deletes all ChildSceneNodes en Lights.
		*/
		void _destroyAll();

		/** @copydoc ParticleRenderer::setVisible */
		//virtual void setVisible(bool visible = true);

		/** @copydoc ParticleRenderer::_prepare */
		virtual void _prepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_unprepare */
		virtual void _unprepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_updateRenderQueue */
		//virtual void _updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool);

		/** @copydoc ParticleRenderer::_processParticle */
		inline virtual void _processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle);

		/** @copydoc ParticleRenderer::_setMaterialName */
		virtual void _setMaterialName(const String & materialName);

		/** @copydoc ParticleRenderer::_notifyCurrentCamera */
		virtual void _notifyCurrentCamera(Camera * cam);
	
		/** @copydoc ParticleRenderer::_notifyAttached */
		virtual void _notifyAttached(Ogre::Node * parent, bool isTagPoint = false);

		/** @copydoc ParticleRenderer::_notifyParticleQuota */
		virtual void _notifyParticleQuota(size_t quota);

		/** @copydoc ParticleRenderer::_notifyDefaultDimensions */
		virtual void _notifyDefaultDimensions(Real width, Real height, Real depth);

		/** @copydoc ParticleRenderer::_notifyParticleResized */
		virtual void _notifyParticleResized();

		/** @copydoc ParticleRenderer::_getSortMode */
		virtual SortMode _getSortMode() const;

		/** @copydoc ParticleRenderer::copyAttributesTo */
		virtual void copyAttributesTo(ParticleRenderer * renderer);

	protected:
		String mLightName; // Used for random light name prefix
		vector<LightRendererVisualData *> mAllVisualData;
		vector<LightRendererVisualData *> mVisualData;
		vector<Ogre::Light *> mLights;
		size_t mQuota;
		Ogre::Light::LightTypes mLightType;
		ColourValue mSpecularColour;
		Real mAttenuationRange;
		Real mAttenuationConstant;
		Real mAttenuationLinear;
		Real mAttenuationQuadratic;
		Radian mSpotlightInnerAngle;
		Radian mSpotlightOuterAngle;
		Real mSpotlightFalloff;
		Real mPowerScale;
		Real mFlashFrequency;
		Real mFlashLength;
		bool mFlashRandom;

		/** Make all nodes to which the entities are attached visible or invisible.
		*/
		void _makeNodesVisible(bool visible);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_LIGHT_RENDERER_H__ */
