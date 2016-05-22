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

#include "Externs/ParticleUniverseSceneDecoratorExtern.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

#include "OgreEntity.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

namespace ParticleUniverse {
	
	SceneDecoratorExtern::SceneDecoratorExtern() : Extern(), mEntitySet(false), mEntity(nullptr), mMeshName(BLANK_STRING), mEntityName(BLANK_STRING), mMaterialName("BaseWhite"), mMaterialNameSet(false), mScale(Vector3::UNIT_SCALE), mPosition(Vector3::ZERO), mSubnode(nullptr), mCount(0) {
	}
	
	void SceneDecoratorExtern::_prepare(ParticleTechnique * technique) {
		if (mEntitySet) {
			if (!mParentTechnique->getParentSystem()) {
				return;
			}
			
			// Attach entity to a child scenenode. If the parent system is attached to a TagPoint, the entity cannot be attached.
			Ogre::SceneNode * sceneNode = mParentTechnique->getParentSystem()->getParentSceneNode();
			if (sceneNode) {
				std::stringstream ss; 
				ss << this;
				String sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(mCount++);
				mSubnode = sceneNode->createChildSceneNode(sceneNodeName);
				mSubnode->setScale(mScale);
				mSubnode->setPosition(mPosition);
				if (!mEntity) {
					createEntity();
					mSubnode->attachObject(mEntity);
				}
			}
		}
	}
	
	void SceneDecoratorExtern::_unprepare(ParticleTechnique * technique) {
		// Todo
	}
	
	void SceneDecoratorExtern::_interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed) {
		// Nothing to do yet
	}
	
	void SceneDecoratorExtern::_notifyStart() {
		// Scale the node
		if (!mSubnode && mParentTechnique->getParentSystem()) {
			std::stringstream ss; 
			ss << this;
			String sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(mCount++);
			Ogre::SceneNode * sceneNode = mParentTechnique->getParentSystem()->getParentSceneNode();
			mSubnode = sceneNode->createChildSceneNode(sceneNodeName);
		}

		if (!mEntity) {
			createEntity();
			mSubnode->attachObject(mEntity);
		}

		mSubnode->setScale(mScale);
		mSubnode->setPosition(mPosition);
	}
	
	void SceneDecoratorExtern::_notifyStop() {
		if (mSubnode && mParentTechnique->getParentSystem()) {
			Ogre::SceneNode * sceneNode = mParentTechnique->getParentSystem()->getParentSceneNode();
			Ogre::SceneNode::ChildNodeIterator it = sceneNode->getChildIterator();
			unsigned short i = 0;
			while (it.hasMoreElements()) { 
				Ogre::SceneNode * child = static_cast<Ogre::SceneNode *>(it.getNext());
				if (child == mSubnode) {
					mSubnode->detachAllObjects();
					sceneNode->removeAndDestroyChild(i);
					mSubnode = nullptr;
				}
				++i;
			}

			destroyEntity();
		}
	}
	
	void SceneDecoratorExtern::createEntity() {
		if (!mEntity) {
			Ogre::SceneManager * sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
			if (sceneManager) {
				std::stringstream ss;
				ss << this;
				mEntityName = mMeshName + ss.str();
				mEntity = sceneManager->createEntity(mEntityName, mMeshName);
				if (mMaterialNameSet) {
					mEntity->setMaterialName(mMaterialName);
				}
			}
		}
	}
	
	void SceneDecoratorExtern::destroyEntity() {
		if (mEntity) {
			Ogre::SceneManager * sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
			if (sceneManager) {
				sceneManager->destroyEntity(mEntityName);
				mEntity = nullptr;
				mEntityName = BLANK_STRING;
			}
		}
	}
	
	const String & SceneDecoratorExtern::getMeshName() const {
		return mMeshName;
	}
	
	void SceneDecoratorExtern::setMeshName(const String & meshName) {
		mMeshName = meshName;
		mEntitySet = true;
	}
	
	const String & SceneDecoratorExtern::getMaterialName() const {
		return mMaterialName;
	}
	
	void SceneDecoratorExtern::setMaterialName(const String & materialName) {
		mMaterialName = materialName;
		mMaterialNameSet = true;
	}
	
	const Vector3 & SceneDecoratorExtern::getScale() const {
		return mScale;
	}
	
	void SceneDecoratorExtern::setScale(Vector3 & scale) {
		mScale = scale;
	}
	
	const Vector3 & SceneDecoratorExtern::getPosition() const {
		return mPosition;
	}
	
	void SceneDecoratorExtern::setPosition(Vector3 & position) {
		mPosition = position;
	}
	
	void SceneDecoratorExtern::copyAttributesTo(Extern * externObject) {
		Extern::copyAttributesTo(externObject);
		SceneDecoratorExtern * sceneDecoratorExtern = static_cast<SceneDecoratorExtern *>(externObject);
		sceneDecoratorExtern->setMaterialName(mMaterialName);
		sceneDecoratorExtern->setMeshName(mMeshName);
		sceneDecoratorExtern->mScale = mScale;
		sceneDecoratorExtern->mPosition = mPosition;
	}

} /* namespace ParticleUniverse */
