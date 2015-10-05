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

#include "i6engine/modules/physics/collisionShapes/MeshStriderCollisionShape.h"

namespace i6engine {
namespace modules {

	MeshStriderCollisionShapeData::MeshStriderCollisionShapeData() : CollisionShapeData(), data() {
	}

	MeshStriderCollisionShapeData::MeshStriderCollisionShapeData(const std::vector<char> & d) : CollisionShapeData(CollisionShapeType::MeshStrider), data(d) {
	}

} /* namespace modules */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_GUID(i6engine::modules::MeshStriderCollisionShapeData, "CS2")
BOOST_CLASS_IMPLEMENTATION(i6engine::modules::MeshStriderCollisionShapeData, boost::serialization::object_serializable)
