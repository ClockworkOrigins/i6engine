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
