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
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_COLLISIONSHAPES_MESHSTRIDERCOLLISIONSHAPE_H__
#define __I6ENGINE_MODULES_COLLISIONSHAPES_MESHSTRIDERCOLLISIONSHAPE_H__

#include <cstdint>
#include <vector>

#include "i6engine/math/i6eVector.h"

#include "i6engine/modules/physics/collisionShapes/CollisionShape.h"

#include "boost/serialization/vector.hpp"

namespace i6engine {
namespace modules {

	struct ISIXE_MODULES_API MeshStriderCollisionShapeData : public CollisionShapeData {
		std::vector<char> data;

		MeshStriderCollisionShapeData() : CollisionShapeData(), data() {
		}

		MeshStriderCollisionShapeData(const std::vector<char> & d) : CollisionShapeData(CollisionShapeType::Heightmap), data(d) {
		}

		template<class Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & boost::serialization::base_object<CollisionShapeData>(*this);
			ar & data;
		}
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COLLISIONSHAPES_MESHSTRIDERCOLLISIONSHAPE_H__ */

/**
 * @}
 */
