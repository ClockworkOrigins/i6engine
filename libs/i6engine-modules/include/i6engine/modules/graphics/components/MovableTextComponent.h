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

#ifndef __I6ENGINE_MODULES_COMPONENTS_MOVABLETEXTCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_MOVABLETEXTCOMPONENT_H__

#include <cstdint>
#include <functional>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

namespace i6e {
namespace modules {

	class GraphicsManager;
	class GraphicsNode;
	class MeshComponent;
	class MovableText;

	/**
	 * \ingroup Graphic
	 *
	 * \class MovableTextComponent
	 * \brief Handles all light related functionality
	 */
	class MovableTextComponent {
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

		/**
		 * SceneNode of the object
		 */
		MovableText * _movableText;

		int64_t _id;

		double _size;

		std::function<double(const Vec3 &, const Vec3 &)> _autoScaleCallback;

		/**
		 * \brief Create a new MovableTextComponent
		 */
		MovableTextComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & font, const std::string & text, double size, const Vec3 & colour, const Vec3 & position);

		/**
		 * \brief ~MovableTextComponent
		 */
		~MovableTextComponent();

		/**
		 * \brief updates settings of the movable text
		 */
		void updateMovableText(const std::string & font, const std::string & text, double size, const Vec3 & colour);

		void setAutoScaleCallback(const std::function<double(const Vec3 &, const Vec3 &)> & callback);

		void Tick();

		/**
		 * \brief forbidden
		 */
		MovableTextComponent(const MovableTextComponent &) = delete;

		/**
		 * \brief forbidden
		 */
		const MovableTextComponent & operator=(const MovableTextComponent &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COMPONENTS_MOVABLETEXTCOMPONENT_H__ */

/**
 * @}
 */
