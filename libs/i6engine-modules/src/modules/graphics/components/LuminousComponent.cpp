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

#include "i6engine/modules/graphics/components/LuminousComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreLight.h"
#include "OGRE/OgreSceneManager.h"

namespace i6engine {
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
} /* namespace i6engine */
