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

#include "ParticleRenderers/ParticleUniverseLightRenderer.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

#include "OgreSceneManager.h"

namespace ParticleUniverse {

	void LightRendererVisualData::setVisible(bool visible) {
		if (node) {
			node->setVisible(visible);
		}
	}

	// Constants
	const Ogre::Light::LightTypes LightRenderer::DEFAULT_LIGHT_TYPE = Ogre::Light::LT_POINT;
	const ColourValue LightRenderer::DEFAULT_DIFFUSE(0, 0, 0);
	const ColourValue LightRenderer::DEFAULT_SPECULAR(0, 0, 0);
	const Real LightRenderer::DEFAULT_ATT_RANGE = 100000;
	const Real LightRenderer::DEFAULT_ATT_CONSTANT = 1.0;
	const Real LightRenderer::DEFAULT_ATT_LINEAR = 0.0;
	const Real LightRenderer::DEFAULT_ATT_QUADRATIC = 0.0;
	const Radian LightRenderer::DEFAULT_SPOT_INNER_ANGLE = Ogre::Degree(30.0);
	const Radian LightRenderer::DEFAULT_SPOT_OUTER_ANGLE = Ogre::Degree(40.0);
	const Real LightRenderer::DEFAULT_FALLOFF = 1.0;
	const Real LightRenderer::DEFAULT_POWER_SCALE = 1.0;

	
	LightRenderer::LightRenderer() : ParticleRenderer(), mLightName(BLANK_STRING), mQuota(0), mLightType(DEFAULT_LIGHT_TYPE), mSpecularColour(DEFAULT_SPECULAR), mAttenuationRange(DEFAULT_ATT_RANGE), mAttenuationConstant(DEFAULT_ATT_CONSTANT), mAttenuationLinear(DEFAULT_ATT_LINEAR), mAttenuationQuadratic(DEFAULT_ATT_QUADRATIC), mSpotlightInnerAngle(DEFAULT_SPOT_INNER_ANGLE), mSpotlightOuterAngle(DEFAULT_SPOT_OUTER_ANGLE), mSpotlightFalloff(DEFAULT_FALLOFF), mPowerScale(DEFAULT_POWER_SCALE), mFlashFrequency(0.0), mFlashLength(0.0), mFlashRandom(false) {
		autoRotate = false;
	}
	
	LightRenderer::~LightRenderer() {
		if (!mParentTechnique) {
			return;
		}
		
		_destroyAll();
	}
	
	void LightRenderer::_destroyAll() {
		if (!mParentTechnique) {
			return;
		}

		// Note: The created ChildSceneNodes are destroyed when the ParticleSystem is destroyed
		for (vector<LightRendererVisualData *>::const_iterator it = mAllVisualData.begin(); it != mAllVisualData.end(); ++it) {
			PU_DELETE_T(*it, LightRendererVisualData, MEMCATEGORY_SCENE_OBJECTS);
		}

		mAllVisualData.clear();
		mVisualData.clear();

		// Destroy the Lights. Do it like this, because it must be assured that the light still exists
		// and has not already been destroyed.
		Ogre::SceneManager * sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
		for (size_t i = 0; i < mQuota; i++) {
			if (sceneManager->hasLight(mLightName + StringConverter::toString(i))) {
				sceneManager->destroyLight(mLightName + StringConverter::toString(i));
			}
		}
		mLights.clear();

		// Reset the visual data in the pool
		mParentTechnique->initVisualDataInPool();
	}
	
	Ogre::Light::LightTypes LightRenderer::getLightType() const {
		return mLightType;
	}
	
	void LightRenderer::setLightType(Ogre::Light::LightTypes lightType) {
		mLightType = lightType;
	}
	
	const ColourValue & LightRenderer::getSpecularColour() const {
		return mSpecularColour;
	}
	
	void LightRenderer::setSpecularColour(const ColourValue & specularColour) {
		mSpecularColour = specularColour;
	}
	
	Real LightRenderer::getAttenuationRange() const {
		return mAttenuationRange;
	}
	
	void LightRenderer::setAttenuationRange(Real attenuationRange) {
		mAttenuationRange = attenuationRange;
	}
	
	Real LightRenderer::getAttenuationConstant() const {
		return mAttenuationConstant;
	}
	
	void LightRenderer::setAttenuationConstant(Real attenuationConstant) {
		mAttenuationConstant = attenuationConstant;
	}
	
	Real LightRenderer::getAttenuationLinear() const {
		return mAttenuationLinear;
	}
	
