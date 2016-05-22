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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/object/ObjectController.h"

#include "../../../../samples/common/include/CommonApplication.h"

#include "gtest/gtest.h"

using namespace i6e;
using namespace i6e::api;

namespace Test_ObjectFacade {

	class ObjectFacadeTest : public ::testing::Test, public sample::CommonApplication {
	protected:
		ObjectFacadeTest() : sample::CommonApplication(false, false), _lock(), _conditionVariable() {
		}

		~ObjectFacadeTest() {
			EngineController::GetSingletonPtr()->reset();
		}

		// registers test as application and ObjectController
		virtual void SetUp() {
			EngineController::GetSingletonPtr()->reset();
			EngineController::GetSingletonPtr()->registerApplication(*this);
			EngineController::GetSingletonPtr()->registerSubSystem("Object", new modules::ObjectController(), 10000);

			std::unique_lock<std::mutex> ul(_lock);
			std::thread(std::bind(&EngineController::start, EngineController::GetSingletonPtr())).detach();
			_conditionVariable.wait(ul);
		}

		void AfterInitialize() override {
			std::unique_lock<std::mutex> ul(_lock);
			_conditionVariable.notify_one();
		}

		virtual void TearDown() {
			std::unique_lock<std::mutex> ul(_lock);
			EngineController::GetSingletonPtr()->stop();
			_conditionVariable.wait(ul);
		}

		void Finalize() override {
			std::unique_lock<std::mutex> ul(_lock);
			_conditionVariable.notify_one();
		}

		std::mutex _lock;
		std::condition_variable _conditionVariable;
	};

	TEST_F(ObjectFacadeTest, createGO) {
		EXPECT_EQ(0, EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap().size());
		std::unique_lock<std::mutex> ul(_lock);
		EngineController::GetSingletonPtr()->getObjectFacade()->createGO("JengaStick", objects::GOTemplate(), EngineController::GetSingletonPtr()->getUUID(), false, [this](GOPtr go) {
			std::unique_lock<std::mutex> ul(_lock);
			EXPECT_EQ("JengaStick", go->getType());
			_conditionVariable.notify_one();
		});
		_conditionVariable.wait(ul);
		EXPECT_EQ(1, EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap().size());
	}

	TEST_F(ObjectFacadeTest, createComponentCallback) {
		EXPECT_EQ(0, EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap().size());
		std::unique_lock<std::mutex> ul(_lock);
		EngineController::GetSingletonPtr()->getObjectFacade()->createGO("JengaStick", objects::GOTemplate(), EngineController::GetSingletonPtr()->getUUID(), false, [this](GOPtr go) {
			attributeMap params;
			params.insert(std::make_pair("targetID", "1"));
			params.insert(std::make_pair("speed", "1.0"));
			EXPECT_TRUE(go->getGOC(components::ComponentTypes::FollowComponent) == nullptr);
			EngineController::GetSingletonPtr()->getObjectFacade()->createComponentCallback(go->getID(), -1, "Follow", params, [this, go](ComPtr c) {
				std::unique_lock<std::mutex> ul(_lock);
				EXPECT_TRUE(go->getGOC(components::ComponentTypes::FollowComponent) != nullptr);
				EXPECT_TRUE(go == c->getOwnerGO());
				_conditionVariable.notify_one();
			});
		});
		_conditionVariable.wait(ul);
		EXPECT_EQ(1, EngineController::GetSingletonPtr()->getObjectFacade()->getGOMap().size());
	}

}
