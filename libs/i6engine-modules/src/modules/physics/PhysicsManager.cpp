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

#include "i6engine/modules/graphics/ResourceManager.h"
#include "i6engine/modules/physics/DebugDrawer.h"
#include "i6engine/modules/physics/MeshManager.h"
#include "i6engine/modules/physics/MeshStrider.h"
#include "i6engine/modules/physics/PhysicsController.h"
#include "i6engine/modules/physics/PhysicsNode.h"

#include "boost/lexical_cast.hpp"

#include "btBulletDynamicsCommon.h"
#include "BulletWorldImporter/btBulletWorldImporter.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreDefaultHardwareBufferManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreMeshManager.h"
#include "OGRE/OgreRoot.h"

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
	// do nothing only in multiplayer clients
	if (i6engine::api::EngineController::GetSingletonPtr()->getType() != i6engine::api::GameType::SERVER && i6engine::api::EngineController::GetSingletonPtr()->getType() != i6engine::api::GameType::SINGLEPLAYER) {
		return false;
	}

	i6engine::modules::PhysicsNode * a = reinterpret_cast<i6engine::modules::PhysicsNode *>(colObj0->getUserPointer());
	i6engine::modules::PhysicsNode * b = reinterpret_cast<i6engine::modules::PhysicsNode *>(colObj1->getUserPointer());

	if (a->getCrashMask() & b->getCrashType()) {
		// A collides with B
		if (i6engine::modules::PhysicsManager::_collisionPairs.find(std::make_pair(a, b)) == i6engine::modules::PhysicsManager::_collisionPairs.end()) {
			i6engine::modules::PhysicsManager::_collisionPairs[std::make_pair(a, b)] = i6engine::modules::PhysicsManager::_tickCount;

			if (a->getShatterInterest() & i6engine::api::PhysicalStateComponent::ShatterInterest::START) {
				i6engine::api::GameMessage::Ptr msg = boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::ComponentMessageType, i6engine::api::components::ComShatter, i6engine::core::Method::Update, new i6engine::api::components::Component_Shatter_Update(a->getID(), a->getCompID(), b->getID(), i6engine::api::PhysicalStateComponent::ShatterInterest::START), i6engine::core::Subsystem::Physic);
				// TODO: (Michael) improve this hack
				i6engine::api::GOPtr objA = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(a->getID());
				if (objA == nullptr) {
					return false;
				}
				i6engine::core::IPKey aOwner = objA->getOwner();
				if (aOwner == i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				} else {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
				}
			}
		} else {
			i6engine::modules::PhysicsManager::_collisionPairs[std::make_pair(a, b)] = i6engine::modules::PhysicsManager::_tickCount;

			if (a->getShatterInterest() & i6engine::api::PhysicalStateComponent::ShatterInterest::ALWAYS) {
				i6engine::api::GameMessage::Ptr msg = boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::ComponentMessageType, i6engine::api::components::ComShatter, i6engine::core::Method::Update, new i6engine::api::components::Component_Shatter_Update(a->getID(), a->getCompID(), b->getID(), i6engine::api::PhysicalStateComponent::ShatterInterest::ALWAYS), i6engine::core::Subsystem::Physic);
				// TODO: (Michael) improve this hack
				i6engine::api::GOPtr objA = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(a->getID());
				if (objA == nullptr) {
					return false;
				}
				i6engine::core::IPKey aOwner = objA->getOwner();
				if (aOwner == i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				} else {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
				}
			}
		}
	}
	if (b->getCrashMask() & a->getCrashType()) {
		// B collides with A
		if (i6engine::modules::PhysicsManager::_collisionPairs.find(std::make_pair(b, a)) == i6engine::modules::PhysicsManager::_collisionPairs.end()) {
			i6engine::modules::PhysicsManager::_collisionPairs[std::make_pair(b, a)] = i6engine::modules::PhysicsManager::_tickCount;

			if (b->getShatterInterest() & i6engine::api::PhysicalStateComponent::ShatterInterest::START) {
				i6engine::api::GameMessage::Ptr msg = boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::ComponentMessageType, i6engine::api::components::ComShatter, i6engine::core::Method::Update, new i6engine::api::components::Component_Shatter_Update(b->getID(), b->getCompID(), a->getID(), i6engine::api::PhysicalStateComponent::ShatterInterest::START), i6engine::core::Subsystem::Physic);
				// TODO: (Michael) improve this hack
				i6engine::api::GOPtr objB = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(b->getID());
				if (objB == nullptr) {
					return false;
				}
				i6engine::core::IPKey bOwner = objB->getOwner();
				if (bOwner == i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				} else {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
				}
			}
		} else {
			i6engine::modules::PhysicsManager::_collisionPairs[std::make_pair(b, a)] = i6engine::modules::PhysicsManager::_tickCount;

			if (b->getShatterInterest() & i6engine::api::PhysicalStateComponent::ShatterInterest::ALWAYS) {
				i6engine::api::GameMessage::Ptr msg = boost::make_shared<i6engine::api::GameMessage>(i6engine::api::messages::ComponentMessageType, i6engine::api::components::ComShatter, i6engine::core::Method::Update, new i6engine::api::components::Component_Shatter_Update(b->getID(), b->getCompID(), a->getID(), i6engine::api::PhysicalStateComponent::ShatterInterest::ALWAYS), i6engine::core::Subsystem::Physic);
				// TODO: (Michael) improve this hack
				i6engine::api::GOPtr objB = i6engine::api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(b->getID());
				if (objB == nullptr) {
					return false;
				}
				i6engine::core::IPKey bOwner = objB->getOwner();
				if (bOwner == i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				} else {
					i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
					i6engine::api::EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
				}
			}
		}
	}
	return false;
}

namespace i6engine {
namespace modules {

	std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t> PhysicsManager::_collisionPairs = std::map<std::pair<PhysicsNode *, PhysicsNode *>, uint64_t>();

	uint64_t PhysicsManager::_tickCount = 0;

	PhysicsManager::PhysicsManager(PhysicsController * pc) : _ctrl(pc), _collisionShapes(), _meshes(), _dynamicsWorld(), _broadphase(), _dispatcher(), _collisionConfiguration(), _solver(), _nodes(), _lngTime(api::EngineController::GetSingleton().getCurrentTime()), _root(nullptr), _resourceManager(nullptr), _tickList(), _meshManager(new MeshManager()), _paused(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		try {
			// Build the broadphase
			// _broadphase = new btDbvtBroadphase();
			_broadphase = new btAxisSweep3(btVector3(-50000, -50000, -50000), btVector3(50000, 50000, 50000));

			// Set up the collision configuration and dispatcher
			_collisionConfiguration = new btDefaultCollisionConfiguration();
			_dispatcher = new btCollisionDispatcher(_collisionConfiguration);

			// The actual physics solver
			_solver = new btSequentialImpulseConstraintSolver;

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

			if (api::EngineController::GetSingletonPtr()->getType() == api::GameType::SERVER) {
				std::string ogrePath;
				// GRAPHIC is correct, because we want the same variable as for ogre
				if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue<std::string>("GRAPHIC", "ogreConfigsPath", ogrePath)) {
					ISIXE_LOG_ERROR("Physics", "An exception has occurred: value ogreConfigsPath in section GRAPHIC not found!");
					api::EngineController::GetSingletonPtr()->stop();
					return;
				}
				Ogre::LogManager * lm = new Ogre::LogManager();
				lm->createLog("ogre.log", true, false, false);
				_root = new Ogre::Root("", ogrePath + "/ogre.cfg", "");
				new Ogre::DefaultHardwareBufferManager();
				_resourceManager = new ResourceManager(ogrePath);
			}
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

		delete _resourceManager;
		delete _root;
		delete _dynamicsWorld->getDebugDrawer();
		delete _dynamicsWorld;
		delete _solver;
		delete _dispatcher;
		delete _collisionConfiguration;
		delete _broadphase;

		removeAll();
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
						if (it->first.first->getShatterInterest() & api::PhysicalStateComponent::ShatterInterest::END) {
							api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::ComponentMessageType, api::components::ComShatter, core::Method::Update, new api::components::Component_Shatter_Update(it->first.first->getID(), it->first.first->getCompID(), it->first.second->getID(), api::PhysicalStateComponent::ShatterInterest::END), i6engine::core::Subsystem::Physic);
							// TODO: (Michael) improve this hack
							core::IPKey aOwner = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(it->first.first->getID())->getOwner();
							if (aOwner == api::EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
								api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
							} else {
								i6engine::api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
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
				PhysicsNode::Ptr newPN = boost::make_shared<PhysicsNode>(pnc->_waitForId, pnc->_id, pnc->pos, pnc->rot, pnc->scale, pnc->collisionGroup, pnc->shapeParams, pnc->shatterInterest, this);
				_nodes.insert(std::make_pair(pnc->_waitForId, newPN));
			}
			if (!_nodes[pnc->getWaitID()]->addChild(pnc->getID(), pnc->pos, pnc->rot, pnc->scale, pnc->collisionGroup, pnc->shapeType, pnc->shapeParams)) {
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg); // PhysicsNode couldn't be created, so queue this message again and try it later
			}
		}
	}

	void PhysicsManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::physics::PhyGravity) {
			Vec3 vec3Gravity = static_cast<api::physics::Physics_Gravity_Update *>(msg->getContent())->grav;

			setGravity(vec3Gravity);
		} else if (type == api::physics::PhyPause) {
			_paused = dynamic_cast<api::physics::Physics_Pause_Update *>(msg->getContent())->pause;
		} else {
			ISIXE_THROW_MESSAGE("PhysicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void PhysicsManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		uint16_t type = msg->getSubtype();

		if (type == api::physics::PhyClean) {
			removeAll();
		} else if (type == api::physics::PhyNode) {
			api::physics::Physics_Node_Delete * pnc = static_cast<api::physics::Physics_Node_Delete *>(msg->getContent());

			PhysicsNode * tmp = getPhysicsNode(pnc->_waitForId, pnc->_id).get();

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
		} else if (msg->getSubtype() == api::physics::PhyReset) {
			_ctrl->reset();
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
		double radius = 1.0f;
		double mass = 0.0f;

		assert(params.find("radius") != params.end());

		if (params.find("radius") != params.end()) {
			radius = boost::lexical_cast<double>(params.find("radius")->second);
		}

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("mass") != params.end()) {
			mass = boost::lexical_cast<double>(params.find("mass")->second);
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
			mass = boost::lexical_cast<double>(params.find("mass")->second);
		}

		if (params.find("shapeIndex") != params.end()) {
			shapeIndex = boost::lexical_cast<uint32_t>(params.find("shapeIndex")->second);
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
		double mass = 0.0f;

		assert(params.find("box") != params.end());

		if (params.find("box") != params.end()) {
			box = Vec3(params, "box");
		}

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("mass") != params.end()) {
			mass = boost::lexical_cast<double>(params.find("mass")->second);
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
			planeConstant = boost::lexical_cast<double>(params.find("planeConstant")->second);
		}

		btCollisionShape * groundShape = new btStaticPlaneShape(planeNormal.toBullet(), planeConstant);
		_collisionShapes.push_back(groundShape);

		return groundShape;
	}

	btCollisionShape * PhysicsManager::createMeshStrider(const attributeMap & params) {
		ASSERT_THREAD_SAFETY_FUNCTION

		assert(params.find("mesh") != params.end());

		std::string meshName = params.find("mesh")->second;

		Vec3 localInertia;
		double mass = 0.0;

		if (params.find("localInertia") != params.end()) {
			localInertia = Vec3(params, "localInertia");
		}

		if (params.find("mass") != params.end()) {
			mass = boost::lexical_cast<double>(params.find("mass")->second);
		}

		if (_meshes.find(meshName) == _meshes.end()) {
			_meshes[meshName] = _meshManager->loadMesh(meshName, "i6engine");
		}

		Ogre::Mesh * mp = _meshes.find(meshName)->second.get();

		btCollisionShape * fallShape = new btBvhTriangleMeshShape(new MeshStrider(mp), true);

		btDefaultSerializer *	serializer = new btDefaultSerializer();
		serializer->startSerialization();
		fallShape->serializeSingleShape(serializer);
		serializer->finishSerialization();

		FILE * file = fopen((meshName + ".bullet").c_str(), "wb");
		fwrite(serializer->getBufferPointer(), size_t(serializer->getCurrentBufferSize()), 1, file);
		fclose(file);

		btVector3 inertia = localInertia.toBullet();
		fallShape->calculateLocalInertia(mass, inertia);

		_collisionShapes.push_back(fallShape);

		return fallShape;
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

	void PhysicsManager::removeAll() {
		ASSERT_THREAD_SAFETY_FUNCTION
		removeMeshes();
	}

	void PhysicsManager::removeMeshes() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_meshes.clear();
	}

	PhysicsNode::Ptr PhysicsManager::getPhysicsNode(const int64_t id, const int64_t compId) {
		ASSERT_THREAD_SAFETY_FUNCTION
		if (_nodes.find(id) == _nodes.end()) {
			return PhysicsNode::Ptr();
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
