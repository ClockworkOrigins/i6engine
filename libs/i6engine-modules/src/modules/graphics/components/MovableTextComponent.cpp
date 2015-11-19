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

namespace i6engine {
namespace modules {

	MovableTextComponent::MovableTextComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, MeshComponent * mesh, const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour) : _manager(manager), _parent(parent), _movableText(nullptr), _mesh(mesh), _id(coid) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		try {
			_movableText = new MovableText(mesh->getEntity(), _manager->getSceneManager(), font);
			_movableText->setText(text);
			_movableText->setSize(size);
			_movableText->setColour(colour);
			_movableText->enable(true);
			_parent->addTicker(this);
			mesh->addMovableTextObserver(coid);
		} catch (Ogre::Exception & e) {
			std::cout << e.what() << std::endl;
		}
	}

	MovableTextComponent::~MovableTextComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		delete _movableText;
		_parent->removeTicker(this);
		_mesh->removeMovableTextObserver(_id);
	}

	void MovableTextComponent::updateMovableText(const std::string & font, const std::string & text, uint16_t size, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_movableText->setText(text);
		_movableText->setSize(size);
		_movableText->setColour(colour);
	}

	void MovableTextComponent::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_movableText->update();
	}

} /* namespace modules */
} /* namespace i6engine */
