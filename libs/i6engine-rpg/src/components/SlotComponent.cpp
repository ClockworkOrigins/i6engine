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

#include "i6engine/rpg/components/SlotComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	SlotComponent::SlotComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _width(), _height() {
		_objFamilyID = config::ComponentTypes::SlotComponent;
		_objComponentID = config::ComponentTypes::SlotComponent;

		api::parseAttribute<true>(params, "width", _width);
		api::parseAttribute<true>(params, "height", _height);
	}

	api::ComPtr SlotComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<SlotComponent, api::Component>(id, params);
	}

	void SlotComponent::Init() {
	}

	api::attributeMap SlotComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "width", _width);
		api::writeAttribute(params, "height", _height);
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
