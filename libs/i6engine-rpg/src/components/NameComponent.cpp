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

#include "i6engine/rpg/components/NameComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	NameComponent::NameComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _name(params.find("name")->second) {
		_objFamilyID = config::ComponentTypes::NameComponent;
		_objComponentID = config::ComponentTypes::NameComponent;
	}

	api::ComPtr NameComponent::createC(int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("NameComponent", "name not set!", params.find("name") != params.end());
		return utils::make_shared<NameComponent, api::Component>(id, params);
	}

	void NameComponent::Init() {
	}

	api::attributeMap NameComponent::synchronize() const {
		api::attributeMap params;

		params.insert(std::make_pair("name", _name));

		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */