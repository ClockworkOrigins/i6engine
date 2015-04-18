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

#ifndef __I6ENGINE_MODULES_PHYSICSNODE_H__
#define __I6ENGINE_MODULES_PHYSICSNODE_H__

#include <vector>

#include "i6engine/api/components/PhysicalStateComponent.h"

#include "LinearMath/btMotionState.h"

class btCollisionShape;
class btCompoundShape;
class btRigidBody;

namespace i6engine {
namespace api {
	struct CollisionGroup;
}
namespace modules {

	typedef std::map<std::string, std::string> attributeMap;

	class PhysicsMailbox;
	class PhysicsManager;

	/**
	 * \brief contains informations for periodic raytests
	 */
	typedef struct PeriodicRaytest {
		Vec3 from;
		Vec3 to;
		api::PhysicalStateComponent::RayTestNotify raytestNotify;
		api::GameMessage::Ptr message;

		api::RayTestResult lastResult;

		PeriodicRaytest() : from(), to(), raytestNotify(), message(), lastResult() {
		}

		PeriodicRaytest(const Vec3 & f, const Vec3 & t, api::PhysicalStateComponent::RayTestNotify rtn, api::GameMessage::Ptr msg) : from(f), to(t), raytestNotify(rtn), message(msg), lastResult() {
		}
	} PeriodicRaytest;

	/**
	 * \class PhysicsNode
	 * \brief Represents the physical state of an object simulated by the Bullet library.
	 * Contains the btTransform and a pointer to the associated rigidBody.
	 *
	 * The class is responsible for signing rigidBodys in and out the PhysicsController.
	 *
	 * Update Mechanism for positioning:
	 * \msc
	 * 	Arbitary, PSC, PN
	 *	Arbitary->PSC [label=setPosition()]
	 *	PSC->PN [label=PosUpdate()]
	 *	PN->PSC [label=PosUpdate()]
	 * \endmsc
	 */
	class PhysicsNode : public btMotionState {
		friend class PhysicsMailbox;
		friend class PhysicsManager;

	public:
		typedef boost::shared_ptr<PhysicsNode> Ptr;

		/**
		 * \brief constructor
		 */
		PhysicsNode(const int64_t id, const int64_t compId, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale, const api::CollisionGroup & cg, const api::attributeMap & params, api::ShatterInterest shatterInterest, PhysicsManager * pm);

		/**
		 * \brief destructor
		 */
		~PhysicsNode();

		/**
		 * \brief adds a new child shape to this node
		 */
		bool addChild(const int64_t compId, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale, const api::CollisionGroup & cg, const api::PhysicalStateComponent::ShapeType shapeType, const attributeMap & shapeParams);

		/**
		 * \brief removes a child shape
		 */
		void removeChild(int64_t compId);

		bool empty() const;

		/**
		 * \brief Clears all forces and sets the velocity to 0, so that e.g. a bike after shattering
		 * 		 doesn't still move after resetting
		 */
		void reset();

		/**
		 * \brief Override from PhysicalStateComponent to update internal transform.
		 * Sets new position by calling PhysicalStateComponent#SetPosition(const Vec3 position)
		 * and updates internal transform PhysicsNode#_transform.
		 * \param[in] position new position
		 */
		void setPosition(const Vec3 & position, int64_t id);

		/**
		 * \brief Override from PhysicalStateComponent to update internal transform.
		 * Sets new rotation by calling PhysicalStateComponent#setRotation(const Quaternion rotation)
		 * and updates internal transform PhysicsNode#_transform.
		 * \param[in] rotation new rotation
		 */
		void setRotation(const Quaternion & rotation, int64_t id);

		/**
		 * \brief Override from PhysicalStateComponent to update internal transform.
		 * Sets new scale by calling PhysicalStateComponent#setScale(Vec3 scale).
		 * \param[in] scale new scale
		 */
		void setScale(const Vec3 & scale, int64_t id);

