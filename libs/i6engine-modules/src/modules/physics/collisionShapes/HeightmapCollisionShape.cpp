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

#include "i6engine/modules/physics/collisionShapes/HeightmapCollisionShape.h"

namespace i6engine {
namespace modules {

	HeightmapCollisionShapeData::HeightmapCollisionShapeData() : CollisionShapeData(), width(), height(), data(), minHeight(), maxHeight(), scaling() {
	}

	HeightmapCollisionShapeData::HeightmapCollisionShapeData(uint32_t w, uint32_t h, const std::vector<double> & d, double minH, double maxH, const Vec3 & s) : CollisionShapeData(CollisionShapeType::Heightmap), width(w), height(h), data(d), minHeight(minH), maxHeight(maxH), scaling(s) {
	}

} /* namespace modules */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_GUID(i6engine::modules::HeightmapCollisionShapeData, "CS1")
BOOST_CLASS_IMPLEMENTATION(i6engine::modules::HeightmapCollisionShapeData, boost::serialization::object_serializable)