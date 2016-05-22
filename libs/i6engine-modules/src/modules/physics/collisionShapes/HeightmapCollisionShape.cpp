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

#include "i6engine/modules/physics/collisionShapes/HeightmapCollisionShape.h"

namespace i6e {
namespace modules {

	HeightmapCollisionShapeData::HeightmapCollisionShapeData() : CollisionShapeData(), width(), height(), data(), minHeight(), maxHeight(), scaling() {
	}

	HeightmapCollisionShapeData::HeightmapCollisionShapeData(uint32_t w, uint32_t h, const std::vector<double> & d, double minH, double maxH, const Vec3 & s) : CollisionShapeData(CollisionShapeType::Heightmap), width(w), height(h), data(d), minHeight(minH), maxHeight(maxH), scaling(s) {
	}

} /* namespace modules */
} /* namespace i6e */

BOOST_CLASS_EXPORT_GUID(i6e::modules::HeightmapCollisionShapeData, "CS1")
BOOST_CLASS_IMPLEMENTATION(i6e::modules::HeightmapCollisionShapeData, boost::serialization::object_serializable)
