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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_COMPONENTS_BILLBOARDCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_BILLBOARDCOMPONENT_H__

#include <cstdint>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

namespace Ogre {
	class Billboard;
	class BillboardSet;
} /* namespace Ogre */

namespace i6engine {
namespace api {
namespace graphics {
	enum class BillboardOrigin;
} /* namespace graphics */
} /* namespace api */
namespace modules {

	class GraphicsManager;
	class GraphicsNode;

	/**
	 * \ingroup Graphic
	 *
	 * \class BillboardComponent
	 * \brief Handles all light related functionality
	 */
	class BillboardComponent {
		friend class GraphicsNode;

	private:
		/**
		 * \brief stores the GraphicsManager controlling this object
		 */
		GraphicsManager * _manager;

		/**
		 * parent of this Component
		 */
		GraphicsNode * _parent;

		Ogre::BillboardSet * _billboardSet;
		std::map<std::string, Ogre::Billboard *> _billboards;

		/**
		 * \brief Create a new BillboardComponent
		 */
		BillboardComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & material, double width, double height, api::graphics::BillboardOrigin bo);

		/**
		 * \brief ~BillboardComponent
		 */
		~BillboardComponent();

		/**
		 * \brief creates a billboard on the billboard set
		 */
		void createOrUpdateBillboard(const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1);

		/**
		 * \brief deletes the billboard
		 */
		void deleteBillboard(const std::string & identifier);

		/**
		 * \brief forbidden
		 */
		BillboardComponent(const BillboardComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const BillboardComponent & operator=(const BillboardComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COMPONENTS_BILLBOARDCOMPONENT_H__ */

/**
 * @}
 */
