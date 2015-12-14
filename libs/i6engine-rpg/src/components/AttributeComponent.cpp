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

#include "i6engine/rpg/components/AttributeComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	AttributeComponent::AttributeComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _attributes(size_t(Attribute::COUNT)) {
		_objFamilyID = config::ComponentTypes::AttributeComponent;
		_objComponentID = config::ComponentTypes::AttributeComponent;

		_attributes[0] = std::stoi(params.find("attribute_0")->second);
		_attributes[1] = std::stoi(params.find("attribute_1")->second);
	}

	api::ComPtr AttributeComponent::createC(int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("AttributeComponent", "attribute_0 not set!", params.find("attribute_0") != params.end());
		ISIXE_THROW_API_COND("AttributeComponent", "attribute_1 not set!", params.find("attribute_1") != params.end());
		return utils::make_shared<AttributeComponent, api::Component>(id, params);
	}

	void AttributeComponent::Init() {
	}

	api::attributeMap AttributeComponent::synchronize() const {
		api::attributeMap params;

		for (size_t i = 0; i < _attributes.size(); i++) {
			params.insert(std::make_pair("attribute_" + std::to_string(i), std::to_string(_attributes[i])));
		}

		return params;
	}

	void AttributeComponent::changeAttribute(Attribute attribute, int32_t diff) {
		_attributes[size_t(attribute)] = _attributes[size_t(attribute)] + diff;
	}

	void AttributeComponent::registerListener(Attribute attribute, const std::function<void(int32_t)> & func) {
		_attributes[size_t(attribute)].registerUpdate(func);
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
