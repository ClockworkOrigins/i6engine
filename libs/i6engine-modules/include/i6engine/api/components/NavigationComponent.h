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

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_NAVIGATIONCOMPONENT_H__
#define __I6ENGINE_API_NAVIGATIONCOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class NavigationComponent
	 * \brief Used for navigation between points
	 */
	class ISIXE_MODULES_API NavigationComponent : public Component {
	public:
		NavigationComponent(const int64_t id, const attributeMap & params);
		virtual ~NavigationComponent();

		/**
		 * \brief get path from position to position
		 */
		virtual std::vector<Vec3> getPath(const Vec3 & from, const Vec3 & to) const = 0;

		/**
		 * \brief get path from position to waypoint
		 */
		virtual std::vector<Vec3> getPath(const Vec3 & from, const std::string & to) const = 0;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_NAVIGATIONCOMPONENT_H__ */

/**
 * @}
 */
