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
 * \addtogroup Physics
 * @{
 */

#ifndef __I6ENGINE_MODULES_COLLISIONSHAPES_HEIGHTMAPCOLLISIONSHAPE_H__
#define __I6ENGINE_MODULES_COLLISIONSHAPES_HEIGHTMAPCOLLISIONSHAPE_H__

#include <cstdint>
#include <vector>

#include "i6engine/math/i6eVector.h"

#include "i6engine/modules/physics/collisionShapes/CollisionShape.h"

#include "boost/serialization/vector.hpp"

namespace i6e {
namespace modules {

	struct ISIXE_MODULES_API HeightmapCollisionShapeData : public CollisionShapeData {
		uint32_t width;
		uint32_t height;
		std::vector<double> data;
		double minHeight;
		double maxHeight;
		Vec3 scaling;

		HeightmapCollisionShapeData();

		HeightmapCollisionShapeData(uint32_t w, uint32_t h, const std::vector<double> & d, double minH, double maxH, const Vec3 & s);

		template<class Archive>
		void serialize(Archive & ar, const unsigned int /*version*/) {
			ar & boost::serialization::base_object<CollisionShapeData>(*this);
			ar & width;
			ar & height;
			ar & data;
			ar & minHeight;
			ar & maxHeight;
			ar & scaling;
		}
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COLLISIONSHAPES_HEIGHTMAPCOLLISIONSHAPE_H__ */

/**
 * @}
 */
