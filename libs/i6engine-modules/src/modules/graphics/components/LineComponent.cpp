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

#include "i6engine/modules/graphics/components/LineComponent.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	LineComponent::LineComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const Vec3 & startPos, const Vec3 & endPos, const Vec3 & colour) : _manager(manager), _parent(parent), _sceneNode(nullptr), _line(nullptr), _material() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name);

		_line = sm->createManualObject("MO_" + std::to_string(goid) + "_" + std::to_string(coid));

		// NOTE: The second parameter to the create method is the resource group the material will be added to.
		// If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
		_material = Ogre::MaterialManager::getSingleton().create("MO_" + std::to_string(goid) + "_" + std::to_string(coid) + "_Material", "General");
		_material->setReceiveShadows(false);
		_material->getTechnique(0)->setLightingEnabled(true);
		_material->getTechnique(0)->getPass(0)->setDiffuse(colour.getX(), colour.getY(), colour.getZ(), 0);
		_material->getTechnique(0)->getPass(0)->setAmbient(colour.getX(), colour.getY(), colour.getZ());
		_material->getTechnique(0)->getPass(0)->setSelfIllumination(colour.getX(), colour.getY(), colour.getZ());


		_line->begin("MO_" + std::to_string(goid) + "_" + std::to_string(coid) + "_Material", Ogre::RenderOperation::OT_LINE_LIST);

		_line->position(startPos.toOgre());
		_line->position(endPos.toOgre());

		_line->end();

		_sceneNode->attachObject(_line);
	}

	LineComponent::~LineComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_sceneNode->detachObject(_line);

		_parent->getSceneNode()->removeAndDestroyChild(_sceneNode->getName());
		Ogre::SceneManager * sm = _manager->getSceneManager();
		sm->destroyManualObject(_line);
	}

} /* namespace modules */
} /* namespace i6e */
