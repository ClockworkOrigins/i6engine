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

#ifndef __I6ENGINE_MODULES_PHYSICSMANAGER_H__
#define __I6ENGINE_MODULES_PHYSICSMANAGER_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"

#include "i6engine/modules/physics/PhysicsNode.h"

#include "LinearMath/btAlignedObjectArray.h"

class btBroadphaseInterface;
class btCollisionDispatcher;
class btCollisionShape;
class btCompoundShape;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btMotionState;
class btRigidBody;
class btSequentialImpulseConstraintSolver;
class btTypedConstraint;
struct btOverlapFilterCallback;

namespace i6e {
namespace modules {

	class DebugDrawer;
	class PhysicsController;

	class PhysicsManager {
	public:
		/**
		 * \brief stores all collision pairs
		 */
		static std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t> _collisionPairs;

		/**
		 * \brief counts ticks for this subsystem
		 */
		static uint64_t _tickCount;

		/**
		 * \brief constructor
		 */
		explicit PhysicsManager(PhysicsController * pc);

		/**
		 * \brief destructor
		 */
		~PhysicsManager();

		/**
		 * \brief handles create messages for the PhysicsManager
		 */
		void NewsCreate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles update messages for the PhysicsManager
		 */
		void NewsUpdate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles delete messages for the PhysicsManager
		 */
		void NewsDelete(const api::GameMessage::Ptr & msg);

		/**
		 * \brief Ticks physics stuff
		 */
		void Tick();

		/**
		 * \brief Creates a new CompoundShape
		 */
		btCompoundShape * createCompound();

		/**
		 * \brief Creates a new RigidBody with a sphere collisionShape.
		 * \param[in] motionState
		 * \param[in] params attributeMap containing all values set for sphere
		 */
		btCollisionShape * createSphere(const api::attributeMap & params);

		/**
		 * \brief Creates a new RigidBody from a collisionShape from a file.
		 * \param[in] motionState
		 * \param[in] params attributeMap containing all values set for file
		 */
		btCollisionShape * createFromFile(const api::attributeMap & params);

		/**
		 * \brief Creates a new RigidBody with a box collisionShape.
		 * \param[in] motionState
		 * \param[in] params attributeMap containing all values set for box
		 */
		btCollisionShape * createBox(const api::attributeMap & params);

		/**
		 * \brief Creates a new RigidBody with a planes collisionShape.
		 * \param[in] motionState
		 * \param[in] params attributeMap containing all values set for plane
		 */
		btCollisionShape * createPlane(const api::attributeMap & params);

		/**
		 * \brief creates an empty default shape
		 */
		btCollisionShape * createEmptyShape();

		/**
		 * \brief Deletes a body's collisionShape and removes it from the PhysicsShapeController#_collisionShapes array
		 * \param[in] body the RigidBody to be deleted
		 */
		void remove(btRigidBody * body);
		void removeShape(btCollisionShape * shape);

		/**
		 * \brief
		 * \returns the dynamic world
		 */
		btDynamicsWorld * getPhysicsWorld() const;

		/**
		 * \brief Sets the gravity of the world
		 * \param[in] gravity The "force" direction. The usual gravity is (0, -y, 0) where -y is the value of the desired gravity (9.8 in rl)
		 */
		void setGravity(const Vec3 & gravity);

		/**
		 * \brief Sets the gravity of the world.
		 * \param[in] gravityDown Sets gravity to (0, -gravityDown, 0).
		 */
		void setGravity(const double gravityDown);

		/**
		 * \brief Adds the body to the world for simulation.
		 */
		void addRigidBody(btRigidBody * rigidBody);

		/**
		 * \brief Removes the body from the world to stop simulation.
		 */
		void removeRigidBody(btRigidBody * rigidBody);

		/**
		 * \brief returns the PhysicsNode with the given ID
		 */
		PhysicsNode * getPhysicsNode(const int64_t id);

		/**
		 * \brief adds a ticking PhysicsNode
		 */
		void addTicker(PhysicsNode * pn);

		/**
		 * \brief removes a ticking PhysicsNode
		 */
		void removeTicker(PhysicsNode * pn);

	private:
		PhysicsController * _ctrl;

		/**
		 * \brief list of all collisionShapes
		 */
		btAlignedObjectArray<btCollisionShape *> _collisionShapes;

		btDiscreteDynamicsWorld * _dynamicsWorld;
		btBroadphaseInterface * _broadphase;
		btCollisionDispatcher * _dispatcher;
		btDefaultCollisionConfiguration * _collisionConfiguration;
		btSequentialImpulseConstraintSolver * _solver;

		std::map<int64_t, PhysicsNode *> _nodes;

		// http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Things
		uint64_t _lngTime;

		std::vector<PhysicsNode *> _tickList;

		std::map<int64_t, std::vector<std::pair<int64_t, btTypedConstraint *>>> _constraints;

		/**
		 * \brief is the subsystem paused or not?
		 */
		bool _paused;

		DebugDrawer * _debugDrawer;

		btOverlapFilterCallback * _filterCallback;

		/**
		 * \brief forbidden
		 */
		PhysicsManager(const PhysicsManager &) = delete;

		/**
		 * \brief forbidden
		 */
		PhysicsManager & operator=(const PhysicsManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PHYSICSMANAGER_H__ */

/**
 * @}
 */
