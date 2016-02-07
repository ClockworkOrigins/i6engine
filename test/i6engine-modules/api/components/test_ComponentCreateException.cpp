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

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/components/AnimatedDirectionalLightComponent.h"
#include "i6engine/api/components/AnimatedLuminousAppearanceComponent.h"
#include "i6engine/api/components/AnimatedSpotLightComponent.h"
#include "i6engine/api/components/AnimationControllerComponent.h"
#include "i6engine/api/components/BillboardComponent.h"
#include "i6engine/api/components/CameraComponent.h"
#include "i6engine/api/components/FollowComponent.h"
#include "i6engine/api/components/LifetimeComponent.h"
#include "i6engine/api/components/LuminousAppearanceComponent.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/components/MovableTextComponent.h"
#include "i6engine/api/components/MoverCircleComponent.h"
#include "i6engine/api/components/MoverInterpolateComponent.h"
#include "i6engine/api/components/MovingCameraComponent.h"
#include "i6engine/api/components/ParticleEmitterComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/Point2PointConstraintComponent.h"
#include "i6engine/api/components/SoundComponent.h"
#include "i6engine/api/components/SoundListenerComponent.h"
#include "i6engine/api/components/SpawnpointComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/TerrainAppearanceComponent.h"
#include "i6engine/api/components/VelocityComponent.h"
#include "i6engine/api/components/WaypointComponent.h"

#include "gtest/gtest.h"

using namespace i6engine;
using namespace i6engine::api;

