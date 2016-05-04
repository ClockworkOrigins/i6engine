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
