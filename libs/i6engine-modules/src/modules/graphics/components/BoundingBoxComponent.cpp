/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "i6engine/modules/graphics/components/BoundingBoxComponent.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"
#include "i6engine/modules/graphics/components/MeshComponent.h"

#include "OGRE/OgreEntity.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreSceneManager.h"

namespace i6engine {
namespace modules {

	BoundingBoxComponent::BoundingBoxComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, MeshComponent * mesh, const Vec3 & colour) : _manager(manager), _parent(parent), _boundingBox(nullptr), _mesh(mesh), _id(coid) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();
		_boundingBox = sm->createManualObject("MO_" + std::to_string(goid));

		// NOTE: The second parameter to the create method is the resource group the material will be added to.
		// If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
		Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("MO_" + std::to_string(goid) + "_Material", "General");
		myManualObjectMaterial->setReceiveShadows(false);
		myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true);
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(colour.getX(), colour.getY(), colour.getZ(), 0);
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(colour.getX(), colour.getY(), colour.getZ());
		myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(colour.getX(), colour.getY(), colour.getZ());

		_boundingBox->begin("MO_" + std::to_string(goid) + "_Material", Ogre::RenderOperation::OT_LINE_LIST);

		Ogre::MovableObject * meshEntity = mesh->getEntity();

		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_BOTTOM));

		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_TOP));

		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::FAR_RIGHT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_LEFT_TOP));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_BOTTOM));
		_boundingBox->position(meshEntity->getBoundingBox().getCorner(Ogre::AxisAlignedBox::CornerEnum::NEAR_RIGHT_TOP));

		_boundingBox->end();
	}

	BoundingBoxComponent::~BoundingBoxComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mesh->removeBoundingBoxObserver(_id);
		_parent->getSceneNode()->detachObject(_boundingBox);
		Ogre::SceneManager * sm = _manager->getSceneManager();
		sm->destroyManualObject(_boundingBox);
		_boundingBox = nullptr;
	}

} /* namespace modules */
} /* namespace i6engine */
