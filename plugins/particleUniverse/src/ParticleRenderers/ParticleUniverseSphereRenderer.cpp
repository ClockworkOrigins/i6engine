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

#include "ParticleRenderers/ParticleUniverseSphereRenderer.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

#include "ParticleRenderers/ParticleUniverseSphere.h"
#include "ParticleRenderers/ParticleUniverseSphereSet.h"

namespace ParticleUniverse {
	
	SphereRenderer::SphereRenderer() : ParticleRenderer() {
		// Create sphere set
		mSphereSet = PU_NEW SphereSet("", 0, true);

		// World-relative axes
		mSphereSet->setSpheresInWorldSpace(true);

		autoRotate = false;
	}
	
	SphereRenderer::~SphereRenderer() {
		if (mSphereSet) {
			PU_DELETE mSphereSet;
			mSphereSet = nullptr;
		}
	}
	
	void SphereRenderer::_prepare(ParticleTechnique * technique) {
		// Use the given technique, although it should be the same as mParentTechnique (must be set already)
		if (!technique || mRendererInitialised) {
			return;
		}

		_notifyParticleQuota(technique->getVisualParticleQuota());

		// Notify attached, but only if the parentnode exists
		if (technique->getParentSystem()->getParentNode()) {		
			_notifyAttached(technique->getParentSystem()->getParentNode(), technique->getParentSystem()->isParentIsTagPoint());
		}
		_notifyDefaultDimensions(_mRendererScale.x * technique->getDefaultWidth(), _mRendererScale.y * technique->getDefaultHeight(), _mRendererScale.z * technique->getDefaultDepth());
		_setMaterialName(technique->getMaterialName());
		mSphereSet->setRenderQueueGroup(mQueueId);
		mRendererInitialised = true;
	}
	
	void SphereRenderer::_unprepare(ParticleTechnique * technique) {
		_notifyAttached(nullptr); // Bugfix V1.5: If detached from scenenode, do not use the pointer to it
	}
	
	void SphereRenderer::_updateRenderQueue(Ogre::RenderQueue * queue, ParticlePool * pool) {
		// Always perform this one
		ParticleRenderer::_updateRenderQueue(queue, pool);

		if (!mVisible) {
			return;
		}

		// Fast check to determine whether there are visual particles
		if (pool->isEmpty(Particle::PT_VISUAL)) {
			return;
		}

		mSphereSet->setCullIndividually(mCullIndividual);

		mSphereSet->beginSpheres(pool->getSize(Particle::PT_VISUAL));
		Sphere sphere;
		
		VisualParticle * particle = static_cast<VisualParticle *>(pool->getFirst(Particle::PT_VISUAL));

		// Activate texture rotation if needed and use the speed of the first particle.
		// The implementation of the texture rotation is low-profile, because its value is limited. For 3d
		// particles the geometry rotation is more interesting.
		if (mSphereSet->isZRotated()) {
			mSphereSet->rotateTexture(particle->zRotationSpeed.valueRadians());
		}

		// Loop through the particles
		while (!pool->end(Particle::PT_VISUAL)) {
			if (particle) {
				sphere.mPosition = particle->position;
				sphere.mColour = particle->colour;
				sphere.mOrientation = particle->orientation;

				if (sphere.mOwnRadius == particle->ownDimensions) {
					sphere.mOwnRadius = true;
					sphere.setRadius (0.5 * particle->width);
				}
					
				mSphereSet->injectSphere(sphere);
			}
			
			particle = static_cast<VisualParticle *>(pool->getNext(Particle::PT_VISUAL));
		}

        mSphereSet->endSpheres();

		// Update the queue
		mSphereSet->_updateRenderQueue(queue);
	}
	
	void SphereRenderer::_notifyAttached(Ogre::Node * parent, bool isTagPoint) {
		mSphereSet->_notifyAttached(parent, isTagPoint);
	}
	
	void SphereRenderer::_setMaterialName(const String & materialName) {
		mSphereSet->setMaterialName(materialName);
	}
	
	void SphereRenderer::_notifyCurrentCamera(Camera * cam) {
		mSphereSet->_notifyCurrentCamera(cam);
	}
	
	void SphereRenderer::_notifyParticleQuota(size_t quota) {
		mSphereSet->setPoolSize(static_cast<unsigned int>(quota));
	}
	
	void SphereRenderer::_notifyDefaultDimensions(Real width, Real height, Real depth) {
		mSphereSet->setDefaultRadius(0.5 * width);
	}
	
	void SphereRenderer::_notifyParticleResized() {	
		mSphereSet->_notifyResized();
	}
	
	void SphereRenderer::_notifyParticleZRotated() {
		mSphereSet->_notifyZRotated();
	}
	
	void SphereRenderer::setRenderQueueGroup(uint8 queueId) {
		mQueueId = queueId;
		if (mSphereSet) {
			mSphereSet->setRenderQueueGroup(queueId);
		}
	}
	
	void SphereRenderer::setVisible(bool visible) {
		ParticleRenderer::setVisible(visible);
		mSphereSet->setVisible(visible);
	}
	
	void SphereRenderer::copyAttributesTo(ParticleRenderer * renderer) {
		// First copy parent attributes
		ParticleRenderer::copyAttributesTo(renderer);

		// First cast to SphereRenderer
		SphereRenderer * sphereRenderer = static_cast<SphereRenderer *>(renderer);

		// Copy attributes in case there is a SphereSet (which should be available)
		if (!sphereRenderer->getSphereSet()) {
			return;
		}
	}

} /* namespace ParticleUniverse */
