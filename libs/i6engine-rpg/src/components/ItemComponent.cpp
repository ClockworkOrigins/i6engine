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
		api::parseAttribute<true>(params, "ident", _identifier);
		api::parseAttribute<true>(params, "value", _value);
		api::parseAttribute<true>(params, "imageset", _imageset);
		api::parseAttribute<true>(params, "image", _image);
		api::parseAttribute<false>(params, "weight", _weight);
		api::parseAttribute<false>(params, "stackable", _stackable);

		_objFamilyID = config::ComponentTypes::ItemComponent;
	}

	api::attributeMap ItemComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "ident", _identifier);
		api::writeAttribute(params, "value", _value);
		api::writeAttribute(params, "imageset", _imageset);
		api::writeAttribute(params, "image", _image);
		if (std::abs(_weight) > DBL_EPSILON) {
			api::writeAttribute(params, "weight", _weight);
		}
		if (_stackable) {
			api::writeAttribute(params, "stackable", _stackable);
		}
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
