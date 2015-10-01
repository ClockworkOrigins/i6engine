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

#include "i6engine/modules/physics/PhysicsManager.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/physics/DebugDrawer.h"
#include "i6engine/modules/physics/PhysicsController.h"
#include "i6engine/modules/physics/PhysicsNode.h"

#include "btBulletDynamicsCommon.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"

namespace i6engine {
namespace modules {

	struct FilterCallback : public btOverlapFilterCallback {
		// return true when pairs need collision
		virtual bool needBroadphaseCollision(btBroadphaseProxy * proxy0, btBroadphaseProxy * proxy1) const {
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

			// add some additional logic here that modified 'collides'
			return true;
		}
	};

	// TODO: move to memberfunction
	// TODO: only save contact pair if objects really collide
	bool myContactAddedCallback(btManifoldPoint & cp, const btCollisionObject * colObj0, int partId0, int index0, const btCollisionObject * colObj1, int partId1, int index1) {
		// do nothing in multiplayer clients
		if (api::EngineController::GetSingletonPtr()->getType() != api::GameType::SERVER && api::EngineController::GetSingletonPtr()->getType() != api::GameType::SINGLEPLAYER) {
			return false;
		}

		PhysicsNode * a = reinterpret_cast<PhysicsNode *>(reinterpret_cast<const btCollisionObjectWrapper *>(colObj0)->getCollisionObject()->getUserPointer());
		PhysicsNode * b = reinterpret_cast<PhysicsNode *>(reinterpret_cast<const btCollisionObjectWrapper *>(colObj1)->getCollisionObject()->getUserPointer());

		if (a->getCrashMask() & b->getCrashType()) {
			// A collides with B
			if (PhysicsManager::_collisionPairs.find(std::make_pair(a, b)) == PhysicsManager::_collisionPairs.end()) {
				PhysicsManager::_collisionPairs[std::make_pair(a, b)] = PhysicsManager::_tickCount;

				if (a->getShatterInterest() & api::ShatterInterest::START) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComShatter, core::Method::Update, new api::components::Component_Shatter_Update(a->getID(), a->getCompID(), b->getID(), api::ShatterInterest::START), core::Subsystem::Physic);
					// TODO: (Michael) improve this hack
					api::GOPtr objA = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(a->getID());
					if (objA == nullptr) {
						return false;
					}
					core::IPKey aOwner = objA->getOwner();
					if (aOwner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					} else {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
						api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
					}
				}
			} else {
				PhysicsManager::_collisionPairs[std::make_pair(a, b)] = PhysicsManager::_tickCount;

				if (a->getShatterInterest() & api::ShatterInterest::ALWAYS) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComShatter, core::Method::Update, new api::components::Component_Shatter_Update(a->getID(), a->getCompID(), b->getID(), api::ShatterInterest::ALWAYS), core::Subsystem::Physic);
					// TODO: (Michael) improve this hack
					api::GOPtr objA = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(a->getID());
					if (objA == nullptr) {
						return false;
					}
					core::IPKey aOwner = objA->getOwner();
					if (aOwner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					} else {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
						api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
					}
				}
			}
		}
		if (b->getCrashMask() & a->getCrashType()) {
			// B collides with A
			if (PhysicsManager::_collisionPairs.find(std::make_pair(b, a)) == PhysicsManager::_collisionPairs.end()) {
				PhysicsManager::_collisionPairs[std::make_pair(b, a)] = PhysicsManager::_tickCount;

				if (b->getShatterInterest() & api::ShatterInterest::START) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComShatter, core::Method::Update, new api::components::Component_Shatter_Update(b->getID(), b->getCompID(), a->getID(), api::ShatterInterest::START), core::Subsystem::Physic);
					// TODO: (Michael) improve this hack
					api::GOPtr objB = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(b->getID());
					if (objB == nullptr) {
						return false;
					}
					core::IPKey bOwner = objB->getOwner();
					if (bOwner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					} else {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
						api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
					}
				}
			} else {
				PhysicsManager::_collisionPairs[std::make_pair(b, a)] = PhysicsManager::_tickCount;

				if (b->getShatterInterest() & api::ShatterInterest::ALWAYS) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComShatter, core::Method::Update, new api::components::Component_Shatter_Update(b->getID(), b->getCompID(), a->getID(), api::ShatterInterest::ALWAYS), core::Subsystem::Physic);
					// TODO: (Michael) improve this hack
					api::GOPtr objB = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(b->getID());
					if (objB == nullptr) {
						return false;
					}
					core::IPKey bOwner = objB->getOwner();
					if (bOwner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					} else {
						api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
						api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
					}
				}
			}
		}
		return false;
	}

	std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t> PhysicsManager::_collisionPairs = std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t>();

	uint64_t PhysicsManager::_tickCount = 0;

	PhysicsManager::PhysicsManager(PhysicsController * pc) : _ctrl(pc), _collisionShapes(), _dynamicsWorld(), _broadphase(), _dispatcher(), _collisionConfiguration(), _solver(), _nodes(), _lngTime(api::EngineController::GetSingleton().getCurrentTime()), _tickList(), _constraints(), _paused(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		try {
			// Build the broadphase
			// _broadphase = new btDbvtBroadphase();
			_broadphase = new btAxisSweep3(btVector3(-50000, -50000, -50000), btVector3(50000, 50000, 50000));

			// Set up the collision configuration and dispatcher
			_collisionConfiguration = new btDefaultCollisionConfiguration();
			_dispatcher = new btCollisionDispatcher(_collisionConfiguration);

			// The actual physics solver
			_solver = new btSequentialImpulseConstraintSolver();

			// The world.
			_dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
			_dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
			_dynamicsWorld->getDispatchInfo().m_useContinuous = true;

			// FilterCallback
			btOverlapFilterCallback * filterCallback = new FilterCallback();
			_dynamicsWorld->getPairCache()->setOverlapFilterCallback(filterCallback);

			_dynamicsWorld->setDebugDrawer(new DebugDrawer());
			_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
			_dynamicsWorld->getSolverInfo().m_splitImpulse = true;
			_dynamicsWorld->getSolverInfo().m_numIterations = 20;

			// add collisioncallback
			gContactAddedCallback = reinterpret_cast<ContactAddedCallback>(&myContactAddedCallback);
		} catch(std::exception & e) {
			ISIXE_LOG_ERROR("Physics", "An exception has occurred: " << e.what());
		}
	}

	PhysicsManager::~PhysicsManager() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_nodes.clear();
		// remove the rigidbodies from the dynamics world and delete them (including motionStates)
		for (int i = _dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
			btCollisionObject * obj = _dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody * body = btRigidBody::upcast(obj);

			if (body && body->getMotionState()) {
				delete body->getMotionState();
			}
			_dynamicsWorld->removeCollisionObject(obj);
		}

		delete _dynamicsWorld->getDebugDrawer();
		delete _dynamicsWorld;
		delete _solver;
		delete _dispatcher;
		delete _collisionConfiguration;
		delete _broadphase;
	}

	void PhysicsManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		try {
			// the time to be simulated, in seconds
			uint64_t cT = api::EngineController::GetSingleton().getCurrentTime();
			btScalar timestep = double(cT - _lngTime) / 1000000.0;
			_lngTime = cT;

			// referring to bullet wiki, this formula must be always true: http://bulletphysics.org/mediawiki-1.5.8/index.php/Stepping_The_World
			//assert(timestep < maxSubSteps * fixedTimeStep);

			if (!_paused) {
				_tickCount++;

				_dynamicsWorld->stepSimulation(timestep, 3);

				for (PhysicsNode * p : _tickList) {
					p->Tick();
				}

				for (std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t>::iterator it = _collisionPairs.begin(); it != _collisionPairs.end();) {
					if (it->second < _tickCount) {
						if (it->first.first->getShatterInterest() & api::ShatterInterest::END) {
							api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComShatter, core::Method::Update, new api::components::Component_Shatter_Update(it->first.first->getID(), it->first.first->getCompID(), it->first.second->getID(), api::ShatterInterest::END), core::Subsystem::Physic);
							// TODO: (Michael) improve this hack
							core::IPKey aOwner = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(it->first.first->getID())->getOwner();
							if (aOwner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
								api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
							} else {
								api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
								api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
							}
						}

						std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t>::iterator it2 = it;
						it++;
						_collisionPairs.erase(it2);
					} else {
						it++;
					}
				}

				if (api::EngineController::GetSingletonPtr()->getDebugdrawer() > 0) {
					_dynamicsWorld->debugDrawWorld();
				}
			}
		} catch(utils::exceptions::SubsystemException & e) {
			e.writeLog();
			// check errorcode and repair, if not possible:
			e.PassToMain();
		}
	}

	void PhysicsManager::NewsCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::physics::PhyNode) {
			api::physics::Physics_Node_Create * pnc = static_cast<api::physics::Physics_Node_Create *>(msg->getContent());

			if (_nodes.find(pnc->_waitForId) == _nodes.end()) {
				PhysicsNode * newPN = new PhysicsNode(pnc->_waitForId, pnc->_id, pnc->pos, pnc->rot, pnc->scale, pnc->collisionGroup, pnc->shapeParams, pnc->shatterInterest, this);
				_nodes.insert(std::make_pair(pnc->_waitForId, newPN));
			}
			if (!_nodes[pnc->getWaitID()]->addChild(pnc->getID(), pnc->pos, pnc->rot, pnc->scale, pnc->collisionGroup, pnc->shapeType, pnc->shapeParams)) {
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg); // PhysicsNode couldn't be created, so queue this message again and try it later
			}
		} else if (type == api::physics::PhyVelocityComponent) {
			// TODO: (Daniel) implement
		}
	}

	void PhysicsManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::physics::PhyGravity) {
			Vec3 vec3Gravity = static_cast<api::physics::Physics_Gravity_Update *>(msg->getContent())->grav;

			setGravity(vec3Gravity);
		} else if (type == api::physics::PhyConstraintBreakImpulse) {
			api::physics::Physics_BreakConstraintImpulse_Update * pbu = dynamic_cast<api::physics::Physics_BreakConstraintImpulse_Update *>(msg->getContent());
			auto it = _constraints.find(pbu->getWaitID());
			if (it != _constraints.end()) {
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
					if (it2->first == pbu->targetGOID) {
						it2->second->setBreakingImpulseThreshold(pbu->breakingImpulse);
					}
				}
			}
		} else if (type == api::physics::PhyPause) {
			_paused = dynamic_cast<api::physics::Physics_Pause_Update *>(msg->getContent())->pause;
		} else if (type == api::physics::PhyP2PConstraint) {
			api::physics::Physics_P2PConstraint_Create * ppc = dynamic_cast<api::physics::Physics_P2PConstraint_Create *>(msg->getContent());
			auto itSelf = _nodes.find(ppc->_waitForId);
			auto itTarget = _nodes.find(ppc->targetGOID);
			if (itSelf == _nodes.end() || itTarget == _nodes.end()) {
				ISIXE_THROW_FAILURE("PhysicsManager", "Tried to create constraint for not existing PhysicsNode(s)");
			}
			btPoint2PointConstraint * p2p = new btPoint2PointConstraint(*itSelf->second->getRigidBody(), *itTarget->second->getRigidBody(), ppc->selfOffset.toBullet(), ppc->targetOffset.toBullet());
			_dynamicsWorld->addConstraint(p2p);
			_constraints[ppc->_waitForId].push_back(std::make_pair(ppc->targetGOID, p2p));
			_constraints[ppc->targetGOID].push_back(std::make_pair(ppc->_waitForId, p2p));
		} else {
			ISIXE_THROW_MESSAGE("PhysicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void PhysicsManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::physics::PhyNode) {
			api::physics::Physics_Node_Delete * pnc = static_cast<api::physics::Physics_Node_Delete *>(msg->getContent());

			PhysicsNode * tmp = getPhysicsNode(pnc->_waitForId);

			if (tmp == nullptr) { // happens during logout: Physic receives clean message from physics subsystem and delete physicsnode from PhysicalStateComponent
				return;
			}

			for (std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t>::iterator it = _collisionPairs.begin(); it != _collisionPairs.end(); ++it) {
				if (it->first.first == tmp || it->first.second == tmp) {
					_collisionPairs.erase(it);
				}
			}

			_nodes[pnc->getWaitID()]->removeChild(pnc->getID());

			if (_nodes[pnc->getWaitID()]->empty()) {
				_nodes.erase(pnc->getWaitID());
			}

			auto it = _constraints.find(pnc->getWaitID());
			if (it != _constraints.end()) {
				for (auto & p : it->second) {
					auto it2 = _constraints.find(p.first);
					if (it2 != _constraints.end()) {
						for (auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
							if (it3->first == pnc->getWaitID()) {
								it2->second.erase(it3);
								break;
							}
						}
						break;
					}
					_dynamicsWorld->removeConstraint(p.second);
					delete p.second;
				}
				_constraints.erase(it);
			}
			delete tmp;
		} else if (msg->getSubtype() == api::physics::PhyReset) {
			_ctrl->reset();
		} else if (msg->getSubtype() == api::physics::PhyConstraint) {
			api::physics::Physics_Constraint_Delete * ppd = dynamic_cast<api::physics::Physics_Constraint_Delete *>(msg->getContent());
			auto it = _constraints.find(ppd->getWaitID());
			if (it != _constraints.end()) {
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
					if (it2->first == ppd->targetGOID) {
						_dynamicsWorld->removeConstraint(it2->second);
						delete it2->second;
						it->second.erase(it2);
					}
				}
			}
			it = _constraints.find(ppd->targetGOID);
			if (it != _constraints.end()) {
				for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
					if (it2->first == ppd->getWaitID()) {
						it->second.erase(it2);
					}
				}
			}
		} else if (type == api::physics::PhyVelocityComponent) {
			// TODO: (Daniel) implement
		} else {
			ISIXE_THROW_MESSAGE("PhysicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	btCompoundShape * PhysicsManager::createCompound() {
		ASSERT_THREAD_SAFETY_FUNCTION

		btCompoundShape * compoundShape = new btCompoundShape();
		_collisionShapes.push_back(compoundShape);

		return compoundShape;
	}

	btCollisionShape * PhysicsManager::createSphere(const attributeMap & params) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 localInertia;
		double radius = 1.0;
		double mass = 0.0;

		assert(params.find("radius") != params.end());

		if (params.find("radius") != params.end()) {
			radius = std::stod(params.find("radius")->second);
		}

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("mass") != params.end()) {
			mass = std::stod(params.find("mass")->second);
		}

		btCollisionShape * sphereShape = new btSphereShape(radius);
		_collisionShapes.push_back(sphereShape);

		btVector3 inertia = localInertia.toBullet();
		sphereShape->calculateLocalInertia(mass, inertia);

		return sphereShape;
	}

	btCollisionShape * PhysicsManager::createFromFile(const attributeMap & params) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 localInertia;
		double mass = 0.0f;
		std::string fileName;
		uint32_t shapeIndex = 0;

		assert(params.find("fileName") != params.end());

		fileName = params.find("fileName")->second;

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("mass") != params.end()) {
			mass = std::stod(params.find("mass")->second);
		}

		if (params.find("shapeIndex") != params.end()) {
			shapeIndex = std::stoul(params.find("shapeIndex")->second);
		}

		btBulletWorldImporter * fileLoader = new btBulletWorldImporter(_dynamicsWorld);
		fileLoader->loadFile(fileName.c_str());
		btCollisionShape * shape = fileLoader->getCollisionShapeByIndex(int(shapeIndex));
		delete fileLoader;

		if (shape == nullptr) {
			ISIXE_THROW_API("PhysicsManager", "Couldn't load collisionShape from file " << fileName);
		}

		_collisionShapes.push_back(shape);

		btVector3 inertia = localInertia.toBullet();

		if (mass > 0.0 && dynamic_cast<btTriangleMeshShape *>(shape) != nullptr) {
			ISIXE_THROW_API("PhysicsManager", "TriangleMeshShape musn't have mass, only static objects are supported!");
		}

		shape->calculateLocalInertia(mass, inertia);

		return shape;
	}

	btCollisionShape * PhysicsManager::createBox(const attributeMap & params) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 box;
		Vec3 localInertia;
		double mass = 0.0;

		assert(params.find("box") != params.end());

		if (params.find("box") != params.end()) {
			box = Vec3(params, "box");
		}

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("mass") != params.end()) {
			mass = std::stod(params.find("mass")->second);
		}

		btCollisionShape * boxShape = new btBoxShape(box.toBullet());
		_collisionShapes.push_back(boxShape);

		btVector3 inertia = localInertia.toBullet();
		boxShape->calculateLocalInertia(mass, inertia);

		return boxShape;
	}

	btCollisionShape * PhysicsManager::createPlane(const attributeMap & params) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 planeNormal;
		Vec3 localInertia;
		double planeConstant = 0.0f;

		if (params.find("planeVector") != params.end()) {
			planeNormal = Vec3(params, "planeVector");
		}

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("planeConstant") != params.end()) {
			planeConstant = std::stod(params.find("planeConstant")->second);
		}

		btCollisionShape * groundShape = new btStaticPlaneShape(planeNormal.toBullet(), planeConstant);
		_collisionShapes.push_back(groundShape);

		return groundShape;
	}

	btCollisionShape * PhysicsManager::createEmptyShape() {
		ASSERT_THREAD_SAFETY_FUNCTION
		btCollisionShape * fallShape = new btEmptyShape();
		_collisionShapes.push_back(fallShape);

		return fallShape;
	}

	void PhysicsManager::remove(btRigidBody * body) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_collisionShapes.remove(body->getCollisionShape());
		delete body->getCollisionShape();
	}

	void PhysicsManager::removeShape(btCollisionShape * shape) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_collisionShapes.remove(shape);
		delete shape;
	}

	PhysicsNode * PhysicsManager::getPhysicsNode(const int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_nodes.find(id) == _nodes.end()) {
			return nullptr;
		}

		return _nodes[id];
	}

	void PhysicsManager::addRigidBody(btRigidBody * rigidBody) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_dynamicsWorld->addRigidBody(rigidBody, btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::DefaultFilter);
	}

	btDynamicsWorld * PhysicsManager::getPhysicsWorld() const {
		ASSERT_THREAD_SAFETY_FUNCTION
		return _dynamicsWorld;
	}

	void PhysicsManager::setGravity(const Vec3 & gravity) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_dynamicsWorld->setGravity(gravity.toBullet());
	}

	void PhysicsManager::setGravity(const double gravityDown) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_dynamicsWorld->setGravity(btVector3(0.0, gravityDown, 0.0));
	}

	void PhysicsManager::removeRigidBody(btRigidBody * rigidBody) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_dynamicsWorld->removeRigidBody(rigidBody);
	}

	void PhysicsManager::addTicker(PhysicsNode * pn) {
		ASSERT_THREAD_SAFETY_FUNCTION
		_tickList.push_back(pn);
	}

	void PhysicsManager::removeTicker(PhysicsNode * pn) {
		ASSERT_THREAD_SAFETY_FUNCTION
		for (std::vector<PhysicsNode *>::iterator it = _tickList.begin(); it != _tickList.end(); ++it) {
			if (*it == pn) {
				_tickList.erase(it);
				break;
			}
		}
	}

} /* namespace modules */
} /* namespace i6engine */
