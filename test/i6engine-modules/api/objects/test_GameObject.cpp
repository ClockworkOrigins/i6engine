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

using namespace i6engine;
using namespace i6engine::api;

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
		void Init() {}
		api::attributeMap synchronize() const { return api::attributeMap(); }

		std::string getTemplateName() const {
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
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & c) const {
			return std::make_pair(AddStrategy::REPLACE_DIS, 0);
		}
	};

	class RejectComponent : public TestComponent {
	public:
		RejectComponent(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
			_objFamilyID = config::ComponentTypes::RejectComponent;
		}
		
		// always rejects
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & c) const {
			return std::make_pair(AddStrategy::REJECT, 0);
		}
	};

	ComPtr func(const int64_t, const std::string &, const i6engine::api::attributeMap &, GameObject *) {
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
			i6engine::api::ComPtr::clear();
		}
	};

	TEST_F(GOTest, Component_Add) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A"; // first
		i6engine::api::ComPtr cAdd1 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(1, attMap);

		attMap["identifier"] = "B"; // add
		i6engine::api::ComPtr cAdd2 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(2, attMap);

		attMap["identifier"] = "C"; // add
		i6engine::api::ComPtr cAdd3 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(3, attMap);

		attMap["identifier"] = "D"; // add
		i6engine::api::ComPtr cAdd4 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(4, attMap);

		attMap["identifier"] = "B"; // reject (duplicate identifier)
		i6engine::api::ComPtr cAdd5 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(5, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(cAdd1);
		owner->setGOC(cAdd2);
		owner->setGOC(cAdd3);
		owner->setGOC(cAdd4);
		owner->setGOC(cAdd5);

		// final form
		// 1A | 2B 3C 4D
		EXPECT_EQ(cAdd1, owner->getGOC(config::ComponentTypes::AddComponent));
		EXPECT_EQ(cAdd1, owner->getGOC(config::ComponentTypes::AddComponent, "A"));
		EXPECT_EQ(cAdd1, owner->getGOCID(1));
		EXPECT_EQ(cAdd2, owner->getGOC(config::ComponentTypes::AddComponent, "B"));
		EXPECT_EQ(cAdd2, owner->getGOCID(2));
		EXPECT_EQ(cAdd3, owner->getGOC(config::ComponentTypes::AddComponent, "C"));
		EXPECT_EQ(cAdd3, owner->getGOCID(3));
		EXPECT_EQ(cAdd4, owner->getGOC(config::ComponentTypes::AddComponent, "D"));
		EXPECT_EQ(cAdd4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "E"));
		EXPECT_EQ(nullptr, owner->getGOCID(5));
	}

	TEST_F(GOTest, Component_Rep0) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A"; // first
		i6engine::api::ComPtr c1 = i6engine::utils::make_shared<Replace0Component, i6engine::api::Component>(1, attMap);

		attMap["identifier"] = "B"; // add
		i6engine::api::ComPtr c2 = i6engine::utils::make_shared<Replace0Component, i6engine::api::Component>(2, attMap);

		attMap["identifier"] = "C"; // replace dis
		i6engine::api::ComPtr c3 = i6engine::utils::make_shared<Replace0Component, i6engine::api::Component>(3, attMap);

		attMap["identifier"] = "D"; // add
		i6engine::api::ComPtr c4 = i6engine::utils::make_shared<Replace0Component, i6engine::api::Component>(4, attMap);

		attMap["identifier"] = "B"; // replace dis
		i6engine::api::ComPtr c5 = i6engine::utils::make_shared<Replace0Component, i6engine::api::Component>(5, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(c1);
		owner->setGOC(c2);
		owner->setGOC(c3);
		owner->setGOC(c4);
		owner->setGOC(c5);

		// final form
		// 5B | 2B 4D <- duplicate B can lead to problems
		EXPECT_EQ(c5, owner->getGOC(config::ComponentTypes::Replace0Component));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace0Component, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(c5, owner->getGOC(config::ComponentTypes::Replace0Component, "B"));
		EXPECT_EQ(c2, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace0Component, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(c4, owner->getGOC(config::ComponentTypes::Replace0Component, "D"));
		EXPECT_EQ(c4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace0Component, "E"));
		EXPECT_EQ(c5, owner->getGOCID(5));
	}

	TEST_F(GOTest, Component_Rep1) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A"; // first
		i6engine::api::ComPtr c1 = i6engine::utils::make_shared<Replace1Component, i6engine::api::Component>(1, attMap);

		attMap["identifier"] = "B"; // add
		i6engine::api::ComPtr c2 = i6engine::utils::make_shared<Replace1Component, i6engine::api::Component>(2, attMap);

		attMap["identifier"] = "C"; // add
		i6engine::api::ComPtr c3 = i6engine::utils::make_shared<Replace1Component, i6engine::api::Component>(3, attMap);

		attMap["identifier"] = "D"; // replace 3C
		i6engine::api::ComPtr c4 = i6engine::utils::make_shared<Replace1Component, i6engine::api::Component>(4, attMap);

		attMap["identifier"] = "B"; // replace 2B
		i6engine::api::ComPtr c5 = i6engine::utils::make_shared<Replace1Component, i6engine::api::Component>(5, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(c1);
		owner->setGOC(c2);
		owner->setGOC(c3);
		owner->setGOC(c4);
		owner->setGOC(c5);

		// final form
		// 1A | 5B 4D
		EXPECT_EQ(c1, owner->getGOC(config::ComponentTypes::Replace1Component));
		EXPECT_EQ(c1, owner->getGOC(config::ComponentTypes::Replace1Component, "A"));
		EXPECT_EQ(c1, owner->getGOCID(1));
		EXPECT_EQ(c5, owner->getGOC(config::ComponentTypes::Replace1Component, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace1Component, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(c4, owner->getGOC(config::ComponentTypes::Replace1Component, "D"));
		EXPECT_EQ(c4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace1Component, "E"));
		EXPECT_EQ(c5, owner->getGOCID(5));
	}

	TEST_F(GOTest, Component_RepDis) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A"; // first
		i6engine::api::ComPtr c1 = i6engine::utils::make_shared<ReplaceDisComponent, i6engine::api::Component>(1, attMap);

		attMap["identifier"] = "B"; // replace
		i6engine::api::ComPtr c2 = i6engine::utils::make_shared<ReplaceDisComponent, i6engine::api::Component>(2, attMap);

		attMap["identifier"] = "C"; // replace
		i6engine::api::ComPtr c3 = i6engine::utils::make_shared<ReplaceDisComponent, i6engine::api::Component>(3, attMap);

		attMap["identifier"] = "D"; // replace
		i6engine::api::ComPtr c4 = i6engine::utils::make_shared<ReplaceDisComponent, i6engine::api::Component>(4, attMap);

		attMap["identifier"] = "B"; // replace
		i6engine::api::ComPtr c5 = i6engine::utils::make_shared<ReplaceDisComponent, i6engine::api::Component>(5, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(c1);
		owner->setGOC(c2);
		owner->setGOC(c3);
		owner->setGOC(c4);
		owner->setGOC(c5);

		// final form
		// 5B |
		EXPECT_EQ(c5, owner->getGOC(config::ComponentTypes::ReplaceDisComponent));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::ReplaceDisComponent, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(c5, owner->getGOC(config::ComponentTypes::ReplaceDisComponent, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::ReplaceDisComponent, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::ReplaceDisComponent, "D"));
		EXPECT_EQ(nullptr, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::ReplaceDisComponent, "E"));
		EXPECT_EQ(c5, owner->getGOCID(5));
	}

	TEST_F(GOTest, Component_Rej) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A"; // first
		i6engine::api::ComPtr c1 = i6engine::utils::make_shared<RejectComponent, i6engine::api::Component>(1, attMap);

		attMap["identifier"] = "B"; // reject
		i6engine::api::ComPtr c2 = i6engine::utils::make_shared<RejectComponent, i6engine::api::Component>(2, attMap);

		attMap["identifier"] = "C"; // reject
		i6engine::api::ComPtr c3 = i6engine::utils::make_shared<RejectComponent, i6engine::api::Component>(3, attMap);

		attMap["identifier"] = "D"; // reject
		i6engine::api::ComPtr c4 = i6engine::utils::make_shared<RejectComponent, i6engine::api::Component>(4, attMap);

		attMap["identifier"] = "B"; // reject
		i6engine::api::ComPtr c5 = i6engine::utils::make_shared<RejectComponent, i6engine::api::Component>(5, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(c1);
		owner->setGOC(c2);
		owner->setGOC(c3);
		owner->setGOC(c4);
		owner->setGOC(c5);

		// final form
		// 1A |
		EXPECT_EQ(c1, owner->getGOC(config::ComponentTypes::RejectComponent));
		EXPECT_EQ(c1, owner->getGOC(config::ComponentTypes::RejectComponent, "A"));
		EXPECT_EQ(c1, owner->getGOCID(1));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::RejectComponent, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::RejectComponent, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::RejectComponent, "D"));
		EXPECT_EQ(nullptr, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::RejectComponent, "E"));
		EXPECT_EQ(nullptr, owner->getGOCID(5));
	}

	TEST_F(GOTest, deleteGOC) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A";
		i6engine::api::ComPtr cAdd1 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(1, attMap);
		attMap["identifier"] = "B";
		i6engine::api::ComPtr cAdd2 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(2, attMap);
		attMap["identifier"] = "C";
		i6engine::api::ComPtr cAdd3 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(3, attMap);
		attMap["identifier"] = "D";
		i6engine::api::ComPtr cAdd4 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(4, attMap);
		attMap["identifier"] = "E";
		i6engine::api::ComPtr cAdd5 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(5, attMap);
		attMap["identifier"] = "F";
		i6engine::api::ComPtr cAdd6 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(6, attMap);
		attMap["identifier"] = "G";
		i6engine::api::ComPtr cAdd7 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(7, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(cAdd1);
		owner->setGOC(cAdd2);
		owner->setGOC(cAdd3);
		owner->setGOC(cAdd4);
		owner->setGOC(cAdd5);
		owner->setGOC(cAdd6);
		owner->setGOC(cAdd7);

		owner->deleteGOC(config::ComponentTypes::AddComponent);
		owner->deleteGOC(config::ComponentTypes::AddComponent);
		owner->deleteGOC(config::ComponentTypes::AddComponent, "E");
		owner->deleteGOC(config::ComponentTypes::AddComponent, "F");

		// final form
		// 3C | 4D 7G
		EXPECT_EQ(cAdd3, owner->getGOC(config::ComponentTypes::AddComponent));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(cAdd3, owner->getGOC(config::ComponentTypes::AddComponent, "C"));
		EXPECT_EQ(cAdd3, owner->getGOCID(3));
		EXPECT_EQ(cAdd4, owner->getGOC(config::ComponentTypes::AddComponent, "D"));
		EXPECT_EQ(cAdd4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "E"));
		EXPECT_EQ(nullptr, owner->getGOCID(5));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "F"));
		EXPECT_EQ(nullptr, owner->getGOCID(6));
		EXPECT_EQ(cAdd7, owner->getGOC(config::ComponentTypes::AddComponent, "G"));
		EXPECT_EQ(cAdd7, owner->getGOCID(7));
	}

	TEST_F(GOTest, clearGOCs) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A";
		i6engine::api::ComPtr cAdd1 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(1, attMap);
		attMap["identifier"] = "B";
		i6engine::api::ComPtr cAdd2 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(2, attMap);
		attMap["identifier"] = "C";
		i6engine::api::ComPtr cAdd3 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(3, attMap);
		attMap["identifier"] = "D";
		i6engine::api::ComPtr cAdd4 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(4, attMap);
		attMap["identifier"] = "E";
		i6engine::api::ComPtr cAdd5 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(5, attMap);
		attMap["identifier"] = "F";
		i6engine::api::ComPtr cAdd6 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(6, attMap);
		attMap["identifier"] = "G";
		i6engine::api::ComPtr cAdd7 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(7, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(cAdd1);
		owner->setGOC(cAdd2);
		owner->setGOC(cAdd3);
		owner->setGOC(cAdd4);
		owner->setGOC(cAdd5);
		owner->setGOC(cAdd6);
		owner->setGOC(cAdd7);

		owner->clearGOCs();

		// final form
		// ---
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "D"));
		EXPECT_EQ(nullptr, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "E"));
		EXPECT_EQ(nullptr, owner->getGOCID(5));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "F"));
		EXPECT_EQ(nullptr, owner->getGOCID(6));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::AddComponent, "G"));
		EXPECT_EQ(nullptr, owner->getGOCID(7));
	}

	TEST_F(GOTest, getGOC) {
		GOPtr owner = i6engine::utils::make_shared<GameObject, GameObject>(0, core::IPKey(), EngineController::GetSingleton().getUUID(), "tpl", [](const int64_t, const std::string &, const i6engine::api::attributeMap &, const WeakGOPtr &) { return i6engine::api::ComPtr(); });
		i6engine::api::attributeMap attMap;
		attMap["identifier"] = "A";
		i6engine::api::ComPtr cAdd1 = i6engine::utils::make_shared<AddComponent, i6engine::api::Component>(1, attMap);
		i6engine::api::ComPtr cAdd2 = i6engine::utils::make_shared<Replace1Component, i6engine::api::Component>(2, attMap);
		i6engine::api::ComPtr cAdd3 = i6engine::utils::make_shared<ReplaceDisComponent, i6engine::api::Component>(3, attMap);
		i6engine::api::ComPtr cAdd4 = i6engine::utils::make_shared<RejectComponent, i6engine::api::Component>(4, attMap);

		// add afterwards to avoid address conflicts due to heap reuse
		owner->setSelf(owner);
		owner->setGOC(cAdd1);
		owner->setGOC(cAdd2);
		owner->setGOC(cAdd3);
		owner->setGOC(cAdd4);

		EXPECT_NE(nullptr, owner->getGOC(config::ComponentTypes::AddComponent));
		EXPECT_EQ(nullptr, owner->getGOC(config::ComponentTypes::Replace0Component));
		EXPECT_NE(nullptr, i6engine::utils::dynamic_pointer_cast<AddComponent>(owner->getGOC(config::ComponentTypes::AddComponent)));
		EXPECT_EQ(nullptr, i6engine::utils::dynamic_pointer_cast<Replace0Component>(owner->getGOC(config::ComponentTypes::AddComponent)));
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
			EXPECT_NE(nullptr, i6engine::utils::dynamic_pointer_cast<AddComponent>(owner->getGOC(config::ComponentTypes::AddComponent)));
			EXPECT_EQ(nullptr, i6engine::utils::dynamic_pointer_cast<Replace0Component>(owner->getGOC(config::ComponentTypes::AddComponent)));
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