	void LightRenderer::setAttenuationLinear(Real attenuationLinear) {
		mAttenuationLinear = attenuationLinear;
	}
	
	Real LightRenderer::getAttenuationQuadratic() const {
		return mAttenuationQuadratic;
	}
	
	void LightRenderer::setAttenuationQuadratic(Real attenuationQuadratic) {
		mAttenuationQuadratic = attenuationQuadratic;
	}
	
	const Radian & LightRenderer::getSpotlightInnerAngle() const {
		return mSpotlightInnerAngle;
	}
	
	void LightRenderer::setSpotlightInnerAngle(const Radian & spotlightInnerAngle) {
		mSpotlightInnerAngle = spotlightInnerAngle;
	}
	
	const Radian & LightRenderer::getSpotlightOuterAngle() const {
		return mSpotlightOuterAngle;
	}
	
	void LightRenderer::setSpotlightOuterAngle(const Radian & spotlightOuterAngle) {
		mSpotlightOuterAngle = spotlightOuterAngle;
	}
	
	Real LightRenderer::getSpotlightFalloff() const {
		return mSpotlightFalloff;
	}
	
	void LightRenderer::setSpotlightFalloff(Real spotlightFalloff) {
		mSpotlightFalloff = spotlightFalloff;
	}
	
	Real LightRenderer::getPowerScale() const {
		return mPowerScale;
	}
	
	void LightRenderer::setPowerScale(Real powerScale) {
		mPowerScale = powerScale;
	}
	
	Real LightRenderer::getFlashFrequency() const {
		return mFlashFrequency;
	}
	
	void LightRenderer::setFlashFrequency(Real flashFrequency) {
		mFlashFrequency = flashFrequency;
	}
	
	Real LightRenderer::getFlashLength() const {
		return mFlashLength;
	}
	
	void LightRenderer::setFlashLength(Real flashLength) {
		mFlashLength = flashLength;
	}
	
	bool LightRenderer::isFlashRandom() const {
		return mFlashRandom;
	}
	
	void LightRenderer::setFlashRandom(bool flashRandom) {
		mFlashRandom = flashRandom;
	}
	
	void LightRenderer::_makeNodesVisible(bool visible) {
		for (vector<LightRendererVisualData *>::const_iterator it = mAllVisualData.begin(); it != mAllVisualData.end(); ++it) {
			(*it)->node->setVisible(visible);
		}
	}
	
	void LightRenderer::_prepare(ParticleTechnique * technique) {
		if (!technique || mRendererInitialised) {
			return;
		}

		std::stringstream ss; 
		ss << this;
		mLightName = ss.str();
		mQuota = technique->getVisualParticleQuota();
		Ogre::SceneNode * parentNode = technique->getParentSystem()->getParentSceneNode();

		if (parentNode) {
			// Create number of VisualData objects including SceneNodes
			String sceneNodeName;
			for (size_t i = 0; i < mQuota; i++) {
				sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(i);
				LightRendererVisualData * visualData = PU_NEW_T(LightRendererVisualData, MEMCATEGORY_SCENE_OBJECTS)(parentNode->createChildSceneNode(sceneNodeName));

				mAllVisualData.push_back(visualData); // Managed by this renderer
				mVisualData.push_back(visualData); // Used to assign to a particle
			}

			// Create number of Lights
			size_t j;
			Ogre::Light * light;
			vector<LightRendererVisualData *>::const_iterator it;
			for (it = mAllVisualData.begin(), j = 0; it != mAllVisualData.end(); ++it, ++j) {
				light = technique->getParentSystem()->getSceneManager()->createLight(mLightName + StringConverter::toString(j));
				(*it)->node->attachObject(light);
				light->setType(mLightType);
				light->setAttenuation(mAttenuationRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadratic);
				light->setDiffuseColour(DEFAULT_DIFFUSE); // The light always gets the diffuse colour from the particle
				light->setSpecularColour(mSpecularColour);
				light->setSpotlightFalloff(mSpotlightFalloff);
				light->setSpotlightInnerAngle(mSpotlightInnerAngle);
				light->setSpotlightOuterAngle(mSpotlightOuterAngle);
				light->setPowerScale(mPowerScale);
				light->setRenderQueueGroup(mQueueId);
				light->setVisible(false);
				mLights.push_back(light);
				(*it)->light = light;
			}
		}

		_makeNodesVisible(false); // Make them invisibe, otherwise they light the scene before start
		mRendererInitialised = true;
	}
	