TEST(Component, CreateException) {
	{
		attributeMap paramsAnimatedDirectionalLight = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" } };
		Component::createC<AnimatedDirectionalLightComponent>(0, paramsAnimatedDirectionalLight);
		for (auto it = paramsAnimatedDirectionalLight.begin(); it != paramsAnimatedDirectionalLight.end(); it++) {
			auto newMap = paramsAnimatedDirectionalLight;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<AnimatedDirectionalLightComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsAnimatedLuminousAppearance = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" } };
		Component::createC<AnimatedLuminousAppearanceComponent>(0, paramsAnimatedLuminousAppearance);
		for (auto it = paramsAnimatedLuminousAppearance.begin(); it != paramsAnimatedLuminousAppearance.end(); it++) {
			auto newMap = paramsAnimatedLuminousAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<AnimatedLuminousAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsAnimatedLuminousAppearance = { { "lightType", "1" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" } };
		Component::createC<AnimatedLuminousAppearanceComponent>(0, paramsAnimatedLuminousAppearance);
		for (auto it = paramsAnimatedLuminousAppearance.begin(); it != paramsAnimatedLuminousAppearance.end(); it++) {
			auto newMap = paramsAnimatedLuminousAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<AnimatedLuminousAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsAnimatedLuminousAppearance = { { "lightType", "2" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		Component::createC<AnimatedLuminousAppearanceComponent>(0, paramsAnimatedLuminousAppearance);
		for (auto it = paramsAnimatedLuminousAppearance.begin(); it != paramsAnimatedLuminousAppearance.end(); it++) {
			auto newMap = paramsAnimatedLuminousAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<AnimatedLuminousAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsAnimatedSpotLight = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" } };
		Component::createC<AnimatedSpotLightComponent>(0, paramsAnimatedSpotLight);
		for (auto it = paramsAnimatedSpotLight.begin(); it != paramsAnimatedSpotLight.end(); it++) {
			auto newMap = paramsAnimatedSpotLight;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<AnimatedSpotLightComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsAnimationController = { };
		Component::createC<AnimationControllerComponent>(0, paramsAnimationController);
		for (auto it = paramsAnimationController.begin(); it != paramsAnimationController.end(); it++) {
			auto newMap = paramsAnimationController;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<AnimationControllerComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsBillboard = { { "material", "blafoo" }, { "width", "0.1" }, { "height", "0.1" }, { "origin", "0" } };
		Component::createC<BillboardComponent>(0, paramsBillboard);
		for (auto it = paramsBillboard.begin(); it != paramsBillboard.end(); it++) {
			auto newMap = paramsBillboard;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<BillboardComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsCamera = { { "pos", "0.0 0.0 0.0" }, { "lookAt", "0.0 0.0 0.0" }, { "nearclip", "1" } };
		Component::createC<CameraComponent>(0, paramsCamera);
		for (auto it = paramsCamera.begin(); it != paramsCamera.end(); it++) {
			auto newMap = paramsCamera;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<CameraComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsFollow = { { "targetID", "-1" }, { "speed", "1.0" } };
		Component::createC<FollowComponent>(0, paramsFollow);
		for (auto it = paramsFollow.begin(); it != paramsFollow.end(); it++) {
			auto newMap = paramsFollow;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<FollowComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsLifetime = { { "lifetime", "0" } };
		Component::createC<LifetimeComponent>(0, paramsLifetime);
		for (auto it = paramsLifetime.begin(); it != paramsLifetime.end(); it++) {
			auto newMap = paramsLifetime;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<LifetimeComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" } };
		Component::createC<LuminousAppearanceComponent>(0, paramsLuminousAppearance);
		for (auto it = paramsLuminousAppearance.begin(); it != paramsLuminousAppearance.end(); it++) {
			auto newMap = paramsLuminousAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<LuminousAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "1" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" } };
		Component::createC<LuminousAppearanceComponent>(0, paramsLuminousAppearance);
		for (auto it = paramsLuminousAppearance.begin(); it != paramsLuminousAppearance.end(); it++) {
			auto newMap = paramsLuminousAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<LuminousAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "2" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		Component::createC<LuminousAppearanceComponent>(0, paramsLuminousAppearance);
		for (auto it = paramsLuminousAppearance.begin(); it != paramsLuminousAppearance.end(); it++) {
			auto newMap = paramsLuminousAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<LuminousAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsMeshAppearance = { { "mesh", "blafoo.mesh" }, { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" }, { "visibility", "1" } };
		Component::createC<MeshAppearanceComponent>(0, paramsMeshAppearance);
		for (auto it = paramsMeshAppearance.begin(); it != paramsMeshAppearance.end(); it++) {
			auto newMap = paramsMeshAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<MeshAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsMovableText = { { "font", "blafoo" }, { "text", "text" }, { "size", "10" }, { "colour", "0.0 0.0 0.0" } };
		Component::createC<MovableTextComponent>(0, paramsMovableText);
		for (auto it = paramsMovableText.begin(); it != paramsMovableText.end(); it++) {
			auto newMap = paramsMovableText;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<MovableTextComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsMoverCircle = { { "axis", "0.0 1.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "duration", "1" }, { "radius", "1.0" }, { "positioning", "0" }, { "started", "0" } };
		Component::createC<MoverCircleComponent>(0, paramsMoverCircle);
		for (auto it = paramsMoverCircle.begin(); it != paramsMoverCircle.end(); it++) {
			auto newMap = paramsMoverCircle;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<MoverCircleComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsMoverInterpolate = { { "direction", "1" }, { "way", "0" }, { "duration", "1" }, { "mode", "0" }, { "positioning", "0" }, { "started", "0" }, { "keyframes", "1" }, { "keyframe_0_pos", "0.0 0.0 0.0" }, { "keyframe_0_rot", "0.0 0.0 0.0 0.0" } };
		Component::createC<MoverInterpolateComponent>(0, paramsMoverInterpolate);
		for (auto it = paramsMoverInterpolate.begin(); it != paramsMoverInterpolate.end(); it++) {
			auto newMap = paramsMoverInterpolate;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<MoverInterpolateComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsMovingCamera = { { "pos", "0.0 0.0 0.0" }, { "lookAt", "0.0 0.0 0.0" }, { "nearclip", "1" } };
		Component::createC<MovingCameraComponent>(0, paramsMovingCamera);
		for (auto it = paramsMovingCamera.begin(); it != paramsMovingCamera.end(); it++) {
			auto newMap = paramsMovingCamera;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<MovingCameraComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsParticleEmitter = { { "particleEmitter", "blafoo.particle" } };
		Component::createC<ParticleEmitterComponent>(0, paramsParticleEmitter);
		for (auto it = paramsParticleEmitter.begin(); it != paramsParticleEmitter.end(); it++) {
			auto newMap = paramsParticleEmitter;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<ParticleEmitterComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsPhysicalState = { { "shapeType", "0" }, { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" }, { "collisionGroup", "0 0 0" }, { "shatterInterest", "0" }, { "compound", "0" } };
		Component::createC<PhysicalStateComponent>(0, paramsPhysicalState);
		for (auto it = paramsPhysicalState.begin(); it != paramsPhysicalState.end(); it++) {
			auto newMap = paramsPhysicalState;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<PhysicalStateComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsPoint2PointConstraint = { { "selfIdentifier", "a" }, { "targetIdentifier", "b" }, { "selfOffset", "0.0 0.0 0.0" }, { "targetOffset", "0.0 0.0 0.0" } };
		Component::createC<Point2PointConstraintComponent>(0, paramsPoint2PointConstraint);
		for (auto it = paramsPoint2PointConstraint.begin(); it != paramsPoint2PointConstraint.end(); it++) {
			auto newMap = paramsPoint2PointConstraint;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<Point2PointConstraintComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsSound = { { "file", "blafoo" }, { "direction", "0.0 0.0 0.0" }, { "offset", "0.0 0.0 0.0" }, { "looping", "0" }, { "maxDist", "10" }, { "cache", "0" } };
		Component::createC<SoundComponent>(0, paramsSound);
		for (auto it = paramsSound.begin(); it != paramsSound.end(); it++) {
			auto newMap = paramsSound;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<SoundComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsSoundListener = { };
		Component::createC<SoundListenerComponent>(0, paramsSoundListener);
		for (auto it = paramsSoundListener.begin(); it != paramsSoundListener.end(); it++) {
			auto newMap = paramsSoundListener;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<SoundListenerComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsSpawnpoint = { { "spawntypes", "blafoo" } };
		Component::createC<SpawnpointComponent>(0, paramsSpawnpoint);
		for (auto it = paramsSpawnpoint.begin(); it != paramsSpawnpoint.end(); it++) {
			auto newMap = paramsSpawnpoint;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<SpawnpointComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsStaticState = { { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" } };
		Component::createC<StaticStateComponent>(0, paramsStaticState);
		for (auto it = paramsStaticState.begin(); it != paramsStaticState.end(); it++) {
			auto newMap = paramsStaticState;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<StaticStateComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsTerrainAppearance = { { "heightmap", "blafoo" }, { "size", "0.0" }, { "inputScale", "0.0" }, { "vertices", "0" }, { "layers", "0" }, { "minX", "0" }, { "minY", "0" }, { "maxX", "0" }, { "maxY", "0" } };
		Component::createC<TerrainAppearanceComponent>(0, paramsTerrainAppearance);
		for (auto it = paramsTerrainAppearance.begin(); it != paramsTerrainAppearance.end(); it++) {
			auto newMap = paramsTerrainAppearance;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<TerrainAppearanceComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsVelocity = { { "acceleration", "0.0" }, { "maxSpeed", "0.0" } };
		Component::createC<VelocityComponent>(0, paramsVelocity);
		for (auto it = paramsVelocity.begin(); it != paramsVelocity.end(); it++) {
			auto newMap = paramsVelocity;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<VelocityComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	{
		attributeMap paramsWaypoint = { { "name", "WP_01" }, { "connections", "" } };
		Component::createC<WaypointComponent>(0, paramsWaypoint);
		for (auto it = paramsWaypoint.begin(); it != paramsWaypoint.end(); it++) {
			auto newMap = paramsWaypoint;
			newMap.erase(it->first);
			ASSERT_THROW(Component::createC<WaypointComponent>(0, newMap), i6engine::utils::exceptions::ApiException);
		}
	}
	ComPtr::clear();
}
