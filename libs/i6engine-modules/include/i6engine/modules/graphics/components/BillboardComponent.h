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

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_BILLBOARDCOMPONENT_H__ */

/**
 * @}
 */
