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

#include "i6engine/modules/physics/collisionShapes/MeshStriderCollisionShape.h"

namespace i6e {
namespace modules {

	MeshStriderCollisionShapeData::MeshStriderCollisionShapeData() : CollisionShapeData(), data() {
	}

	MeshStriderCollisionShapeData::MeshStriderCollisionShapeData(const std::vector<char> & d) : CollisionShapeData(CollisionShapeType::MeshStrider), data(d) {
	}

} /* namespace modules */
} /* namespace i6e */

BOOST_CLASS_EXPORT_GUID(i6e::modules::MeshStriderCollisionShapeData, "CS2")
BOOST_CLASS_IMPLEMENTATION(i6e::modules::MeshStriderCollisionShapeData, boost::serialization::object_serializable)
