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

#include <algorithm>

#include "i6engine/core/messaging/IPKey.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"
#include "i6engine/api/components/MoverCircleComponent.h"
#include "i6engine/api/components/PhysicalStateComponent.h"

#include "gtest/gtest.h"

using namespace i6engine;
using namespace i6engine::api;

namespace Test_GameObject {

	class TestComponent : public Component {
	public:
		TestComponent(const int64_t id, const api::attributeMap & params) : Component(id, params) {
			_objFamilyID = 0;
		}
		virtual ~TestComponent() {
		}
		void Init() {}
		api::attributeMap synchronize() { return api::attributeMap(); }

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
		}
		// always replace dispatcher
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & c) const {
			return std::make_pair(AddStrategy::REPLACE_DIS, 0);
		}
	};

	class RejectComponent : public TestComponent {
	public:
		RejectComponent(const int64_t id, const api::attributeMap & params) : TestComponent(id, params) {
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
		EXPECT_EQ(cAdd1, owner->getGOC(0));
		EXPECT_EQ(cAdd1, owner->getGOC(0, "A"));
		EXPECT_EQ(cAdd1, owner->getGOCID(1));
		EXPECT_EQ(cAdd2, owner->getGOC(0, "B"));
		EXPECT_EQ(cAdd2, owner->getGOCID(2));
		EXPECT_EQ(cAdd3, owner->getGOC(0, "C"));
		EXPECT_EQ(cAdd3, owner->getGOCID(3));
		EXPECT_EQ(cAdd4, owner->getGOC(0, "D"));
		EXPECT_EQ(cAdd4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
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
		EXPECT_EQ(c5, owner->getGOC(0));
		EXPECT_EQ(nullptr, owner->getGOC(0, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(c5, owner->getGOC(0, "B"));
		EXPECT_EQ(c2, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(0, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(c4, owner->getGOC(0, "D"));
		EXPECT_EQ(c4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
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
		EXPECT_EQ(c1, owner->getGOC(0));
		EXPECT_EQ(c1, owner->getGOC(0, "A"));
		EXPECT_EQ(c1, owner->getGOCID(1));
		EXPECT_EQ(c5, owner->getGOC(0, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(0, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(c4, owner->getGOC(0, "D"));
		EXPECT_EQ(c4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
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
		EXPECT_EQ(c5, owner->getGOC(0));
		EXPECT_EQ(nullptr, owner->getGOC(0, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(c5, owner->getGOC(0, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(0, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(nullptr, owner->getGOC(0, "D"));
		EXPECT_EQ(nullptr, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
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
		EXPECT_EQ(c1, owner->getGOC(0));
		EXPECT_EQ(c1, owner->getGOC(0, "A"));
		EXPECT_EQ(c1, owner->getGOCID(1));
		EXPECT_EQ(nullptr, owner->getGOC(0, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(0, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(nullptr, owner->getGOC(0, "D"));
		EXPECT_EQ(nullptr, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
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

		owner->deleteGOC(0);
		owner->deleteGOC(0);
		owner->deleteGOC(0, "E");
		owner->deleteGOC(0, "F");

		// final form
		// 3C | 4D 7G
		EXPECT_EQ(cAdd3, owner->getGOC(0));
		EXPECT_EQ(nullptr, owner->getGOC(0, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(nullptr, owner->getGOC(0, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(cAdd3, owner->getGOC(0, "C"));
		EXPECT_EQ(cAdd3, owner->getGOCID(3));
		EXPECT_EQ(cAdd4, owner->getGOC(0, "D"));
		EXPECT_EQ(cAdd4, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
		EXPECT_EQ(nullptr, owner->getGOCID(5));
		EXPECT_EQ(nullptr, owner->getGOC(0, "F"));
		EXPECT_EQ(nullptr, owner->getGOCID(6));
		EXPECT_EQ(cAdd7, owner->getGOC(0, "G"));
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
		EXPECT_EQ(nullptr, owner->getGOC(0));
		EXPECT_EQ(nullptr, owner->getGOC(0, "A"));
		EXPECT_EQ(nullptr, owner->getGOCID(1));
		EXPECT_EQ(nullptr, owner->getGOC(0, "B"));
		EXPECT_EQ(nullptr, owner->getGOCID(2));
		EXPECT_EQ(nullptr, owner->getGOC(0, "C"));
		EXPECT_EQ(nullptr, owner->getGOCID(3));
		EXPECT_EQ(nullptr, owner->getGOC(0, "D"));
		EXPECT_EQ(nullptr, owner->getGOCID(4));
		EXPECT_EQ(nullptr, owner->getGOC(0, "E"));
		EXPECT_EQ(nullptr, owner->getGOCID(5));
		EXPECT_EQ(nullptr, owner->getGOC(0, "F"));
		EXPECT_EQ(nullptr, owner->getGOCID(6));
		EXPECT_EQ(nullptr, owner->getGOC(0, "G"));
		EXPECT_EQ(nullptr, owner->getGOCID(7));
	}
}
