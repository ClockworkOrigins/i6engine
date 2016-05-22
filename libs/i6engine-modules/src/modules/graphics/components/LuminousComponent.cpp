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

#include "i6engine/modules/graphics/components/LuminousComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreLight.h"
#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	LuminousComponent::LuminousComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter) : _manager(manager), _parent(parent), _sceneNode(nullptr) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		Ogre::SceneManager * sm = _manager->getSceneManager();

		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_sceneNode = _parent->getSceneNode()->createChildSceneNode(name, position.toOgre());
		Ogre::Light * light = sm->createLight(name);
		_sceneNode->attachObject(light);

		light->setType(Ogre::Light::LightTypes(type));
		light->setDiffuseColour(diffuse.getX(), diffuse.getY(), diffuse.getZ());
		light->setSpecularColour(specular.getX(), specular.getY(), specular.getZ());
		light->setAttenuation(attenuation.getW(), attenuation.getX(), attenuation.getY(), attenuation.getZ());
		light->setDirection(direction.toOgre());
		light->setSpotlightInnerAngle(Ogre::Radian(spotLightRangeInner));
		light->setSpotlightOuterAngle(Ogre::Radian(spotLightRangeOuter));
	}

	LuminousComponent::~LuminousComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::SceneManager * sm = _manager->getSceneManager();
		Ogre::Light * light = dynamic_cast<Ogre::Light *>(_sceneNode->getAttachedObject(0));

		_sceneNode->detachObject(light);
		sm->destroyLight(light);

		_parent->getSceneNode()->removeAndDestroyChild(_sceneNode->getName());
	}

	void LuminousComponent::updateLuminousComponent(const api::LuminousAppearanceComponent::LightType type, const Vec3 & diffuse, const Vec3 & specular, const Vec4 & attenuation, const Vec3 & direction, const Vec3 & position, double spotLightRangeInner, double spotLightRangeOuter) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Light * light = dynamic_cast<Ogre::Light *>(_sceneNode->getAttachedObject(0));

		_sceneNode->setPosition(position.toOgre());

		light->setType(Ogre::Light::LightTypes(type));
		light->setDiffuseColour(diffuse.getX(), diffuse.getY(), diffuse.getZ());
		light->setSpecularColour(specular.getX(), specular.getY(), specular.getZ());
		light->setAttenuation(attenuation.getW(), attenuation.getX(), attenuation.getY(), attenuation.getZ());
		light->setDirection(direction.toOgre());
		light->setSpotlightInnerAngle(Ogre::Radian(spotLightRangeInner * PI / 180));
		light->setSpotlightOuterAngle(Ogre::Radian(spotLightRangeOuter * PI / 180));
	}

} /* namespace modules */
} /* namespace i6e */
