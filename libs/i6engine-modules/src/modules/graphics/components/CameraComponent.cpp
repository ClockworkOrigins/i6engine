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

#include "i6engine/modules/graphics/components/CameraComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreCompositorManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	CameraComponent::CameraComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov) : _manager(manager), _parent(parent), _sceneNode(nullptr), _zOrder(INT_MAX) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name, position.toOgre());
		Ogre::Camera * camera = sm->createCamera("camera" + std::to_string(goid) + "_" + std::to_string(coid));
		_sceneNode->attachObject(camera);
		camera->setPosition(position.toOgre());
		camera->lookAt(_parent->getSceneNode()->getPosition() + math::rotateVector(lookAt, Quaternion(_parent->getSceneNode()->getOrientation())).toOgre());
		camera->setNearClipDistance(nC);
		camera->setFOVy(Ogre::Radian(fov));
		camera->setAspectRatio(aspect);
	}

	CameraComponent::~CameraComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::SceneManager * sm = _manager->getSceneManager();
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(_sceneNode->getAttachedObject(0));
		Ogre::Viewport * vp = camera->getViewport();
		vp->setCamera(nullptr); // ISIXE-1765

		Ogre::CompositorManager::ResourceMapIterator resourceIterator = Ogre::CompositorManager::getSingleton().getResourceIterator();

		// add all compositor resources to the view container
		while (resourceIterator.hasMoreElements()) {
			Ogre::ResourcePtr resource = resourceIterator.getNext();
			const Ogre::String & compositorName = resource->getName();
			try {
				Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName, false);
				Ogre::CompositorManager::getSingleton().removeCompositor(vp, compositorName);
			} catch (...) {
				/// Warn user
				Ogre::LogManager::getSingleton().logMessage("Could not load compositor " + compositorName);
			}
		}

		_manager->getRenderWindow()->removeViewport(vp->getZOrder());

		_sceneNode->detachObject(camera);
		sm->destroyCamera(camera);

		_parent->getSceneNode()->removeAndDestroyChild(_sceneNode->getName());
	}

	void CameraComponent::updateCameraComponent(const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(_sceneNode->getAttachedObject(0));
		camera->setPosition(position.toOgre());
		camera->lookAt(lookAt.toOgre());
		camera->setNearClipDistance(nC);
		camera->setFOVy(Ogre::Radian(fov));
		camera->setAspectRatio(aspect);
	}

	void CameraComponent::updateCameraFrustumComponent(const double left, const double right, const double top, const double bottom) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(_sceneNode->getAttachedObject(0));
		camera->setFrustumExtents(left, right, top, bottom);
	}

	void CameraComponent::enableCompositor(const std::string & compositor, bool enabled) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(_sceneNode->getAttachedObject(0));
		Ogre::Viewport * vp = camera->getViewport();
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositor, enabled);
	}

	void CameraComponent::createOrUpdateViewport(int zOrder, const double left, const double top, const double width, const double height, const double red, const double green, const double blue, const double alpha) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Viewport * vp = nullptr;
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(_sceneNode->getAttachedObject(0));
		if (camera->getViewport() == nullptr) {
			_zOrder = zOrder;
			vp = _manager->getRenderWindow()->addViewport(camera, zOrder, float(left), float(top), float(width), float(height));
		} else {
			vp = camera->getViewport();
			vp->setDimensions(left, top, width, height);
		}

		vp->setBackgroundColour(Ogre::ColourValue(float(red), float(green), float(blue), float(alpha)));

		Ogre::CompositorManager::ResourceMapIterator resourceIterator = Ogre::CompositorManager::getSingleton().getResourceIterator();

		// add all compositor resources to the view container
		while (resourceIterator.hasMoreElements()) {
			Ogre::ResourcePtr resource = resourceIterator.getNext();
			const Ogre::String & compositorName = resource->getName();

			int addPosition = -1;
			if (compositorName == "HDR") {
				// HDR must be first in the chain
				addPosition = 0;
			}
			try {
				Ogre::CompositorManager::getSingleton().addCompositor(vp, compositorName, addPosition);
				Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName, false);
			} catch (...) {
				/// Warn user
				Ogre::LogManager::getSingleton().logMessage("Could not load compositor " + compositorName);
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
