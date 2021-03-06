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

using namespace i6e;
using namespace i6e::api;

TEST(Component, Synchronize) {
	{
		attributeMap paramsAnimatedDirectionalLightAppearance = { { "lightType", "1" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" }, { "diffuseDuration", "1000" }, { "diffuseKey_0", "1.0 0.0 0.0" }, { "diffuseKey_1", "0.0 1.0 0.0" }, { "specularDuration", "1000" }, { "specularKey_0", "1.0 0.0 0.0" }, { "specularKey_1", "0.0 1.0 0.0" }, { "attenuationDuration", "1000" }, { "attenuationKey_0", "1.0 0.0 0.0 0.0" }, { "attenuationKey_1", "0.0 1.0 0.0 0.0" }, { "directionDuration", "1000" }, { "directionKey_0", "1.0 0.0 0.0" }, { "directionKey_1", "0.0 1.0 0.0" } };
		auto lc = Component::createC<AnimatedDirectionalLightComponent>(0, paramsAnimatedDirectionalLightAppearance);
		auto newMap = lc->synchronize();
		Component::createC<AnimatedDirectionalLightComponent>(0, newMap);
	}
	{
		attributeMap paramsAnimatedLuminousAppearance = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" }, { "diffuseDuration", "1000" }, { "diffuseKey_0", "1.0 0.0 0.0" }, { "diffuseKey_1", "0.0 1.0 0.0" }, { "specularDuration", "1000" }, { "specularKey_0", "1.0 0.0 0.0" }, { "specularKey_1", "0.0 1.0 0.0" }, { "attenuationDuration", "1000" }, { "attenuationKey_0", "1.0 0.0 0.0 0.0" }, { "attenuationKey_1", "0.0 1.0 0.0 0.0" } };
		auto lc = Component::createC<AnimatedLuminousAppearanceComponent>(0, paramsAnimatedLuminousAppearance);
		auto newMap = lc->synchronize();
		Component::createC<AnimatedLuminousAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsAnimatedLuminousAppearance = { { "lightType", "1" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" }, { "diffuseDuration", "1000" }, { "diffuseKey_0", "1.0 0.0 0.0" }, { "diffuseKey_1", "0.0 1.0 0.0" }, { "specularDuration", "1000" }, { "specularKey_0", "1.0 0.0 0.0" }, { "specularKey_1", "0.0 1.0 0.0" }, { "attenuationDuration", "1000" }, { "attenuationKey_0", "1.0 0.0 0.0 0.0" }, { "attenuationKey_1", "0.0 1.0 0.0 0.0" } };
		auto lc = Component::createC<AnimatedLuminousAppearanceComponent>(0, paramsAnimatedLuminousAppearance);
		auto newMap = lc->synchronize();
		Component::createC<AnimatedLuminousAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsAnimatedLuminousAppearance = { { "lightType", "2" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" }, { "diffuseDuration", "1000" }, { "diffuseKey_0", "1.0 0.0 0.0" }, { "diffuseKey_1", "0.0 1.0 0.0" }, { "specularDuration", "1000" }, { "specularKey_0", "1.0 0.0 0.0" }, { "specularKey_1", "0.0 1.0 0.0" }, { "attenuationDuration", "1000" }, { "attenuationKey_0", "1.0 0.0 0.0 0.0" }, { "attenuationKey_1", "0.0 1.0 0.0 0.0" } };
		auto lc = Component::createC<AnimatedLuminousAppearanceComponent>(0, paramsAnimatedLuminousAppearance);
		auto newMap = lc->synchronize();
		Component::createC<AnimatedLuminousAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsAnimatedSpotLightAppearance = { { "lightType", "2" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" }, { "diffuseDuration", "1000" }, { "diffuseKey_0", "1.0 0.0 0.0" }, { "diffuseKey_1", "0.0 1.0 0.0" }, { "specularDuration", "1000" }, { "specularKey_0", "1.0 0.0 0.0" }, { "specularKey_1", "0.0 1.0 0.0" }, { "attenuationDuration", "1000" }, { "attenuationKey_0", "1.0 0.0 0.0 0.0" }, { "attenuationKey_1", "0.0 1.0 0.0 0.0" }, { "directionDuration", "1000" }, { "directionKey_0", "1.0 0.0 0.0" }, { "directionKey_1", "0.0 1.0 0.0" }, { "innerRangeDuration", "1000" }, { "innerRangeKey_0", "1.0" }, { "innerRangeKey_1", "10.0" }, { "outerRangeDuration", "1000" }, { "outerRangeKey_0", "1.0" }, { "outerRangeKey_1", "10.0" } };
		auto lc = Component::createC<AnimatedSpotLightComponent>(0, paramsAnimatedSpotLightAppearance);
		auto newMap = lc->synchronize();
		Component::createC<AnimatedSpotLightComponent>(0, newMap);
	}
	{
		attributeMap paramsAnimationController = { };
		auto acc = Component::createC<AnimationControllerComponent>(0, paramsAnimationController);
		auto newMap = acc->synchronize();
		Component::createC<AnimationControllerComponent>(0, newMap);
	}
	{
		attributeMap paramsBillboard = { { "material", "blafoo" }, { "width", "0.1" }, { "height", "0.1" }, { "origin", "0" } };
		auto bc = Component::createC<BillboardComponent>(0, paramsBillboard);
		auto newMap = bc->synchronize();
		Component::createC<BillboardComponent>(0, newMap);
	}
	{
		attributeMap paramsCamera = { { "pos", "0.0 0.0 0.0" }, { "lookAt", "0.0 0.0 0.0" }, { "nearclip", "1" }, { "aspect", "1.0" }, { "viewport", "1" }, { "zOrder", "1" }, { "vp_left", "1.0" }, { "vp_top", "1.0" }, { "vp_width", "1.0" }, { "vp_height", "1.0" }, { "vp_red", "1.0" }, { "vp_green", "1.0" }, { "vp_blue", "1.0" }, { "vp_alpha", "1.0" } };
		auto cc = Component::createC<CameraComponent>(0, paramsCamera);
		auto newMap = cc->synchronize();
		Component::createC<CameraComponent>(0, newMap);
	}
	{
		attributeMap paramsFollow = { { "targetID", "-1" }, { "speed", "1.0" } };
		auto fc = Component::createC<FollowComponent>(0, paramsFollow);
		auto newMap = fc->synchronize();
		Component::createC<FollowComponent>(0, newMap);
	}
	{
		attributeMap paramsLifetime = { { "lifetime", "0" } };
		auto lc = Component::createC<LifetimeComponent>(0, paramsLifetime);
		auto newMap = lc->synchronize();
		Component::createC<LifetimeComponent>(0, newMap);
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "0" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		auto lc = Component::createC<LuminousAppearanceComponent>(0, paramsLuminousAppearance);
		auto newMap = lc->synchronize();
		Component::createC<LuminousAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "1" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		auto lc = Component::createC<LuminousAppearanceComponent>(0, paramsLuminousAppearance);
		auto newMap = lc->synchronize();
		Component::createC<LuminousAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsLuminousAppearance = { { "lightType", "2" }, { "diffuseColor", "0.0 0.0 0.0" }, { "specularColor", "0.0 0.0 0.0" }, { "attenuation", "0.0 0.0 0.0 0.0" }, { "direction", "0.0 0.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "spotLightRangeInner", "1.0" }, { "spotLightRangeOuter", "1.0" } };
		auto lc = Component::createC<LuminousAppearanceComponent>(0, paramsLuminousAppearance);
		auto newMap = lc->synchronize();
		Component::createC<LuminousAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsMeshAppearance = { { "mesh", "blafoo.mesh" }, { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" }, { "visibility", "1" }, { "material", "blafoo" } };
		auto mc = Component::createC<MeshAppearanceComponent>(0, paramsMeshAppearance);
		auto newMap = mc->synchronize();
		Component::createC<MeshAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsMovableText = { { "font", "blafoo" }, { "text", "text" }, { "size", "10" }, { "colour", "0.0 0.0 0.0" } };
		auto mc = Component::createC<MovableTextComponent>(0, paramsMovableText);
		auto newMap = mc->synchronize();
		Component::createC<MovableTextComponent>(0, newMap);
	}
	{
		attributeMap paramsMoverCircle = { { "axis", "0.0 1.0 0.0" }, { "pos", "0.0 0.0 0.0" }, { "realCenterPos", "0.0 0.0 0.0" }, { "lastPos", "0.0 0.0 0.0" }, { "duration", "1" }, { "radius", "1.0" }, { "positioning", "0" }, { "continue", "true" }, { "startTime", "0" }, { "started", "0" }, { "linkable", "1" } };
		auto mc = Component::createC<MoverCircleComponent>(0, paramsMoverCircle);
		auto newMap = mc->synchronize();
		Component::createC<MoverCircleComponent>(0, newMap);
	}
	{
		attributeMap paramsMoverInterpolate = { { "direction", "1" }, { "way", "0" }, { "keyframes", "1" }, { "keyframe_0_pos", "0.0 0.0 0.0" }, { "keyframe_0_rot", "0.0 0.0 0.0 0.0" }, { "duration", "1" }, { "mode", "0" }, { "positioning", "0" }, { "continue", "true" }, { "startTime", "0" }, { "started", "0" }, { "linkable", "1" } };
		auto mc = Component::createC<MoverInterpolateComponent>(0, paramsMoverInterpolate);
		auto newMap = mc->synchronize();
		Component::createC<MoverInterpolateComponent>(0, newMap);
	}
	{
		attributeMap paramsMovingCamera = { { "pos", "0.0 0.0 0.0" }, { "lookAt", "0.0 0.0 0.0" }, { "nearclip", "1" }, { "aspect", "1.0" }, { "viewport", "1" }, { "zOrder", "1" }, { "vp_left", "1.0" }, { "vp_top", "1.0" }, { "vp_width", "1.0" }, { "vp_height", "1.0" }, { "vp_red", "1.0" }, { "vp_green", "1.0" }, { "vp_blue", "1.0" }, { "vp_alpha", "1.0" } };
		auto mc = Component::createC<MovingCameraComponent>(0, paramsMovingCamera);
		auto newMap = mc->synchronize();
		Component::createC<MovingCameraComponent>(0, newMap);
	}
	{
		attributeMap paramsParticleEmitter = { { "pos", "0.0 0.0 0.0" }, { "particleEmitter", "blafoo.particle" }, { "fadeOut", "1" }, { "fadeOutCooldown", "1000000" } };
		auto pc = Component::createC<ParticleEmitterComponent>(0, paramsParticleEmitter);
		auto newMap = pc->synchronize();
		Component::createC<ParticleEmitterComponent>(0, newMap);
	}
	{
		attributeMap paramsPhysicalState = { { "shapeType", "0" }, { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" }, { "collisionGroup", "0 0 0" }, { "shatterInterest", "0" }, { "syncPrio", "0" }, { "compound", "0" }, { "gravity", "0.0 0.0 0.0" } };
		auto pc = Component::createC<PhysicalStateComponent>(0, paramsPhysicalState);
		auto newMap = pc->synchronize();
		Component::createC<PhysicalStateComponent>(0, newMap);
	}
	{
		attributeMap paramsPoint2PointConstraint = { { "selfIdentifier", "a" }, { "targetIdentifier", "b" }, { "selfOffset", "0.0 0.0 0.0" }, { "targetOffset", "0.0 0.0 0.0" }, { "breakingImpulse", "0.0" } };
		auto pc = Component::createC<Point2PointConstraintComponent>(0, paramsPoint2PointConstraint);
		auto newMap = pc->synchronize();
		Component::createC<Point2PointConstraintComponent>(0, newMap);
	}
	{
		attributeMap paramsSound = { { "file", "blafoo" }, { "direction", "0.0 0.0 0.0" }, { "offset", "0.0 0.0 0.0" }, { "looping", "0" }, { "maxDist", "10" }, { "cache", "0" } };
		auto sc = Component::createC<SoundComponent>(0, paramsSound);
		auto newMap = sc->synchronize();
		Component::createC<SoundComponent>(0, newMap);
	}
	{
		attributeMap paramsSoundListener = { };
		auto sc = Component::createC<SoundListenerComponent>(0, paramsSoundListener);
		auto newMap = sc->synchronize();
		Component::createC<SoundListenerComponent>(0, newMap);
	}
	{
		attributeMap paramsSpawnpoint = { { "spawntypes", "blafoo" } };
		auto sc = Component::createC<SpawnpointComponent>(0, paramsSpawnpoint);
		auto newMap = sc->synchronize();
		Component::createC<SpawnpointComponent>(0, newMap);
	}
	{
		attributeMap paramsStaticState = { { "pos", "0.0 0.0 0.0" }, { "scale", "0.0 0.0 0.0" }, { "rot", "0.0 0.0 0.0 0.0" } };
		auto sc = Component::createC<StaticStateComponent>(0, paramsStaticState);
		auto newMap = sc->synchronize();
		Component::createC<StaticStateComponent>(0, newMap);
	}
	{
		attributeMap paramsTerrainAppearance = { { "heightmap", "blafoo" }, { "size", "0.0" }, { "inputScale", "0.0" }, { "vertices", "0" }, { "layers", "2" }, { "layer_0_size", "1.0" }, { "layer_0_diffusespecular", "texture.png" }, { "layer_0_normal", "texture_normal.png" }, { "layer_1_size", "1.0" }, { "layer_1_diffusespecular", "texture.png" }, { "layer_1_normal", "texture_normal.png" }, { "layer_1_minHeight", "0.0" }, { "layer_1_fadeDist", "0.0" }, { "minX", "0" }, { "minY", "0" }, { "maxX", "0" }, { "maxY", "0" } };
		auto tc = Component::createC<TerrainAppearanceComponent>(0, paramsTerrainAppearance);
		auto newMap = tc->synchronize();
		Component::createC<TerrainAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsTerrainAppearance = { { "size", "0.0" }, { "inputScale", "0.0" }, { "vertices", "0" }, { "layers", "2" }, { "layer_0_size", "1.0" }, { "layer_0_diffusespecular", "texture.png" }, { "layer_0_normal", "texture_normal.png" }, { "layer_1_size", "1.0" }, { "layer_1_diffusespecular", "texture.png" }, { "layer_1_normal", "texture_normal.png" }, { "layer_1_minHeight", "0.0" }, { "layer_1_fadeDist", "0.0" }, { "minX", "0" }, { "minY", "0" }, { "maxX", "0" }, { "maxY", "0" } };
		writeAttribute(paramsTerrainAppearance, "heightdata", std::vector<std::vector<double>>());
		auto tc = Component::createC<TerrainAppearanceComponent>(0, paramsTerrainAppearance);
		auto newMap = tc->synchronize();
		Component::createC<TerrainAppearanceComponent>(0, newMap);
	}
	{
		attributeMap paramsVelocity = { { "acceleration", "0.0" }, { "deceleration", "0.0" }, { "maxSpeed", "0.0" }, { "resistanceCoefficient", "0.0" }, { "windage", "0.0" }, { "handling", "0" } };
		auto vc = Component::createC<VelocityComponent>(0, paramsVelocity);
		auto newMap = vc->synchronize();
		Component::createC<VelocityComponent>(0, newMap);
	}
	{
		attributeMap paramsWaypoint = { { "name", "WP_01" }, { "connections", "WP_02" } };
		auto wc = Component::createC<WaypointComponent>(0, paramsWaypoint);
		auto newMap = wc->synchronize();
		Component::createC<WaypointComponent>(0, newMap);
	}
	ComPtr::clear();
}
