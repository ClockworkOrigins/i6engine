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

#ifndef __I6ENGINE_API_PHYSICSCONFIG_H__
#define __I6ENGINE_API_PHYSICSCONFIG_H__

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/components/PhysicalStateComponent.h"

namespace i6engine {
namespace api {
namespace physics {
	enum PhysicsMessageTypes {
		PhyReset,
		PhyCollision,
		PhyGravity,
		PhyNode,
		PhyNodeGravity,
		PhyNodePosition,
		PhyClean,
		PhyLinearVelocity,
		PhyForce,
		PhyVelForce,
		PhyApplyRotation,
		PhyResetForces,
		PhyRayTest,
		PhyRayTestResult,
		PhyPause,
		PhyAddPosition,
		PhyP2PConstraint
	};

	/**
	 * \brief sends update of collision flags vom PhysicalStateComponent to PhysicsNode
	 */
	typedef struct Physics_Collision_Update : GameMessageStruct {
		CollisionGroup flags;
		Physics_Collision_Update();
		Physics_Collision_Update(const int64_t i, const int64_t compId, const CollisionGroup & f);
		Physics_Collision_Update * copy() { return new Physics_Collision_Update(*this); }
	} Physics_Collision_Update;

	/**
	 * \brief sends update for world gravity
	 */
	typedef struct Physics_Gravity_Update : GameMessageStruct {
		Vec3 grav;
		Physics_Gravity_Update();
		Physics_Gravity_Update(const Vec3 & v);
		Physics_Gravity_Update * copy() { return new Physics_Gravity_Update(*this); }
	} Physics_Gravity_Update;

	/**
	 * \brief cleans up all physics objects
	 */
	typedef struct Physics_Clean_Delete : GameMessageStruct {
		Physics_Clean_Delete();
		Physics_Clean_Delete * copy() { return new Physics_Clean_Delete(*this); }
	} Physics_Clean_Delete;

	/**
	 * \brief message for creation of new PhysicsNode
	 */
	typedef struct Physics_Node_Create : GameMessageStruct {
		Vec3 pos;
		Quaternion rot;
		Vec3 scale;
		CollisionGroup collisionGroup;
		PhysicalStateComponent::ShapeType shapeType;
		attributeMap shapeParams;
		ShatterInterest shatterInterest;

		Physics_Node_Create(const int64_t i, const int64_t ci, const Vec3 & p, const Quaternion & r, const Vec3 & s, const CollisionGroup & cg, const PhysicalStateComponent::ShapeType st, const attributeMap & pa, const ShatterInterest si);
		Physics_Node_Create * copy() { return new Physics_Node_Create(*this); }
	} Physics_Node_Create;

	/**
	 * \brief message for creation of new PhysicsNode
	 */
	typedef struct Physics_Node_Update : GameMessageStruct {
		Quaternion rot;
		Vec3 scale;
		Physics_Node_Update(const int64_t i, const int64_t ci, const Quaternion & r, const Vec3 & s);
		Physics_Node_Update * copy() { return new Physics_Node_Update(*this); }
	} Physics_Node_Update;

	/**
	 * \brief message for update of PhysicsNode position
	 */
	typedef struct Physics_NodePosition_Update : GameMessageStruct {
		Vec3 pos;
		Physics_NodePosition_Update(const int64_t i, const int64_t ci, const Vec3 & p);
		Physics_NodePosition_Update * copy() { return new Physics_NodePosition_Update(*this); }
	} Physics_NodePosition_Update;

	/**
	 * \brief sets the linear velocity of a PhysicsNode
	 */
	typedef struct Physics_LinearVelocity_Update : GameMessageStruct {
		Vec3 linVel;
		Physics_LinearVelocity_Update(const int64_t i, const int64_t ci, const Vec3 & v) : GameMessageStruct(ci, i), linVel(v) {
		}
		Physics_LinearVelocity_Update * copy() { return new Physics_LinearVelocity_Update(*this); }
	} Physics_LinearVelocity_Update;

	/**
	 * \brief applies a central impulse for a PhysicsNode
	 */
	typedef std::tuple<Vec3, Vec3, bool> Force;
	typedef struct Physics_Force_Update : GameMessageStruct {
		std::vector<Force> forces;
		Physics_Force_Update(const int64_t i, const int64_t ci, const std::vector<Force> & fs) : GameMessageStruct(ci, i), forces(fs) {
		}
		Physics_Force_Update * copy() { return new Physics_Force_Update(*this); }
	} Physics_Force_Update;

	/**
	 * \brief sends message to PhysicalStateComponent with updates of velocity and impulse
	 */
	typedef struct Physics_Comp_Update : GameMessageStruct {
		Vec3 linVel;
		Vec3 centForce;
		Physics_Comp_Update(const int64_t i, const int64_t ci, const Vec3 & v, const Vec3 & f) : GameMessageStruct(ci, i), linVel(v), centForce(f) {
		}
		Physics_Comp_Update * copy() { return new Physics_Comp_Update(*this); }
	} Physics_Comp_Update;

