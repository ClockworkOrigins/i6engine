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
#include "i6engine/api/components/VelocityComponent.h"

namespace i6e {
namespace api {
namespace physics {

	enum PhysicsMessageTypes {
		PhyReset,
		PhyCollision,
		PhyGravity,
		PhyNode,
		PhyNodeGravity,
		PhyNodePosition,
		PhyLinearVelocity,
		PhyForce,
		PhyVelForce,
		PhyApplyRotation,
		PhyResetForces,
		PhyRayTest,
		PhyRayTestResult,
		PhyPause,
		PhyAddPosition,
		PhyP2PConstraint,
		PhyConstraint,
		PhyConstraintBreakImpulse,
		PhyVelocityComponent,
		PhyAccelerate,
		PhyDecelerate,
		PhyMaxSpeed,
		PhyResistanceCoefficient,
		PhyWindage,
		PhyStopAcceleration
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
		Physics_P2PConstraint_Create(const int64_t goid, const int64_t targetGoid, const Vec3 & s, const Vec3 & t) : GameMessageStruct(-1, goid), targetGOID(targetGoid), selfOffset(s), targetOffset(t) {
		}
		Physics_P2PConstraint_Create * copy() {
			return new Physics_P2PConstraint_Create(*this);
		}
	} Physics_P2PConstraint_Create;

	/**
	 * \brief message for setting breaking impulse for Constraint between two PhysicsNodes
	 */
	typedef struct Physics_BreakConstraintImpulse_Update : GameMessageStruct {
		int64_t targetGOID;
		double breakingImpulse;
		Physics_BreakConstraintImpulse_Update(const int64_t goid, const int64_t targetGoid, double bi) : GameMessageStruct(-1, goid), targetGOID(targetGoid), breakingImpulse(bi) {
		}
		Physics_BreakConstraintImpulse_Update * copy() {
			return new Physics_BreakConstraintImpulse_Update(*this);
		}
	} Physics_BreakConstraintImpulse_Update;

	/**
	 * \brief message for deletion of a Constraint between two PhysicsNodes
	 */
	typedef struct Physics_Constraint_Delete : GameMessageStruct {
		int64_t targetGOID;
		Physics_Constraint_Delete(const int64_t goid, const int64_t targetGoid) : GameMessageStruct(-1, goid), targetGOID(targetGoid) {
		}
		Physics_Constraint_Delete * copy() {
			return new Physics_Constraint_Delete(*this);
		}
	} Physics_Constraint_Delete;

	/**
	 * \brief message for creation of a VelocityComponent in Physics
	 */
	typedef struct Physics_VelocityComponent_Create : GameMessageStruct {
		double maxSpeed;
		double resistanceCoefficient;
		double windage;
		Physics_VelocityComponent_Create(const int64_t coid, const int64_t goid, double ms, double rc, double w) : GameMessageStruct(coid, goid), maxSpeed(ms), resistanceCoefficient(rc), windage(w) {
		}
		Physics_VelocityComponent_Create * copy() {
			return new Physics_VelocityComponent_Create(*this);
		}
	} Physics_VelocityComponent_Create;

	/**
	 * \brief message for deletion of a VelocityComponent in Physics
	 */
	typedef struct Physics_VelocityComponent_Delete : GameMessageStruct {
		Physics_VelocityComponent_Delete(const int64_t coid, const int64_t goid) : GameMessageStruct(coid, goid) {
		}
		Physics_VelocityComponent_Delete * copy() {
			return new Physics_VelocityComponent_Delete(*this);
		}
	} Physics_VelocityComponent_Delete;

	/**
	 * \brief message for acceleration of VelocityComponent
	 */
	typedef struct Physics_Accelerate_Update : GameMessageStruct {
		Vec3 acceleration;
		VelocityComponent::MaxSpeedHandling handling;
		std::function<void(void)> callback;
		Physics_Accelerate_Update(const int64_t coid, const int64_t goid, const Vec3 & a, VelocityComponent::MaxSpeedHandling h, const std::function<void(void)> & cb) : GameMessageStruct(coid, goid), acceleration(a), handling(h), callback(cb) {
		}
		Physics_Accelerate_Update * copy() {
			return new Physics_Accelerate_Update(*this);
		}
	} Physics_Accelerate_Update;

	/**
	 * \brief message for deceleration of VelocityComponent
	 */
	typedef struct Physics_Decelerate_Update : GameMessageStruct {
		Vec3 deceleration;
		VelocityComponent::DecelerationHandling handling;
		std::function<void(void)> callback;
		Physics_Decelerate_Update(const int64_t coid, const int64_t goid, const Vec3 & d, VelocityComponent::DecelerationHandling h, const std::function<void(void)> & cb) : GameMessageStruct(coid, goid), deceleration(d), handling(h), callback(cb) {
		}
		Physics_Decelerate_Update * copy() {
			return new Physics_Decelerate_Update(*this);
		}
	} Physics_Decelerate_Update;

	/**
	 * \brief message for stopping acceleration of VelocityComponent
	 */
	typedef struct Physics_StopAcceleration_Update : GameMessageStruct {
		Physics_StopAcceleration_Update(const int64_t coid, const int64_t goid) : GameMessageStruct(coid, goid) {
		}
		Physics_StopAcceleration_Update * copy() {
			return new Physics_StopAcceleration_Update(*this);
		}
	} Physics_StopAcceleration_Update;

	/**
	 * \brief message for setting maxSpeed of a VelocityComponent
	 */
	typedef struct Physics_SetMaxSpeed_Update : GameMessageStruct {
		double maxSpeed;
		Physics_SetMaxSpeed_Update(const int64_t coid, const int64_t goid, double ms) : GameMessageStruct(coid, goid), maxSpeed(ms) {
		}
		Physics_SetMaxSpeed_Update * copy() {
			return new Physics_SetMaxSpeed_Update(*this);
		}
	} Physics_SetMaxSpeed_Update;

	/**
	 * \brief message for creation of a VelocityComponent in Physics
	 */
	typedef struct Physics_SetResistanceCoefficient_Update : GameMessageStruct {
		double resistanceCoefficient;
		Physics_SetResistanceCoefficient_Update(const int64_t coid, const int64_t goid, double rc) : GameMessageStruct(coid, goid), resistanceCoefficient(rc) {
		}
		Physics_SetResistanceCoefficient_Update * copy() {
			return new Physics_SetResistanceCoefficient_Update(*this);
		}
	} Physics_SetResistanceCoefficient_Update;

	/**
	 * \brief message for creation of a VelocityComponent in Physics
	 */
	typedef struct Physics_SetWindage_Update : GameMessageStruct {
		double windage;
		Physics_SetWindage_Update(const int64_t coid, const int64_t goid, double w) : GameMessageStruct(coid, goid), windage(w) {
		}
		Physics_SetWindage_Update * copy() {
			return new Physics_SetWindage_Update(*this);
		}
	} Physics_SetWindage_Update;

} /* namespace physics */
} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_PHYSICSCONFIG_H__ */

/**
 * @}
 */
