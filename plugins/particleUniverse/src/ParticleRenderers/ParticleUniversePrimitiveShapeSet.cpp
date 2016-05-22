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

#include "ParticleRenderers/ParticleUniversePrimitiveShapeSet.h"

#include "ParticleUniverseException.h"
#include "ParticleUniverseSystemManager.h"

#include "OgreMaterialManager.h"
#include "OgreNode.h"
#include "OgreSceneManager.h"
#include "OgreTechnique.h"

namespace ParticleUniverse {
	
	PrimitiveShapeSet::PrimitiveShapeSet() : mBoundingRadius(0.0), mCullIndividual(false), mWorldSpace(false), mZRotated(false), mAllDefaultSize(true) {
		setMaterialName("BaseWhite");
		mCastShadows = false;
	}
	
	PrimitiveShapeSet::PrimitiveShapeSet(const String & name, unsigned int poolSize, bool externalData) : MovableObject(name), mBoundingRadius(0.0), mCullIndividual(false), mWorldSpace(false), mZRotated(false), mAllDefaultSize(true) {
		setMaterialName("BaseWhite");
		mCastShadows = false;
	}
	
	PrimitiveShapeSet::~PrimitiveShapeSet() {
	}
	
	void PrimitiveShapeSet::setMaterialName(const String & name) {
		mMaterialName = name;

		if (ParticleSystemManager::getSingletonPtr()->isAutoLoadMaterials()) {
			mpMaterial = Ogre::MaterialManager::getSingleton().getByName(name);

			if (mpMaterial.isNull()) {
				EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + name, "PrimitiveShapeSet::setMaterialName");
			}
	
			/** Load material if not already loaded
			*/
			mpMaterial->load();
		}
	}
	
	const String & PrimitiveShapeSet::getMaterialName() const {
		return mMaterialName;
	}
	
	void PrimitiveShapeSet::_notifyCurrentCamera(Camera * cam) {
		Ogre::MovableObject::_notifyCurrentCamera(cam);
		mCurrentCamera = cam;
    }
	
	const AxisAlignedBox & PrimitiveShapeSet::getBoundingBox() const {
		return mAABB;
	}
	
	const Ogre::MaterialPtr & PrimitiveShapeSet::getMaterial() const {
		return mpMaterial;
	}
	
	void PrimitiveShapeSet::_notifyResized() {
		mAllDefaultSize = false;
	}
	
	void PrimitiveShapeSet::getWorldTransforms(Ogre::Matrix4 * xform) const {
		if (mWorldSpace) {
			*xform = Ogre::Matrix4::IDENTITY;
		} else {
			*xform = _getParentNodeFullTransform();
		}
	}
	
    const Quaternion & PrimitiveShapeSet::getWorldOrientation() const {
		return mParentNode->_getDerivedOrientation();
	}
	
	const Vector3 & PrimitiveShapeSet::getWorldPosition() const {
		return mParentNode->_getDerivedPosition();
	}
	
	void PrimitiveShapeSet::setZRotated(bool zRotated) {
		mZRotated = zRotated;
	}
	
	bool PrimitiveShapeSet::isZRotated() const {
		return mZRotated;
	}
	
	void PrimitiveShapeSet::_notifyZRotated() {
		mZRotated = true;
	}
	
	void PrimitiveShapeSet::rotateTexture(Real speed) {
		// Get the material and rotate it, assume the material is loaded already, otherwise skip.
		Ogre::MaterialPtr material = getMaterial();
		if (material.isNull()) {
			return;
		}

		Ogre::TextureUnitState::EffectMap::const_iterator it;
		unsigned short numberOfTechniques = material->getNumTechniques();
		for (unsigned short u = 0; u < numberOfTechniques; ++u) {
			Ogre::Technique * technique = material->getTechnique(u);
			unsigned short numberOfPasses = technique->getNumPasses();
			for (unsigned short v = 0; v < numberOfPasses; ++v) {
				Ogre::Pass * pass = technique->getPass(v);
				unsigned short numberOfTextureUnitStates = pass->getNumTextureUnitStates();
				for (unsigned short w = 0; w < numberOfTextureUnitStates; ++w) {
					// Set the rotation if not already available.
					// This can only be done once! Changing the rotationspeed or removing the rotation
					// and resetting it doesn´t seem to work.
					Ogre::TextureUnitState * textureUnitState = pass->getTextureUnitState(w);
					it = textureUnitState->getEffects().find(Ogre::TextureUnitState::ET_ROTATE);
					if (it == textureUnitState->getEffects().end()) {
						textureUnitState->setRotateAnimation(speed);
					}
				}
			}
		}
	}
	
	bool PrimitiveShapeSet::isCullIndividually() const {
		return mCullIndividual;
	}
	
	void PrimitiveShapeSet::setCullIndividually(bool cullIndividual) {
		mCullIndividual = cullIndividual;
	}
	
	uint32 PrimitiveShapeSet::getTypeFlags() const {
		return Ogre::SceneManager::FX_TYPE_MASK;
	}
	
	Real PrimitiveShapeSet::getSquaredViewDepth(const Camera * const cam) const {
		assert(mParentNode);
		return mParentNode->getSquaredViewDepth(cam);
	}
	
	Real PrimitiveShapeSet::getBoundingRadius() const {
		return mBoundingRadius;
	}
	
	const Ogre::LightList& PrimitiveShapeSet::getLights() const {
		return queryLights();
	}

} /* namespace ParticleUniverse */
