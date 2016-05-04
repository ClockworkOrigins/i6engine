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

#ifndef __I6ENGINE_MODULES_COMPONENTS_MOVABLETEXTCOMPONENT_H__
#define __I6ENGINE_MODULES_COMPONENTS_MOVABLETEXTCOMPONENT_H__

#include <cstdint>

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

		/**
		 * \brief Create a new MovableTextComponent
		 */
		MovableTextComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & font, const std::string & text, double size, const Vec3 & colour);

		/**
		 * \brief ~MovableTextComponent
		 */
		~MovableTextComponent();

		/**
		 * \brief updates settings of the movable text
		 */
		void updateMovableText(const std::string & font, const std::string & text, double size, const Vec3 & colour);

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
