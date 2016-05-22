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

#include "i6engine/modules/graphics/components/MovableTextComponent.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"
#include "i6engine/modules/graphics/components/MeshComponent.h"
#include "i6engine/modules/graphics/graphicswidgets/MovableText.h"

#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	MovableTextComponent::MovableTextComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & font, const std::string & text, double size, const Vec3 & colour) : _manager(manager), _parent(parent), _movableText(nullptr), _id(coid), _size(size), _autoScaleCallback() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		try {
			_movableText = new MovableText("MTC_" + std::to_string(goid) + "_" + std::to_string(coid), text, font, size, Ogre::ColourValue(colour.getX(), colour.getY(), colour.getZ(), 1.0f));
			_parent->getSceneNode()->attachObject(_movableText);
		} catch (Ogre::Exception & e) {
			ISIXE_LOG_WARN("MovableTextComponent", e.what());
		}
	}

	MovableTextComponent::~MovableTextComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_parent->getSceneNode()->detachObject(_movableText);
		delete _movableText;
		if (_autoScaleCallback != nullptr) {
			_parent->removeTicker(this);
		}
	}

	void MovableTextComponent::updateMovableText(const std::string & font, const std::string & text, double size, const Vec3 & colour) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_movableText->setCaption(text);
		_movableText->setCharacterHeight(size);
		_movableText->setColor(Ogre::ColourValue(colour.getX(), colour.getY(), colour.getZ(), 1.0f));
		_size = size;
	}

	void MovableTextComponent::setAutoScaleCallback(const std::function<double(const Vec3 &, const Vec3 &)> & callback) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_autoScaleCallback == nullptr && callback != nullptr) {
			_parent->addTicker(this);
		} else if (callback == nullptr && _autoScaleCallback != nullptr) {
			_parent->removeTicker(this);
		}
		_autoScaleCallback = callback;
	}

	void MovableTextComponent::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Vector3 cameraPos = _manager->getRenderWindow()->getViewport(0)->getCamera()->getDerivedPosition();
		Ogre::Vector3 ownPos = _parent->getSceneNode()->getPosition();
		_movableText->setCharacterHeight(_autoScaleCallback(Vec3(cameraPos), Vec3(ownPos)));
	}

} /* namespace modules */
} /* namespace i6e */