	/**
	 * \brief applies new rotation to PhysicsNode
	 */
	typedef struct Physics_ApplyRotation_Update : GameMessageStruct {
		Quaternion rot;
		Physics_ApplyRotation_Update(const int64_t i, const int64_t ci, const Quaternion & rotation);
		Physics_ApplyRotation_Update * copy() { return new Physics_ApplyRotation_Update(*this); }
	} Physics_ApplyRotation_Update;

	/**
	 * \brief removes all powers from a PhysicsNode
	 */
	typedef struct Physics_Reset_Update : GameMessageStruct {
		Physics_Reset_Update(const int64_t i, const int64_t ci) : GameMessageStruct(ci, i) {
		}
		Physics_Reset_Update * copy() { return new Physics_Reset_Update(*this); }
	} Physics_Reset_Update;

	/**
	 * \brief sets the gravity for the given object
	 * This overrides the default gravity, thus (0, 0, 0) will result in no gravity at all
	 */
	typedef struct Physics_NodeGravity_Update : GameMessageStruct {
		Vec3 gravity;
		Physics_NodeGravity_Update(const int64_t i, const int64_t ci, const Vec3 & grav) : GameMessageStruct(ci, i), gravity(grav) {
		}
		Physics_NodeGravity_Update * copy() { return new Physics_NodeGravity_Update(*this); }
	} Physics_NodeGravity_Update;

	/**
	 * \brief message for deletion of new PhysicsNode
	 */
	typedef struct Physics_Node_Delete : GameMessageStruct {
		Physics_Node_Delete(const int64_t i, const int64_t ci);
		Physics_Node_Delete * copy() { return new Physics_Node_Delete(*this); }
	} Physics_Node_Delete;

	/**
	 * \brief message for raytest
	 */
	typedef struct Physics_RayTest_Update : GameMessageStruct {
		Vec3 from;
		Vec3 to;
		PhysicalStateComponent::RayTestRepetition raytestRepetition;
		PhysicalStateComponent::RayTestNotify raytestNotify;
		GameMessage::Ptr message;

		Physics_RayTest_Update(const int64_t i, const int64_t ci, const Vec3 & f, const Vec3 & t, PhysicalStateComponent::RayTestRepetition rtr, PhysicalStateComponent::RayTestNotify rtn, const GameMessage::Ptr & msg);
		Physics_RayTest_Update * copy() { return new Physics_RayTest_Update(*this); }
	} Physics_RayTest_Update;

	/**
	 * \brief message for raytest feedback
	 */
	typedef struct ISIXE_MODULES_API Physics_RayTestNotify_Update : GameMessageStruct {
		RayTestResult raytestResult;

		Physics_RayTestNotify_Update(const int64_t i, const int64_t ci);
		Physics_RayTestNotify_Update * copy() { return new Physics_RayTestNotify_Update(*this); }
	} Physics_RayTestNotify_Update;

	/**
	 * \brief pauses the subsystem
	 * never send through network
	 */
	typedef struct ISIXE_MODULES_API Physics_Pause_Update : GameMessageStruct {
		bool pause;
		Physics_Pause_Update(bool p) : pause(p) {
		}
		Physics_Pause_Update * copy() {
			return new Physics_Pause_Update(*this);
		}
	} Physics_Pause_Update;

	/**
	 * \brief message for addition of object position (cumulative)
	 */
	typedef struct Physics_AddPosition_Update : GameMessageStruct {
		Vec3 pos;
		Physics_AddPosition_Update(const int64_t i, const int64_t ci, const Vec3 & p);
		Physics_AddPosition_Update * copy() {
			return new Physics_AddPosition_Update(*this);
		}
	} Physics_AddPosition_Update;

	/**
	 * \brief message for creation of Point2PointConstraint between two PhysicsNodes
	 */
	typedef struct Physics_P2PConstraint_Create : GameMessageStruct {
		int64_t targetGOID;
		Vec3 selfOffset;
		Vec3 targetOffset;
		Physics_P2PConstraint_Create(const int64_t goid, const int64_t targetGoid, const Vec3 & s, const Vec3 & t) : GameMessageStruct(-1, goid), targetGOID(targetGOID), selfOffset(s), targetOffset(t) {
		}
		Physics_P2PConstraint_Create * copy() {
			return new Physics_P2PConstraint_Create(*this);
		}
	} Physics_P2PConstraint_Create;

	/**
	 * \brief message for deletion of Point2PointConstraint between two PhysicsNodes
	 */
	typedef struct Physics_P2PConstraint_Delete : GameMessageStruct {
		int64_t targetGOID;
		Physics_P2PConstraint_Delete(const int64_t goid, const int64_t targetGoid) : GameMessageStruct(-1, goid), targetGOID(targetGOID) {
		}
		Physics_P2PConstraint_Delete * copy() {
			return new Physics_P2PConstraint_Delete(*this);
		}
	} Physics_P2PConstraint_Delete;

} /* namespace physics */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_PHYSICSCONFIG_H__ */

/**
 * @}
 */
