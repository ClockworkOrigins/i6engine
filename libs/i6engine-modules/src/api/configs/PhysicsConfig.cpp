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

#include "i6engine/api/configs/PhysicsConfig.h"

namespace i6e {
namespace api {
namespace physics {

	Physics_Collision_Update::Physics_Collision_Update() : GameMessageStruct() {}
	Physics_Collision_Update::Physics_Collision_Update(const int64_t i, const int64_t compId, const CollisionGroup & f) : GameMessageStruct(compId, i), flags(f) {
	}

	Physics_Gravity_Update::Physics_Gravity_Update() {}
	Physics_Gravity_Update::Physics_Gravity_Update(const Vec3 & v) : grav(v) {}

	Physics_Node_Create::Physics_Node_Create(const int64_t i, const int64_t ci, const Vec3 & p, const Quaternion & r, const Vec3 & s, const CollisionGroup & cg, const PhysicalStateComponent::ShapeType st, const attributeMap & pa, const ShatterInterest si) : GameMessageStruct(ci, i), pos(p), rot(r), scale(s), collisionGroup(cg), shapeType(st), shapeParams(pa), shatterInterest(si) {
	}

	Physics_Node_Update::Physics_Node_Update(const int64_t i, const int64_t ci, const Quaternion & r, const Vec3 & s) : GameMessageStruct(ci, i), rot(r), scale(s) {
	}

	Physics_NodePosition_Update::Physics_NodePosition_Update(const int64_t i, const int64_t ci, const Vec3 & p) : GameMessageStruct(ci, i), pos(p) {
	}

	Physics_Node_Delete::Physics_Node_Delete(const int64_t i, const int64_t ci) : GameMessageStruct(ci, i) {
	}

	Physics_ApplyRotation_Update::Physics_ApplyRotation_Update(const int64_t i, const int64_t ci, const Quaternion & rotation) : GameMessageStruct(ci, i), rot(rotation) {
	}

	Physics_RayTest_Update::Physics_RayTest_Update(const int64_t i, const int64_t ci, const Vec3 & f, const Vec3 & t, PhysicalStateComponent::RayTestRepetition rtr, PhysicalStateComponent::RayTestNotify rtn, const GameMessage::Ptr & msg) : GameMessageStruct(ci, i), from(f), to(t), raytestRepetition(rtr), raytestNotify(rtn), message(msg) {
	}

	Physics_RayTestNotify_Update::Physics_RayTestNotify_Update(const int64_t i, const int64_t ci) : GameMessageStruct(ci, i), raytestResult() {
	}

	Physics_AddPosition_Update::Physics_AddPosition_Update(const int64_t i, const int64_t ci, const Vec3 & p) : GameMessageStruct(ci, i), pos(p) {
	}

} /* namespace physics */
} /* namespace modules */
} /* namespace i6e */