	void LightRenderer::_unprepare(ParticleTechnique * technique) {
		mRendererInitialised = false; // Retriggers the _prepare function
		_destroyAll(); // Delete all nodes, they will be rebuild
	}
	
//	void LightRenderer::_updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool)
//	{
//		ParticleRenderer::_updateRenderQueue(queue, pool);
//	}
	
	void LightRenderer::_processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
		if (!mVisible) {
			return;
		}

		VisualParticle * visualParticle = static_cast<VisualParticle *>(particle);
		LightRendererVisualData * visualData = static_cast<LightRendererVisualData *>(particle->visualData);

		// Check whether the particle has visual data
		if (!visualData && !mVisualData.empty()) {
			visualParticle->visualData = mVisualData.back();
			mVisualData.pop_back();
		}

		visualData = static_cast<LightRendererVisualData *>(particle->visualData);
		if (!visualData) {
			return;
		}

		// Update the node
		Ogre::SceneNode * node = visualData->node;
		if (!node) {
			return;
		}

		node->_setDerivedPosition(visualParticle->position);
		node->setDirection(visualParticle->direction, Ogre::Node::TS_WORLD); // Needed for direction of spotlight

		Ogre::Light * light = visualData->light;
		if (!light) {
			return;
		}

		// Update the light: The light gets the diffuse colour from the particle, so Colour Affectors etc. can be used.
		light->setDiffuseColour(visualParticle->colour.r, visualParticle->colour.g, visualParticle->colour.b);

		// Update the counters (if needed)
		if (mFlashFrequency > 0.0) {
			visualData->flashFrequencyCount += timeElapsed;
			if (visualData->flashFrequencyCount > mFlashFrequency) {
				visualData->flashFrequencyCount -= mFlashFrequency;

				// Update the light
				if (mFlashRandom) {
					if (Math::UnitRandom() > 0.5) {
						light->setVisible(!light->isVisible());
					}
				} else {
					light->setVisible(true);
					visualData->flashLengthCount = 0.0;
				}
			}
			if (!mFlashRandom && light->isVisible()) {
				visualData->flashLengthCount += timeElapsed;
				if (visualData->flashLengthCount > mFlashLength) {
					light->setVisible(false);
					visualData->flashLengthCount -= mFlashLength;
				}
			}
		} else {
			light->setVisible(true);
		}
	}
	
	void LightRenderer::_setMaterialName(const String & materialName) {
		// No implementation here, a light has no material
	}
	
	void LightRenderer::_notifyCurrentCamera(Camera * cam) {
		// No implementation here
	}
	
	void LightRenderer::_notifyAttached(Ogre::Node * parent, bool isTagPoint) {
		// No implementation here
	}
	
	void LightRenderer::_notifyParticleQuota(size_t quota) {
		mRendererInitialised = false; // Retriggers the _prepare function
		_destroyAll(); // Delete all nodes en entities, they will be rebuild
	}
	
	void LightRenderer::_notifyDefaultDimensions(Real width, Real height, Real depth) {
		// No implementation here, a light has no dimensions
	}
	
	void LightRenderer::_notifyParticleResized() {
		// No implementation here, a light cannot be resized
	}
	
	SortMode LightRenderer::_getSortMode() const {
		return SM_DISTANCE; // In fact there is no sorting
	}
	
	void LightRenderer::copyAttributesTo(ParticleRenderer * renderer) {
		// First copy parent attributes
		ParticleRenderer::copyAttributesTo(renderer);

		// First cast to LightRenderer
		LightRenderer * lightRenderer = static_cast<LightRenderer *>(renderer);

		// Copy attributes
		lightRenderer->mLightType = mLightType;
		lightRenderer->mSpecularColour = mSpecularColour;
		lightRenderer->mAttenuationRange = mAttenuationRange;
		lightRenderer->mAttenuationConstant = mAttenuationConstant;
		lightRenderer->mAttenuationLinear = mAttenuationLinear;
		lightRenderer->mAttenuationQuadratic = mAttenuationQuadratic;
		lightRenderer->mSpotlightInnerAngle = mSpotlightInnerAngle;

		lightRenderer->mSpotlightOuterAngle = mSpotlightOuterAngle;
		lightRenderer->mSpotlightFalloff = mSpotlightFalloff;
		lightRenderer->mPowerScale = mPowerScale;
		lightRenderer->mFlashFrequency = mFlashFrequency;
		lightRenderer->mFlashLength = mFlashLength;
		lightRenderer->mFlashRandom = mFlashRandom;
	}

} /* namespace ParticleUniverse */
