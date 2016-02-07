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

	ItemComponent::ItemComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _value(), _imageset(), _image(), _weight(0.0), _stackable(false) {
		parseAttribute<true>(params, "ident", _identifier);
		parseAttribute<true>(params, "value", _value);
		parseAttribute<true>(params, "imageset", _imageset);
		parseAttribute<true>(params, "image", _image);
		parseAttribute<false>(params, "weight", _weight);
		parseAttribute<false>(params, "stackable", _stackable);

		_objFamilyID = config::ComponentTypes::ItemComponent;
	}

	api::attributeMap ItemComponent::synchronize() const {
		api::attributeMap params;

		params.insert(std::make_pair("ident", _identifier));
		params.insert(std::make_pair("value", std::to_string(_value)));
		params.insert(std::make_pair("imageset", _imageset));
		params.insert(std::make_pair("image", _image));
		params.insert(std::make_pair("weight", std::to_string(_weight)));
		params.insert(std::make_pair("stackable", std::to_string(_stackable)));

		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
