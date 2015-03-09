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

#include "i6engine/api/components/Component.h"

#include "boost/bind.hpp"

#include "gtest/gtest.h"

namespace i6Test {

	class RegistrationTestComponent : public i6engine::api::Component {
	public:
		RegistrationTestComponent(int64_t id, const i6engine::api::attributeMap & params) : i6engine::api::Component(id, params) {
		}

		~RegistrationTestComponent() {
		}

		static i6engine::api::ComPtr createC(const int64_t id, const i6engine::api::attributeMap & params) {
			return i6engine::utils::make_shared<RegistrationTestComponent, i6engine::api::Component>(id, params);
		}

		void Init() override {
		}

		i6engine::api::attributeMap synchronize() override {
			return i6engine::api::attributeMap();
		}

		virtual std::pair<i6engine::api::AddStrategy, int64_t> howToAdd(const i6engine::api::ComPtr & comp) const override {
			return std::make_pair(i6engine::api::AddStrategy::ADD, 0);
		}

		std::string getTemplateName() const override {
			return "RegistrationTest";
		}

		std::vector<i6engine::api::componentOptions> getComponentOptions() override {
			std::vector<i6engine::api::componentOptions> result;
			return result;
		}
	};

} /* namespace i6Test */

TEST(Component, Registration) {
	EXPECT_FALSE(i6engine::api::Component::componentCallbacks.empty());
	EXPECT_GT(i6engine::api::Component::componentCallbacks.size(), 1);
	bool found = false;
	for (auto cc : i6engine::api::Component::componentCallbacks) {
		if (cc._name == "RegistrationTest") {
			found = true;
			break;
		}
	}
	EXPECT_TRUE(found);
}

REGISTERCOMPONENT(i6Test::RegistrationTestComponent, RegistrationTestComponent)
