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

#ifndef __I6ENGINE_MODULES_COLLISIONSHAPE_COLLISIONSHAPE_H__
#define __I6ENGINE_MODULES_COLLISIONSHAPE_COLLISIONSHAPE_H__

#include "i6engine/utils/i6eSystemParameters.h"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/export.hpp"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_COLLISIONSHAPE_COLLISIONSHAPE_H__ */

/**
 * @}
 */
