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

#include "i6engine/modules/physics/PhysicsNode.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/modules/physics/PhysicsManager.h"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace i6engine {
namespace modules {

	PhysicsNode::PhysicsNode(const int64_t id, const int64_t compId, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale, const api::CollisionGroup & cg, const api::attributeMap & params, api::ShatterInterest shatterInterest, PhysicsManager * pm) : _manager(pm), _id(id), _compId(compId), _collisionGroup(cg), _position(position), _rotation(rotation), _scale(scale), _transform(rotation.toBullet(), position.toBullet()), _rigidBody(), _oldScale(1.0f, 1.0f, 1.0f), _linearVelocity(), _centralForce(), _shatterInterest(shatterInterest), _parentShape(nullptr), _childShapes() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		if (std::stoi(params.find("compound")->second)) {
			_parentShape = _manager->createCompound();
		}
	}

	PhysicsNode::~PhysicsNode() {
		ASSERT_THREAD_SAFETY_FUNCTION
		stopSimulatingBody();

		assert(_rigidBody != nullptr);

		_manager->getPhysicsWorld()->removeCollisionObject(_rigidBody);

		delete _rigidBody;

		if (!_rayTests.empty()) {
			removeTicker();
		}

		for (PeriodicRaytest * pr : _rayTests) {
			delete pr;
		}
	}

	bool PhysicsNode::addChild(const int64_t compId, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale, const api::CollisionGroup & cg, const api::PhysicalStateComponent::ShapeType shapeType, const attributeMap & shapeParams) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		btCollisionShape * newShape = nullptr;
		switch (shapeType) {
		case api::PhysicalStateComponent::ShapeType::PLANE: {
			newShape = _manager->createPlane(shapeParams);
			break;
		}
		case api::PhysicalStateComponent::ShapeType::BOX: {
			newShape = _manager->createBox(shapeParams);
			break;
		}
		case api::PhysicalStateComponent::ShapeType::SPHERE: {
			newShape = _manager->createSphere(shapeParams);
			break;
		}
		case api::PhysicalStateComponent::ShapeType::FILE: {
			newShape = _manager->createFromFile(shapeParams);
			break;
		}
		default: {
			assert(false);
			break;
		}
		}

		if (newShape == nullptr) {
			return false;
		}

		if (_childShapes.empty()) {
			Vec3 angularFactor(1.0, 1.0, 1.0);
			Vec3 linearFactor(1.0, 1.0, 1.0);
			double mass = 0.0;
			double restitution = 0.0;
			double friction = 0.0;
			int activationState = ACTIVE_TAG;
			double linearDamping = 0.0;
			double angularDamping = 0.0;
			Vec3 localInertia;

			if (shapeParams.find("localInertia") != shapeParams.end()) {
				localInertia = Vec3(shapeParams, "localInertia");
			}

			if (shapeParams.find("angularFactor") != shapeParams.end()) {
				angularFactor = Vec3(shapeParams, "angularFactor");
			}

			if (shapeParams.find("linearFactor") != shapeParams.end()) {
				linearFactor = Vec3(shapeParams, "linearFactor");
			}

			if (shapeParams.find("mass") != shapeParams.end()) {
				mass = std::stod(shapeParams.find("mass")->second);
			}

			if (shapeParams.find("restitution") != shapeParams.end()) {
				restitution = std::stod(shapeParams.find("restitution")->second);
			}

			if (shapeParams.find("friction") != shapeParams.end()) {
				friction = std::stod(shapeParams.find("friction")->second);
			}

			if (shapeParams.find("activationState") != shapeParams.end()) {
				activationState = std::stoi(shapeParams.find("activationState")->second);
			}

			if (shapeParams.find("linearDamping") != shapeParams.end()) {
				linearDamping = std::stod(shapeParams.find("linearDamping")->second);
			}

			if (shapeParams.find("angularDamping") != shapeParams.end()) {
				angularDamping = std::stod(shapeParams.find("angularDamping")->second);
			}

			_transform = btTransform(rotation.toBullet(), position.toBullet());

			if (_parentShape != nullptr) {
				btVector3 inertia(localInertia.toBullet());
				if (mass > 0.0 && !localInertia.isValid()) {
					newShape->calculateLocalInertia(mass, inertia);
				}
				btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, this, _parentShape, inertia);
				_rigidBody = new btRigidBody(groundRigidBodyCI);
			} else {
				btVector3 inertia(localInertia.toBullet());
				if (mass > 0.0 && !localInertia.isValid()) {
					newShape->calculateLocalInertia(mass, inertia);
				}
				btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, this, newShape, inertia);
				_rigidBody = new btRigidBody(groundRigidBodyCI);
			}
			_rigidBody->setFriction(friction);
			_rigidBody->setRestitution(restitution);
			_rigidBody->setActivationState(activationState);
			_rigidBody->setLinearFactor(linearFactor.toBullet());
			_rigidBody->setAngularFactor(angularFactor.toBullet());
			_rigidBody->setDamping(linearDamping, angularDamping);

			_rigidBody->proceedToTransform(_transform);

			_rigidBody->setUserPointer(reinterpret_cast<void *>(this));

			_rigidBody->getCollisionShape()->setLocalScaling(_scale.toBullet());

			startSimulatingBody();

			_rigidBody->getBroadphaseProxy()->m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;

			_rigidBody->setCollisionFlags(0);
			// Object sould not physically react to collisions
			if (cg.responseType & api::ResponseType::GHOST) {
				_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			}
			// Object is static (no response to gravitiy and other forces)
			if (cg.responseType & api::ResponseType::STATIC) {
				_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
			}
			// bullet should call callback on collision
			if (cg.responseType & api::ResponseType::TRIGGER) {
				_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			}

			if (_parentShape != nullptr) {
				newShape->setLocalScaling(_scale.toBullet());
				btTransform transform;
				transform.setIdentity();
				_childShapes.insert(std::make_pair(compId, std::make_pair(_parentShape->getNumChildShapes(), newShape)));
				_parentShape->addChildShape(transform, newShape);
			}
		} else {
			newShape->setLocalScaling(_scale.toBullet());

			_childShapes.insert(std::make_pair(compId, std::make_pair(_parentShape->getNumChildShapes(), newShape)));
			_parentShape->addChildShape(btTransform(rotation.toBullet(), position.toBullet()), newShape);
		}

		return true;
	}

	void PhysicsNode::removeChild(int64_t id) {
		if (_parentShape != nullptr) {
			_parentShape->removeChildShape(_childShapes[id].second);
			_manager->removeShape(_childShapes[id].second);
			_childShapes.erase(id);
		}
	}

	bool PhysicsNode::empty() const {
		return _childShapes.empty();
	}

	void PhysicsNode::setPosition(const Vec3 & position, int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (id == _compId) {
			_position = position;
			_transform.setOrigin(position.toBullet());

			if (_rigidBody && !_rigidBody->isKinematicObject()) {
				_rigidBody->proceedToTransform(_transform);
			}
		} else {
			_parentShape->updateChildTransform(int(_childShapes[id].first), btTransform(_parentShape->getChildTransform(int(_childShapes[id].first)).getRotation(), position.toBullet()));
		}
	}

	void PhysicsNode::setRotation(const Quaternion & rotation, int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (id == _compId) {
			_transform.setRotation((rotation.normalize()).toBullet());

			// TODO (???): nur fuer statische objekte
			// das auskommentierte geht irgendwie nicht mehr
			if (_rigidBody /*&& !_rigidBody->isKinematicObject()*/) {
				_rigidBody->proceedToTransform(_transform);
			}
		} else {
			_parentShape->updateChildTransform(int(_childShapes[id].first), btTransform(rotation.toBullet(), _parentShape->getChildTransform(int(_childShapes[id].first)).getOrigin()));
		}
	}

	void PhysicsNode::applyRotation(const Quaternion & rotation, int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 oldSpeed(_rigidBody->getLinearVelocity());

		Vec3 newSpeed = math::rotateVector(oldSpeed, rotation);
		_rigidBody->setLinearVelocity(newSpeed.toBullet());
		setRotation(getRotation() * rotation, id);
	}

	void PhysicsNode::setScale(const Vec3 & scale, int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_rigidBody == nullptr) {
			return;
		}

		if (id == _compId) {
			_scale = scale;
		}

		if (_parentShape == nullptr) {
			_rigidBody->getCollisionShape()->setLocalScaling(scale.toBullet());
		} else {
			_childShapes[id].second->setLocalScaling(scale.toBullet());
		}
	}

	void PhysicsNode::setCollisionFlags(const api::CollisionGroup & cg) {
		_collisionGroup = cg;
		_rigidBody->getBroadphaseProxy()->m_collisionFilterMask = btBroadphaseProxy::DefaultFilter;

		_rigidBody->setCollisionFlags(0);
		// Object sould not physically react to collisions
		if (cg.responseType & api::ResponseType::GHOST) {
			_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		// Object ist static (no response to gravitiy and other forces)
		if (cg.responseType & api::ResponseType::STATIC) {
			_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		}
		// bullet should call callback on collision
		if (cg.responseType & api::ResponseType::TRIGGER) {
			_rigidBody->setCollisionFlags(_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		}
	}

	void PhysicsNode::reset() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_rigidBody->clearForces();
		_rigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	}

	void PhysicsNode::setWorldTransform(const btTransform & worldTrans) {
		ASSERT_THREAD_SAFETY_FUNCTION
		// Get our own representation of the world transform.
		Vec3 position = Vec3(worldTrans.getOrigin());
		Quaternion rotation = Quaternion(worldTrans.getRotation());

		_transform = worldTrans;

		api::GOPtr go = api::EngineController::GetSingletonPtr()->getObjectFacade()->getObject(_id);

		if (go == nullptr) {
			return;
		}

		utils::sharedPtr<api::PhysicalStateComponent, api::Component> pC = go->getGOC<api::PhysicalStateComponent>(api::components::PhysicalStateComponent);

		if (_rigidBody && !_rigidBody->isKinematicObject()) {
			_rigidBody->proceedToTransform(_transform);

			if (pC == nullptr) {
				ISIXE_LOG_WARN("PhysicsNode", "GameObject doesn't exist any more");
				return;
			}
			pC->setLinearVelocity(Vec3(_rigidBody->getLinearVelocity()), 1);
		}

		if (_position != position && (_position - position).length() > 0.001) {
			_position = position;

			pC->setPosition(position, 1);
		}
		if (_rotation != rotation) {
			// FIXME: (Michael) better definition of 'same rotation'
			Quaternion r1(_rotation.toBullet() * Vec3(10, 10, 10).toBullet() * _rotation.toBullet().inverse());
			Quaternion r2(rotation.toBullet() * Vec3(10, 10, 10).toBullet() * rotation.toBullet().inverse());
			Vec3 r11 = r1.toVector();
			Vec3 r22 = r2.toVector();

			if ((r11 - r22).length() > 0.01) {
				_rotation = rotation;

				pC->setRotation(rotation, 1);
			}
		}
	}

	void PhysicsNode::startSimulatingBody() {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_rigidBody != nullptr);

		_manager->addRigidBody(_rigidBody);
	}

	void PhysicsNode::stopSimulatingBody() {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_rigidBody != nullptr);
		_manager->remove(_rigidBody);
		_manager->removeRigidBody(_rigidBody);
	}

	api::RayTestResult PhysicsNode::HitTest(const Vec3 & from, const Vec3 & to) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Vec3 realFrom = math::rotateVector(from, _rotation);
		Vec3 realTo = math::rotateVector(to, _rotation);
		btCollisionWorld::ClosestRayResultCallback RayCallback((_position + realFrom).toBullet(), (_position + realFrom + realTo).toBullet());

		_manager->getPhysicsWorld()->rayTest((_position + realFrom).toBullet(), (_position + realFrom + realTo).toBullet(), RayCallback);

		api::RayTestResult rtr;
		rtr.sourceID = _id;

		if (RayCallback.hasHit()) {
			rtr.objID = static_cast<PhysicsNode *>(RayCallback.m_collisionObject->getUserPointer())->getID();
			rtr.collisionPoint = Vec3(RayCallback.m_hitPointWorld);
		}

		return rtr;
	}

	void PhysicsNode::News(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(msg->getMessageType() == api::messages::PhysicsNodeMessageType);

		if (msg->getMessageType() == api::messages::PhysicsNodeMessageType) {
			if (msg->getSubtype() == api::physics::PhyCollision) {
				setCollisionFlags(static_cast<api::physics::Physics_Collision_Update *>(msg->getContent())->flags);
			} else if (msg->getSubtype() == api::physics::PhyNode) {
				api::physics::Physics_Node_Update * pu = static_cast<api::physics::Physics_Node_Update *>(msg->getContent());

				
				setRotation(pu->rot, pu->getID());
				setScale(pu->scale, pu->getID());

				if (pu->getID() == _compId) {
					_rotation = pu->rot;
				}
			} else if (msg->getSubtype() == api::physics::PhyNodePosition) {
				api::physics::Physics_NodePosition_Update * pu = static_cast<api::physics::Physics_NodePosition_Update *>(msg->getContent());

				setPosition(pu->pos, pu->getID());
			} else if (msg->getSubtype() == api::physics::PhyAddPosition) {
				api::physics::Physics_AddPosition_Update * pu = static_cast<api::physics::Physics_AddPosition_Update *>(msg->getContent());

				setPosition(pu->pos, pu->getID());
			} else if (msg->getSubtype() == api::physics::PhyLinearVelocity) {
				api::physics::Physics_LinearVelocity_Update * pu = static_cast<api::physics::Physics_LinearVelocity_Update *>(msg->getContent());

				_linearVelocity = pu->linVel;

				_rigidBody->setLinearVelocity(_linearVelocity.toBullet());
			} else if (msg->getSubtype() == api::physics::PhyForce) {
				api::physics::Physics_Force_Update * pu = static_cast<api::physics::Physics_Force_Update *>(msg->getContent());

				for (size_t i = 0; i < pu->forces.size(); i++) {
					btVector3 force = std::get<0>(pu->forces[i]).toBullet();
					if (std::get<2>(pu->forces[i])) {
						force = (_rigidBody->getOrientation() * force * _rigidBody->getOrientation().inverse()).getAxis();
					}
					btVector3 offset = std::get<1>(pu->forces[i]).toBullet();
					offset = (_rigidBody->getOrientation() * offset * _rigidBody->getOrientation().inverse()).getAxis();
					_rigidBody->applyForce(force, offset);
				}
			} else if (msg->getSubtype() == api::physics::PhyApplyRotation) {
				api::physics::Physics_ApplyRotation_Update * pu = static_cast<api::physics::Physics_ApplyRotation_Update *>(msg->getContent());
				applyRotation(pu->rot, pu->getID());
			} else if (msg->getSubtype() == api::physics::PhyNodeGravity) {
				api::physics::Physics_NodeGravity_Update * pu = static_cast<api::physics::Physics_NodeGravity_Update *>(msg->getContent());
				_rigidBody->setGravity(pu->gravity.toBullet());
			} else if (msg->getSubtype() == api::physics::PhyResetForces) {
				reset();
			} else if (msg->getSubtype() == api::physics::PhyRayTest) {
				api::physics::Physics_RayTest_Update * pru = dynamic_cast<api::physics::Physics_RayTest_Update *>(msg->getContent());

				if (pru->raytestRepetition == api::PhysicalStateComponent::RayTestRepetition::PERIODIC) {
					bool found = false;
					if (_rayTests.empty()) {
						addTicker();
					}
					for (unsigned int i = 0; i < _rayTests.size(); i++) {
						if (pru->from == _rayTests[i]->from && pru->to == _rayTests[i]->to) {
							delete _rayTests[i];
							_rayTests[i] = new PeriodicRaytest(pru->from, pru->to, pru->raytestNotify, pru->message);
							found = true;
							break;
						}
					}
					if (!found) {
						_rayTests.push_back(new PeriodicRaytest(pru->from, pru->to, pru->raytestNotify, pru->message));
					}
				} else if (pru->raytestRepetition == api::PhysicalStateComponent::RayTestRepetition::STOP) {
					for (size_t i = 0; i < _rayTests.size(); i++) {
						if (pru->from == _rayTests[i]->from && pru->to == _rayTests[i]->to) {
							delete _rayTests[i];
							_rayTests.erase(_rayTests.begin() + int(i));
							break;
						}
					}

					if (_rayTests.empty()) {
						removeTicker();
					}
				} else if (pru->raytestRepetition == api::PhysicalStateComponent::RayTestRepetition::ONCE) {
					dynamic_cast<api::physics::Physics_RayTestNotify_Update *>(pru->message->getContent())->raytestResult = HitTest(pru->from, pru->to);

					api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(pru->message);
				}
			} else {
				ISIXE_THROW_MESSAGE("PhysicsNode", "Unknown message received: " << msg->getMessageInfo());
			}
		}
	}

	void PhysicsNode::Tick() {
		for (PeriodicRaytest * pr : _rayTests) {
			api::RayTestResult rtr = HitTest(pr->from, pr->to);

			if (pr->raytestNotify == api::PhysicalStateComponent::RayTestNotify::ALWAYS) {
				api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(*pr->message);
				dynamic_cast<api::physics::Physics_RayTestNotify_Update *>(msg->getContent())->raytestResult = rtr;
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			} else if (pr->raytestNotify == api::PhysicalStateComponent::RayTestNotify::CHANGE) {
				if ((pr->lastResult.objID < 0 && rtr.objID >= 0) || (pr->lastResult.objID >= 0 && rtr.objID < 0)) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(*pr->message);
					dynamic_cast<api::physics::Physics_RayTestNotify_Update *>(msg->getContent())->raytestResult = rtr;
					api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
			} else if (pr->raytestNotify == api::PhysicalStateComponent::RayTestNotify::FOUND) {
				if (rtr.objID >= 0) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(*pr->message);
					dynamic_cast<api::physics::Physics_RayTestNotify_Update *>(msg->getContent())->raytestResult = rtr;
					api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
			} else if (pr->raytestNotify == api::PhysicalStateComponent::RayTestNotify::NOTFOUND) {
				if (rtr.objID < 0) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(*pr->message);
					dynamic_cast<api::physics::Physics_RayTestNotify_Update *>(msg->getContent())->raytestResult = rtr;
					api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
			} else if (pr->raytestNotify == api::PhysicalStateComponent::RayTestNotify::OBJECTCHANGE) {
				if (rtr.objID != pr->lastResult.objID) {
					api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(*pr->message);
					dynamic_cast<api::physics::Physics_RayTestNotify_Update *>(msg->getContent())->raytestResult = rtr;
					api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
			}

			pr->lastResult = rtr;
		}
	}

	void PhysicsNode::addTicker() {
		_manager->addTicker(this);
	}

	void PhysicsNode::removeTicker() {
		_manager->removeTicker(this);
	}

} /* namespace modules */
} /* namespace i6engine */
