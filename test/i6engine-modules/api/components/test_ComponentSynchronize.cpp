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
#include "i6engine/api/components/SoundComponent.h"
#include "i6engine/api/components/SoundListenerComponent.h"
#include "i6engine/api/components/SpawnpointComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/components/TerrainAppearanceComponent.h"
#include "i6engine/api/components/WaypointComponent.h"

#include "gtest/gtest.h"

using i6engine::api::attributeMap;

TEST(Component, Synchronize) {
	{
		attributeMap paramsBillboard = { { "material", "blafoo" }, { "width", "0.1" }, { "height", "0.1" }, { "origin", "0" } };
		auto bc = i6engine::api::BillboardComponent::createC(0, paramsBillboard);
		auto newMap = bc->synchronize();
		i6engine::api::BillboardComponent::createC(0, newMap);
	}
	{
		attributeMap paramsCamera = { { "pos", "0.0 0.0 0.0" }, { "lookAt", "0.0 0.0 0.0" }, { "nearclip", "1" }, { "aspect", "1.0" }, { "viewport", "1" }, { "vp_left", "1.0" }, { "vp_top", "1.0" }, { "vp_width", "1.0" }, { "vp_height", "1.0" }, { "vp_red", "1.0" }, { "vp_green", "1.0" }, { "vp_blue", "1.0" }, { "vp_alpha", "1.0" } };
		auto cc = i6engine::api::CameraComponent::createC(0, paramsCamera);
		auto newMap = cc->synchronize();
		i6engine::api::CameraComponent::createC(0, newMap);
	}
	{
		attributeMap paramsFollow = { { "targetID", "-1" }, { "speed", "1.0" } };
		auto fc = i6engine::api::FollowComponent::createC(0, paramsFollow);
		auto newMap = fc->synchronize();
		i6engine::api::FollowComponent::createC(0, newMap);
	}
	{
		attributeMap paramsLifetime = { { "lifetime", "0" } };
		auto lc = i6engine::api::LifetimeComponent::createC(0, paramsLifetime);
		auto newMap = lc->synchronize();
		i6engine::api::LifetimeComponent::createC(0, newMap);
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		auto lc = i6engine::api::LuminousAppearanceComponent::createC(0, paramsLuminousAppearance);
		auto newMap = lc->synchronize();
		i6engine::api::LuminousAppearanceComponent::createC(0, newMap);
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "1" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		auto lc = i6engine::api::LuminousAppearanceComponent::createC(0, paramsLuminousAppearance);
		auto newMap = lc->synchronize();
		i6engine::api::LuminousAppearanceComponent::createC(0, newMap);
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "2" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		auto lc = i6engine::api::LuminousAppearanceComponent::createC(0, paramsLuminousAppearance);
		auto newMap = lc->synchronize();
		i6engine::api::LuminousAppearanceComponent::createC(0, newMap);
	}
	{
		attributeMap paramsMeshAppearance = { { "mesh", "blafoo.mesh" }, { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" }, { "visibility", "1" }, { "material", "blafoo" } };
		auto mc = i6engine::api::MeshAppearanceComponent::createC(0, paramsMeshAppearance);
		auto newMap = mc->synchronize();
		i6engine::api::MeshAppearanceComponent::createC(0, newMap);
	}
	{
		attributeMap paramsMovableText = { { "font", "blafoo" }, { "text", "text" }, { "size", "10" }, { "colour", "0.0 0.0 0.0" } };
		auto mc = i6engine::api::MovableTextComponent::createC(0, paramsMovableText);
		auto newMap = mc->synchronize();
		i6engine::api::MovableTextComponent::createC(0, newMap);
	}
	{
		attributeMap paramsMoverCircle = { { "axis", "0.0 1.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "realCenterPos", "0.0 0.0 0.0" }, { "lastPos", "0.0 0.0 0.0" }, { "duration", "1" }, { "radius", "1.0" }, { "positioning", "0" }, { "continue", "true" }, { "startTime", "0" }, { "started", "0" }, { "linkable", "1" } };
		auto mc = i6engine::api::MoverCircleComponent::createC(0, paramsMoverCircle);
		auto newMap = mc->synchronize();
		i6engine::api::MoverCircleComponent::createC(0, newMap);
	}
	{
		attributeMap paramsMoverInterpolate = { { "direction", "1" }, { "way", "0" }, { "keyframes", "1" }, { "keyframe_0_pos", "0.0 0.0 0.0" }, { "keyframe_0_rot", "0.0 0.0 0.0 0.0" }, { "duration", "1" }, { "mode", "0" }, { "positioning", "0" }, { "continue", "true" }, { "startTime", "0" }, { "started", "0" }, { "linkable", "1" } };
		auto mc = i6engine::api::MoverInterpolateComponent::createC(0, paramsMoverInterpolate);
		auto newMap = mc->synchronize();
		i6engine::api::MoverInterpolateComponent::createC(0, newMap);
	}
	{
		attributeMap paramsMovingCamera = { { "pos", "0.0 0.0 0.0" }, { "lookAt", "0.0 0.0 0.0" }, { "nearclip", "1" }, { "aspect", "1.0" }, { "viewport", "1" }, { "vp_left", "1.0" }, { "vp_top", "1.0" }, { "vp_width", "1.0" }, { "vp_height", "1.0" }, { "vp_red", "1.0" }, { "vp_green", "1.0" }, { "vp_blue", "1.0" }, { "vp_alpha", "1.0" } };
		auto mc = i6engine::api::MovingCameraComponent::createC(0, paramsMovingCamera);
		auto newMap = mc->synchronize();
		i6engine::api::MovingCameraComponent::createC(0, newMap);
	}
	{
		attributeMap paramsParticleEmitter = { { "pos", "0.0 0.0 0.0" }, { "particleEmitter", "blafoo.particle" }, { "fadeOut", "1" }, { "fadeOutCooldown", "1000000" } };
		auto pc = i6engine::api::ParticleEmitterComponent::createC(0, paramsParticleEmitter);
		auto newMap = pc->synchronize();
		i6engine::api::ParticleEmitterComponent::createC(0, newMap);
	}
	{
		attributeMap paramsPhysicalState = { { "shapeType", "0" }, { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" }, { "collisionGroup", "0 0 0" }, { "shatterInterest", "0" }, { "syncPrio", "0" }, { "compound", "0" }, { "gravity", "0.0 0.0 0.0" } };
		auto pc = i6engine::api::PhysicalStateComponent::createC(0, paramsPhysicalState);
		auto newMap = pc->synchronize();
		i6engine::api::PhysicalStateComponent::createC(0, newMap);
	}
	{
		attributeMap paramsSound = { { "file", "blafoo" }, { "direction", "0.0 0.0 0.0" }, { "offset", "0.0 0.0 0.0" }, { "looping", "0" }, { "maxDist", "10" }, { "cache", "0" } };
		auto sc = i6engine::api::SoundComponent::createC(0, paramsSound);
		auto newMap = sc->synchronize();
		i6engine::api::SoundComponent::createC(0, newMap);
	}
	{
		attributeMap paramsSoundListener = { };
		auto sc = i6engine::api::SoundListenerComponent::createC(0, paramsSoundListener);
		auto newMap = sc->synchronize();
		i6engine::api::SoundListenerComponent::createC(0, newMap);
	}
	{
		attributeMap paramsSpawnpoint = { { "spawntypes", "blafoo" } };
		auto sc = i6engine::api::SpawnpointComponent::createC(0, paramsSpawnpoint);
		auto newMap = sc->synchronize();
		i6engine::api::SpawnpointComponent::createC(0, newMap);
	}
	{
		attributeMap paramsStaticState = { { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" } };
		auto sc = i6engine::api::StaticStateComponent::createC(0, paramsStaticState);
		auto newMap = sc->synchronize();
		i6engine::api::StaticStateComponent::createC(0, newMap);
	}
	{
		attributeMap paramsTerrainAppearance = { { "heightmap", "blafoo" }, { "size", "0.0" }, { "inputScale", "0.0" }, { "layers", "1" }, { "layer_0_size", "1.0" }, { "layer_0_diffusespecular", "texture.png" }, { "layer_0_normal", "texture_normal.png" } };
		auto tc = i6engine::api::TerrainAppearanceComponent::createC(0, paramsTerrainAppearance);
		auto newMap = tc->synchronize();
		i6engine::api::TerrainAppearanceComponent::createC(0, newMap);
	}
	{
		attributeMap paramsWaypoint = { { "name", "WP_01" }, { "connections", "WP_02" } };
		auto wc = i6engine::api::WaypointComponent::createC(0, paramsWaypoint);
		auto newMap = wc->synchronize();
		i6engine::api::WaypointComponent::createC(0, newMap);
	}
	i6engine::api::ComPtr::clear();
}
