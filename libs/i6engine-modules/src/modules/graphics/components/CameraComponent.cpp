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

#include "i6engine/modules/graphics/components/CameraComponent.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreCompositorManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreSceneManager.h"

namespace i6engine {
namespace modules {

	CameraComponent::CameraComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const Vec3 & position, const Vec3 & lookAt, const double nC, double aspect, const double fov) : _manager(manager), _parent(parent), _sceneNode(nullptr), _zOrder(INT_MAX) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name, position.toOgre());
		Ogre::Camera * camera = sm->createCamera("camera" + std::to_string(goid) + "_" + std::to_string(coid));
		_sceneNode->attachObject(camera);
		camera->setPosition(position.toOgre());
		camera->lookAt(lookAt.toOgre());
		camera->setNearClipDistance(nC);
		camera->setFOVy(Ogre::Radian(fov));
		camera->setAspectRatio(aspect);
	}

	CameraComponent::~CameraComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::SceneManager * sm = _manager->getSceneManager();
		Ogre::Camera * camera = dynamic_cast<Ogre::Camera *>(_sceneNode->getAttachedObject(0));
		Ogre::Viewport * vp = camera->getViewport();

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

		if (_zOrder != INT_MAX) {
			_manager->getRenderWindow()->removeViewport(_zOrder);
		}

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
} /* namespace i6engine */
