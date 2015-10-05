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

#ifndef __I6ENGINE_MODULES_COLLISIONSHAPE_COLLISIONSHAPE_H__
#define __I6ENGINE_MODULES_COLLISIONSHAPE_COLLISIONSHAPE_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/export.hpp"

namespace i6engine {
namespace modules {

	enum class CollisionShapeType {
		None,
		Heightmap,
		MeshStrider
	};

	struct ISIXE_MODULES_API CollisionShapeData {
		CollisionShapeType type;

		CollisionShapeData() : type(CollisionShapeType::None) {
		}

		CollisionShapeData(CollisionShapeType t) : type(t) {
		}

		virtual ~CollisionShapeData() {
		}

		std::string Serialize() const;

		static CollisionShapeData * Deserialize(const std::string & serialized);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & type;
		}
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_COLLISIONSHAPE_COLLISIONSHAPE_H__ */

/**
 * @}
 */
