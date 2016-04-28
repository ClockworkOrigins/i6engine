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

#include "i6engine/core/messaging/IPKey.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"
#include "i6engine/api/components/MoverCircleComponent.h"
#include "i6engine/api/components/MoverInterpolateComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"

#include "gtest/gtest.h"

using namespace i6engine;
using namespace i6engine::api;

i6engine::api::ComPtr func(const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) {
	return i6engine::api::ComPtr();
}
void funcID(const int64_t) {
}

TEST(MoverComponent, absolute) {
	{
		EngineController::GetSingletonPtr()->getObjectFacade()->registerNotifyCallback(funcID);
		EngineController::GetSingletonPtr()->getObjectFacade()->registerAddTickerCallback([](const WeakComPtr &) {});
		EngineController::GetSingletonPtr()->getObjectFacade()->registerRemoveTickerCallback([](int64_t) {});

		Vec3 realCircleCenter(5, 10, 15);
		Vec3 axis(0, 0, 2);
		Vec3 start(1, 2, 3);

		i6engine::api::attributeMap aMap;
		aMap["positioning"] = "0";	// ABSOLUTE
		axis.insertInMap("axis", aMap);
		realCircleCenter.insertInMap("pos", aMap);
		aMap["radius"] = "50";
		aMap["duration"] = "100";
		aMap["started"] = "1";

		i6engine::api::attributeMap phyMap;		// just for dummy values
		axis.insertInMap("pos", phyMap);
		axis.insertInMap("scale", phyMap);
		Quaternion q(1, 2, 3, 4);
		q.insertInMap("rot", phyMap);
		phyMap["shapeType"] = "0";
		phyMap["collisionGroup"] = "0 0 0";
		phyMap["shatterInterest"] = "0";
		phyMap["compound"] = "0";

		i6engine::utils::sharedPtr<MoverCircleComponent, Component> mover = i6engine::utils::dynamic_pointer_cast<MoverCircleComponent>(Component::createC<MoverCircleComponent>(0, aMap));
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(1, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", func);
		i6engine::utils::sharedPtr<PhysicalStateComponent, Component> psc = i6engine::utils::make_shared<PhysicalStateComponent, Component>(2, phyMap);
		owner->setSelf(owner);
		owner->setGOC(psc);
		owner->setGOC(mover);
		mover->Init();

		mover->start(start);
		utils::dynamic_pointer_cast<Component>(psc)->Tick();

		ASSERT_EQ(Vec3(-45, 10, 15), psc->getPosition());
	}
	GOPtr::clear();
	i6engine::utils::sharedPtr<Component, Component>::clear();
}

TEST(MoverComponent, absoluteStartPosition) {
	{
		EngineController::GetSingletonPtr()->getObjectFacade()->registerNotifyCallback(funcID);
		EngineController::GetSingletonPtr()->getObjectFacade()->registerAddTickerCallback([](const WeakComPtr &) {});
		EngineController::GetSingletonPtr()->getObjectFacade()->registerRemoveTickerCallback([](int64_t) {});

		i6engine::api::attributeMap paramsMover;
		paramsMover["direction"] = "1";
		paramsMover["way"] = "0";
		paramsMover["duration"] = "5000000";
		paramsMover["positioning"] = "0";
		paramsMover["started"] = "1";
		paramsMover["keyframes"] = "2";
		paramsMover["mode"] = "3";

		Vec3 pos(1.0, 2.0, -3.0);
		Quaternion rot = Quaternion::IDENTITY;

		pos.insertInMap("keyframe_0_pos", paramsMover);
		(pos + Vec3(0.0, -5.0, 0.0)).insertInMap("keyframe_1_pos", paramsMover);
		rot.insertInMap("keyframe_0_rot", paramsMover);
		rot.insertInMap("keyframe_1_rot", paramsMover);

		i6engine::api::attributeMap phyMap;		// just for dummy values
		pos = Vec3(42.0, 3.14, -5.0);
		pos.insertInMap("pos", phyMap);
		phyMap["scale"] = "1.0 1.0 1.0";
		Quaternion q = Quaternion::IDENTITY;
		q.insertInMap("rot", phyMap);
		phyMap["shapeType"] = "0";
		phyMap["collisionGroup"] = "0 0 0";
		phyMap["shatterInterest"] = "0";
		phyMap["compound"] = "0";

		i6engine::utils::sharedPtr<MoverInterpolateComponent, Component> mover = i6engine::utils::dynamic_pointer_cast<MoverInterpolateComponent>(Component::createC<MoverInterpolateComponent>(0, paramsMover));
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(1, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", func);
		i6engine::utils::sharedPtr<PhysicalStateComponent, Component> psc = i6engine::utils::make_shared<PhysicalStateComponent, Component>(2, phyMap);
		owner->setSelf(owner);
		owner->setGOC(psc);
		owner->setGOC(mover);
		utils::dynamic_pointer_cast<Component>(psc)->Init();

		// assume the psc was added earlier and objects was already moved to some position unequal to Vec3::ZERO
		psc->setPosition(psc->getPosition(), 1);

		mover->Init();

		utils::dynamic_pointer_cast<Component>(psc)->Tick();

		ASSERT_EQ(Vec3(1.0, 2.0, -3.0), psc->getPosition());
	}
	GOPtr::clear();
	i6engine::utils::sharedPtr<Component, Component>::clear();
}
