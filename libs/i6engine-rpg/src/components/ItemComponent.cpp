/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/rpg/components/ItemComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/rpg/components/Config.h"

namespace i6e {
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
} /* namespace i6e */
