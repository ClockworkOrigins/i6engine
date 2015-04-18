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

#include "i6engine/api/configs/PhysicsConfig.h"

namespace i6engine {
namespace api {
namespace physics {

	Physics_Collision_Update::Physics_Collision_Update() : GameMessageStruct() {}
	Physics_Collision_Update::Physics_Collision_Update(const int64_t i, const int64_t compId, const CollisionGroup & f) : GameMessageStruct(compId, i), flags(f) {
	}

	Physics_Gravity_Update::Physics_Gravity_Update() {}
	Physics_Gravity_Update::Physics_Gravity_Update(const Vec3 & v) : grav(v) {}

	Physics_Clean_Delete::Physics_Clean_Delete() {}

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
} /* namespace i6engine */
