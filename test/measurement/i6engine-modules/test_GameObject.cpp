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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "gtest/gtest.h"

using namespace i6e;
using namespace i6e::api;

namespace Test_GameObject {
namespace config {

	enum ComponentTypes {
		AddComponent,
		Replace0Component,
		Replace1Component,
		ReplaceDisComponent,
		RejectComponent
	};

} /* namespace config */

	class TestComponent : public Component {
	public:
		TestComponent(const int64_t id, const api::attributeMap & params) : Component(id, params) {
		}
		virtual ~TestComponent() {
		}
		void Init() override {}
		api::attributeMap synchronize() const override { return api::attributeMap(); }

		std::string getTemplateName() const override {
			return "";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}
	};

	class AddComponent : public TestComponent {
	public:
		AddComponent(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
			_objFamilyID = config::ComponentTypes::AddComponent;
		}

		~AddComponent() {
		}

		// ADD unless duplicate detected. than reject
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & c) const {
			if (std::any_of(_subComps.begin(), _subComps.end(), [c](const ComPtr & cc) { return cc->getIdentifier() == c->getIdentifier(); })) {
				return std::make_pair(AddStrategy::REJECT, 0);
			} else {
				return std::make_pair(AddStrategy::ADD, 0);
			}
		}
	};

	class Replace0Component : public TestComponent {
	public:
		Replace0Component(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
			_objFamilyID = config::ComponentTypes::Replace0Component;
		}

		// all odd ids replace the dispatcher
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & c) const {
			if (c->getID() % 2) {
				return std::make_pair(AddStrategy::REPLACE, -1);
			} else {
				return std::make_pair(AddStrategy::ADD, 0);
			}
		}
	};

	class Replace1Component : public TestComponent {
	public:
		Replace1Component(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
			_objFamilyID = config::ComponentTypes::Replace1Component;
		}

		// 4 and 5 replace 3 and 2 (ids, not idx in vector)
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & c) const {
			if (c->getID() == 4) {
				return std::make_pair(AddStrategy::REPLACE, 1);
			} else if (c->getID() == 5) {
				return std::make_pair(AddStrategy::REPLACE, 0);
			} else {
				return std::make_pair(AddStrategy::ADD, 0);
			}
		}
	};

	class ReplaceDisComponent : public TestComponent {
	public:
		ReplaceDisComponent(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
			_objFamilyID = config::ComponentTypes::ReplaceDisComponent;
		}
		// always replace dispatcher
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr &) const {
			return std::make_pair(AddStrategy::REPLACE_DIS, 0);
		}
	};

	class RejectComponent : public TestComponent {
	public:
		RejectComponent(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
			_objFamilyID = config::ComponentTypes::RejectComponent;
		}

		// always rejects
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr &) const {
			return std::make_pair(AddStrategy::REJECT, 0);
		}
	};

	ComPtr func(const int64_t, const std::string &, const i6e::api::attributeMap &, GameObject *) {
		return ComPtr();
	}

	class GOTest : public ::testing::Test {
	protected:
		virtual void SetUp() {
			EngineController::GetSingletonPtr()->getObjectFacade()->registerNotifyCallback([](const int64_t) {});
			EngineController::GetSingletonPtr()->getObjectFacade()->registerAddTickerCallback([](const WeakComPtr &) {});
			EngineController::GetSingletonPtr()->getObjectFacade()->registerRemoveTickerCallback([](int64_t) {});
		}

		virtual void TearDown() {
			// TODO: (Michael) kill EngineController to get a complete new one in next test (Singleton)
			GOPtr::clear();
			i6e::api::ComPtr::clear();
		}
	};

	TEST_F(GOTest, getGOC) {
		GOPtr owner = i6e::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6e::api::attributeMap &, const WeakGOPtr &) { return i6e::api::ComPtr(); });
		i6e::api::attributeMap attMap;
		attMap["identifier"] = "A";
		i6e::api::ComPtr cAdd1 = i6e::utils::make_shared<AddComponent, i6e::api::Component>(1, attMap);
		i6e::api::ComPtr cAdd2 = i6e::utils::make_shared<Replace1Component, i6e::api::Component>(2, attMap);
		i6e::api::ComPtr cAdd3 = i6e::utils::make_shared<ReplaceDisComponent, i6e::api::Component>(3, attMap);
		i6e::api::ComPtr cAdd4 = i6e::utils::make_shared<RejectComponent, i6e::api::Component>(4, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(cAdd1);
		owner->setGOC(cAdd2);
		owner->setGOC(cAdd3);
		owner->setGOC(cAdd4);

		EXPECT_NE(nullptr, owner->getGOC(config::ComponentTypes::AddComponent));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace0Component));
		EXPECT_NE(nullptr, i6e::utils::dynamic_pointer_cast<AddComponent>(owner->getGOC(config::ComponentTypes::AddComponent)));
		EXPECT_EQ(nullptr, i6e::utils::dynamic_pointer_cast<Replace0Component>(owner->getGOC(config::ComponentTypes::AddComponent)));
		EXPECT_NE(nullptr, owner->getGOC<AddComponent>(config::ComponentTypes::AddComponent));
		EXPECT_EQ(nullptr, owner->getGOC<Replace0Component>(config::ComponentTypes::AddComponent));
		EXPECT_NE(nullptr, owner->getGOC<AddComponent>());
		EXPECT_EQ(nullptr, owner->getGOC<Replace0Component>());

		const uint32_t COUNTER = 1000000;

		uint64_t startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		for (uint32_t i = 0; i < COUNTER; i++) {
			EXPECT_NE(nullptr, owner->getGOC(config::ComponentTypes::AddComponent));
			EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace0Component));
		}
		uint64_t endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		std::cout << "Test 1: " << endTime - startTime << std::endl;

		startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		for (uint32_t i = 0; i < COUNTER; i++) {
			EXPECT_NE(nullptr, i6e::utils::dynamic_pointer_cast<AddComponent>(owner->getGOC(config::ComponentTypes::AddComponent)));
			EXPECT_EQ(nullptr, i6e::utils::dynamic_pointer_cast<Replace0Component>(owner->getGOC(config::ComponentTypes::AddComponent)));
		}
		endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		std::cout << "Test 2: " << endTime - startTime << std::endl;

		startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		for (uint32_t i = 0; i < COUNTER; i++) {
			EXPECT_NE(nullptr, owner->getGOC<AddComponent>(config::ComponentTypes::AddComponent));
			EXPECT_EQ(nullptr, owner->getGOC<Replace0Component>(config::ComponentTypes::AddComponent));
		}
		endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		std::cout << "Test 3: " << endTime - startTime << std::endl;

		startTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		for (uint32_t i = 0; i < COUNTER; i++) {
			EXPECT_NE(nullptr, owner->getGOC<AddComponent>());
			EXPECT_EQ(nullptr, owner->getGOC<Replace0Component>());
		}
		endTime = uint64_t(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		std::cout << "Test 4: " << endTime - startTime << std::endl;
	}
}
