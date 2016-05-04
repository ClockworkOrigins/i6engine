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

#include "i6engine/modules/graphics/components/MovableTextComponent.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"
#include "i6engine/modules/graphics/components/MeshComponent.h"
#include "i6engine/modules/graphics/graphicswidgets/MovableText.h"

#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	MovableTextComponent::MovableTextComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & font, const std::string & text, double size, const Vec3 & colour) : _manager(manager), _parent(parent), _movableText(nullptr), _id(coid) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		try {
			_movableText = new MovableText("MTC_" + std::to_string(goid) + "_" + std::to_string(coid), text, font, size, Ogre::ColourValue(colour.getX(), colour.getY(), colour.getZ(), 1.0f));
			_parent->getSceneNode()->attachObject(_movableText);
		} catch (Ogre::Exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	MovableTextComponent::~MovableTextComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_parent->getSceneNode()->detachObject(_movableText);
		delete _movableText;
	}

	void MovableTextComponent::updateMovableText(const std::string & font, const std::string & text, double size, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_movableText->setCaption(text);
		_movableText->setCharacterHeight(size);
		_movableText->setColor(Ogre::ColourValue(colour.getX(), colour.getY(), colour.getZ(), 1.0f));
	}

} /* namespace modules */
} /* namespace i6e */