		/**
		 * \brief Called by Bullet when the object is first scheduled for simulation to retrieve the initial transform.
		 *
		 * \params[out] worldTrans
		 */
		inline void getWorldTransform(btTransform & worldTrans) const { worldTrans = _transform; }

		/**
		 * \brief Called by Bullet when the simulated object has moved.
		 *
		 */
		void setWorldTransform(const btTransform & worldTrans);

		/**
		 * \brief returns the rigidBody of the PhysicsNode
		 */
		inline btRigidBody * getRigidBody() const { return _rigidBody; }

		inline Vec3 getOldScale() const { return _oldScale; }

		/**
		 * \brief Tests a Hit with a ray given by the vector
		 */
		api::RayTestResult HitTest(const Vec3 & from, const Vec3 & to);

		/**
		 * \brief returns the GO id of the Node
		 */
		int64_t getID() const {
			return _id;
		}

		int64_t getCompID() const {
			return _compId;
		}

		/**
		 * \brief returns the CrashType of the Node
		 */
		uint32_t getCrashType() { return _collisionGroup.crashType; }

		/**
		 * \brief sets the collision flags of the Node
		 */
		void setCollisionFlags(const api::CollisionGroup & cg);

		/**
		 * \brief sets the crashtype of the node
		 */
		void setCrashType(uint32_t ct) {
			_collisionGroup.crashType = ct;
		}

		/**
		 * \brief sets the crash mask of the Node
		 */
		void setCrashMask(uint32_t cm) {
			_collisionGroup.crashMask = cm;
		}

		/**
		 * \brief returns the CrashMask of the Node
		 */
		uint32_t getCrashMask() { return _collisionGroup.crashMask; }

		/**
		 * \brief returns the position of the node
		 */
		Vec3 getPosition() const { return _position; }

		/**
		 * \brief returns the rotation of the node
		 */
		Quaternion getRotation() const { return _rotation; }

		/**
		 * \brief returns the scale of the node
		 */
		Vec3 getScale() const { return _scale; }

		/**
		 * \brief applies the new rotation transforming rotation and forces
		 */
		void applyRotation(const Quaternion & rotation, int64_t id);

		/**
		 * \brief get the collision interest
		 */
		api::ShatterInterest getShatterInterest() const { return _shatterInterest; }

		/**
		 * \brief called during every physics tick
		 */
		void Tick();

	private:
		/**
		 * \brief pointer to the PhysicsManager
		 */
		PhysicsManager * _manager;

		/**
		 * \brief id of the corresponding GameObject
		 */
		int64_t _id;
		int64_t _compId;
		api::CollisionGroup _collisionGroup;

		Vec3 _position;
		Quaternion _rotation;
		Vec3 _scale;

		btTransform _transform;
		btRigidBody * _rigidBody;
		Vec3 _oldScale;

		Vec3 _linearVelocity;
		Vec3 _centralForce;

		/**
		 * \brief flag containing shatter interests
		 */
		api::ShatterInterest _shatterInterest;

		/**
		 * \brief map of all periodic raytests
		 */
		std::vector<PeriodicRaytest *> _rayTests;

		/**
		 * \brief CompoundShape managing all subshapes
		 * position and rotation are offsets
		 */
		btCompoundShape * _parentShape;

		std::map<int64_t, std::pair<int64_t, btCollisionShape *>> _childShapes;

		/**
		 * \brief Adds the component's RigidBody to Bullet's world.
		 */
		void startSimulatingBody();

		/**
		 * \brief Removes the component's RigidBody from Bullet's world.
		 */
		void stopSimulatingBody();

		/**
		 * \brief handles messages for the node
		 */
		void News(const api::GameMessage::Ptr & msg);

		/**
		 * \brief adds this node as a ticking one
		 */
		void addTicker();

		/**
		 * \brief removes this node from ticking list
		 */
		void removeTicker();

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_PHYSICSNODE_H__ */

/**
 * @}
 */
