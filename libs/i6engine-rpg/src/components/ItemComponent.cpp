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

#include "i6engine/rpg/components/ItemComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6engine {
namespace rpg {
namespace components {

	ItemComponent::ItemComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _value(), _imageset(), _image(), _weight(0.0) {
		ISIXE_THROW_API_COND("ItemComponent", "value not set!", params.find("value") != params.end());
		ISIXE_THROW_API_COND("ItemComponent", "imageset not set!", params.find("imageset") != params.end());
		ISIXE_THROW_API_COND("ItemComponent", "image not set!", params.find("image") != params.end());
		_value = std::stoul(params.find("value")->second.c_str());
		_imageset = params.find("imageset")->second;
		_image = params.find("image")->second;

		if (params.find("weight") != params.end()) {
			_weight = std::stod(params.find("weight")->second);
		}

		_objFamilyID = config::ComponentTypes::ItemComponent;
	}

	api::attributeMap ItemComponent::synchronize() const {
		api::attributeMap params;

		params.insert(std::make_pair("value", std::to_string(_value)));
		params.insert(std::make_pair("imageset", _imageset));
		params.insert(std::make_pair("image", _image));
		params.insert(std::make_pair("weight", std::to_string(_weight)));

		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
