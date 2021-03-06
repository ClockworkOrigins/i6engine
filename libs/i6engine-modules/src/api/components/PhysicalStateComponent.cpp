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

#include "i6engine/api/components/PhysicalStateComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/ShatterComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#undef max

namespace i6e {
namespace api {

	PhysicalStateComponent::PhysicalStateComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _positionNew(), _posDirty(0), _rotDirty(0), _speedDirty(0), _rotation(), _scale(), _scaleNew(), _scaleDirty(0), _linearVelocity(0.0, 0.0, 0.0), _forces(), _gravity(), _collisionGroup(), _shapeType(), _shapeParams(params), _initialized(false), _shatterInterest(), _lock(), _syncPrio(0) {
		_objFamilyID = components::PhysicalStateComponent;
		_objComponentID = components::PhysicalStateComponent;

		_shapeParams.erase("identifier");

		bool compound;
		parseAttribute<true>(params, "collisionGroup", _collisionGroup);
		parseAttribute<true>(params, "pos", _position);
		parseAttribute<true>(params, "rot", _rotation);
		parseAttribute<true>(params, "scale", _scale);
		parseAttribute<true>(params, "shapeType", _shapeType);
		parseAttribute<true>(params, "shatterInterest", _shatterInterest);
		parseAttribute<true>(params, "compound", compound);
		parseAttribute<false>(params, "syncPrio", _syncPrio);
		parseAttribute<false>(params, "gravity", _gravity);
	}

	PhysicalStateComponent::~PhysicalStateComponent() {
	}

	void PhysicalStateComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNode, core::Method::Create, new physics::Physics_Node_Create(_objOwnerID, getID(), _position, _rotation, _scale, _collisionGroup, _shapeType, _shapeParams, _shatterInterest), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		GameMessage::Ptr msg2 = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Create, new graphics::Graphics_Node_Create(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg2);

		if (_gravity.isValid()) {
			setGravity(_gravity);
		}

		_initialized = true;

		addTicker();
	}

	void PhysicalStateComponent::Finalize() {
		removeTicker();

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNode, core::Method::Delete, new physics::Physics_Node_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	Vec3 PhysicalStateComponent::getPosition() const {
		return _position;
	}

	void PhysicalStateComponent::setPosition(const Vec3 & position, uint32_t prio) {
		boost::mutex::scoped_lock sl(_lock);
		if (prio >= _posDirty && _positionNew != position && (_positionNew - position).length() > 0.001) {
			_positionNew = position;
			_posDirty = prio;
		}
	}

	void PhysicalStateComponent::setRotation(const Quaternion & rotation, uint32_t prio) {
		boost::mutex::scoped_lock sl(_lock);
		if (prio >= _rotDirty && _rotationNew != rotation) {
			// FIXME: (Michael) better definition of 'same rotation'
			Vec3 r11 = math::rotateVector(Vec3(10.0, 10.0, 10.0), _rotationNew);
			Vec3 r22 = math::rotateVector(Vec3(10.0, 10.0, 10.0), rotation);
			
			if ((r11 - r22).length() > 0.01) {
				_rotationNew = rotation;
				_rotDirty = prio;
			}
		}
	}

	void PhysicalStateComponent::applyRotation(const Quaternion & rotation) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyApplyRotation, core::Method::Update, new physics::Physics_ApplyRotation_Update(_objOwnerID, getID(), rotation), core::Subsystem::Object);
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void PhysicalStateComponent::setScale(const Vec3 & scale, uint32_t prio) {
		boost::mutex::scoped_lock sl(_lock);
		if (prio >= _scaleDirty && _scaleNew != scale) {
			_scaleNew = scale;
			_scaleDirty = prio;
		}
	}

	void PhysicalStateComponent::setCollisionFlags(const CollisionGroup & col) {
		_collisionGroup = col;
		if (_initialized) {
			GameMessage::Ptr msgLocal = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyCollision, core::Method::Update, new physics::Physics_Collision_Update(_objOwnerID, getID(), col), core::Subsystem::Object);

			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msgLocal);
		}
	}

	void PhysicalStateComponent::reset() {
		setLinearVelocity(Vec3(0.0, 0.0, 0.0), UINT32_MAX);

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyResetForces, core::Method::Update, new physics::Physics_Reset_Update(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		boost::mutex::scoped_lock sl(_lock);
		_forces.clear();
	}

	void PhysicalStateComponent::setCollisionShape(ShapeType st, const attributeMap & params) {
		_shapeType = st;
		_shapeParams = params;
	}

	void PhysicalStateComponent::setLinearVelocity(const Vec3 & linVel, uint32_t prio) {
		boost::mutex::scoped_lock sl(_lock);
		if (prio >= _speedDirty && _speedNew != linVel && (_speedNew - linVel).length() > 0.001) {
			_speedNew = linVel;
			_speedDirty = prio;
		}
	}

	void PhysicalStateComponent::applyCentralForce(const Vec3 & cForce, bool forceIsLocalSpace) {
		boost::mutex::scoped_lock sl(_lock);
		_forces.push_back(std::make_tuple(cForce, Vec3::ZERO, forceIsLocalSpace));
	}

	void PhysicalStateComponent::applyForce(const Vec3 & force, const Vec3 & offset, bool forceIsLocalSpace) {
		boost::mutex::scoped_lock sl(_lock);
		_forces.push_back(std::make_tuple(force, offset, forceIsLocalSpace));
	}

	void PhysicalStateComponent::setGravity(const Vec3 & gravity) {
		_gravity = gravity;
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNodeGravity, core::Method::Update, new physics::Physics_NodeGravity_Update(_objOwnerID, getID(), gravity), core::Subsystem::Object);
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void PhysicalStateComponent::News(const GameMessage::Ptr & msg) {
		if (msg->getMessageType() == messages::PositionMessageType) {
			if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
				setPosition(static_cast<objects::Position_Update *>(msg->getContent())->pos, UINT32_MAX);
				setRotation(static_cast<objects::Position_Update *>(msg->getContent())->rot, UINT32_MAX);
				_scale = static_cast<objects::Position_Update *>(msg->getContent())->scale;
			} else {
				ISIXE_THROW_MESSAGE("PhysicalStateComponent", "Unknown Message: " << msg->getSubtype());
			}
		} else if (msg->getMessageType() == messages::ComponentMessageType) {
			if (msg->getSubtype() == physics::PhyVelForce) {
				_linearVelocity = static_cast<physics::Physics_Comp_Update *>(msg->getContent())->linVel;
			} else if (msg->getSubtype() == components::ComShatter) {
				ComPtr sc = getOwnerGO()->getGOC(components::ShatterComponent);
				if (sc != nullptr) {
					sc->News(msg);
				}
			} else {
				Component::News(msg);
			}
		}
	}

	void PhysicalStateComponent::Tick() {
		bool isDirty = false;
		uint32_t netPrio = 0;
		{
			boost::mutex::scoped_lock sl(_lock);
			if (_posDirty > 0) {
				_position = _positionNew;
				if (_posDirty > 1) {
					GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNodePosition, core::Method::Update, new physics::Physics_NodePosition_Update(_objOwnerID, getID(), _position), core::Subsystem::Object);
					EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
				netPrio = std::max(netPrio, _posDirty);
				_posDirty = 0;
				isDirty = true;
			}
			if (_rotDirty > 0) {
				_rotation = _rotationNew;
				if (_rotDirty > 1) {
					GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNode, core::Method::Update, new physics::Physics_Node_Update(_objOwnerID, getID(), _rotation, _scale), core::Subsystem::Object);
					EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
				netPrio = std::max(netPrio, _rotDirty);
				_rotDirty = 0;
				isDirty = true;
			}
			if (_speedDirty > 0) {
				_linearVelocity = _speedNew;
				if (_speedDirty > 1) {
					GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyLinearVelocity, core::Method::Update, new physics::Physics_LinearVelocity_Update(_objOwnerID, getID(), _linearVelocity), core::Subsystem::Object);
					EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
				_speedDirty = 0;
			}
			if (_scaleDirty > 0) {
				_scale = _scaleNew;
				if (_scaleDirty > 1) {
					GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNode, core::Method::Update, new physics::Physics_Node_Update(_objOwnerID, getID(), _rotation, _scale), core::Subsystem::Object);
					EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				}
				netPrio = std::max(netPrio, _scaleDirty);
				_scaleDirty = 0;
				isDirty = true;
			}

			if (_forces.size() > 0) {
				GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyForce, core::Method::Update, new physics::Physics_Force_Update(_objOwnerID, getID(), _forces), core::Subsystem::Object);
				EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
				_forces.clear();
			}
		}

		if (isDirty) {
			// send message to graphic
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Update, new graphics::Graphics_Node_Update(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object);

			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

			GOPtr go = getOwnerGO();

			// send message into network
			if (go != nullptr && go->getGOC(components::NetworkSenderComponent) != nullptr && _syncPrio <= netPrio) {
				GameMessage::Ptr msg2 = boost::make_shared<GameMessage>(messages::PositionMessageType, objects::Position, core::Method::Update, new objects::Position_Update(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object);

				EngineController::GetSingletonPtr()->getNetworkFacade()->publish(POSITION_CHANNEL, msg2);
			}
		}
	}

	void PhysicalStateComponent::setShatterInterest(ShatterInterest si) {
		_shatterInterest = si;
	}

	attributeMap PhysicalStateComponent::synchronize() const {
		attributeMap params = _shapeParams;
		writeAttribute(params, "pos", _position);
		writeAttribute(params, "rot", _rotation);
		writeAttribute(params, "scale", _scale);
		writeAttribute(params, "collisionGroup", _collisionGroup);
		writeAttribute(params, "shapeType", _shapeType);
		writeAttribute(params, "shatterInterest", _shatterInterest);
		if (_gravity.isValid()) {
			writeAttribute(params, "gravity", _gravity);
		}
		if (_syncPrio != 0) {
			writeAttribute(params, "syncPrio", _syncPrio);
		}
		return params;
	}

	void PhysicalStateComponent::rayTest(const Vec3 & from, const Vec3 & to, RayTestRepetition rtr, RayTestNotify rtn, const GameMessage::Ptr & msg) {
		GameMessage::Ptr message = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyRayTest, core::Method::Update, new physics::Physics_RayTest_Update(_objOwnerID, _id, from, to, rtr, rtn, msg), core::Subsystem::Object);
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(message);
	}

	std::pair<AddStrategy, int64_t> PhysicalStateComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	void PhysicalStateComponent::addPosition(const Vec3 & pos) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyAddPosition, core::Method::Update, new physics::Physics_AddPosition_Update(_objOwnerID, getID(), pos), core::Subsystem::Object);
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	std::vector<componentOptions> PhysicalStateComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Position", [this]() {
			return _position.toString();
		}, [this](std::string s) {
			try {
				_position = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNodePosition, core::Method::Update, new physics::Physics_NodePosition_Update(_objOwnerID, getID(), _position), core::Subsystem::Object));
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Update, new graphics::Graphics_Node_Update(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object));
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Rotation", [this]() {
			std::stringstream ss;
			ss << _rotation;
			return ss.str();
		}, [this](std::string s) {
			try {
				_rotation = Quaternion(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNode, core::Method::Update, new physics::Physics_Node_Update(_objOwnerID, getID(), _rotation, _scale), core::Subsystem::Object));
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Update, new graphics::Graphics_Node_Update(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object));
			return true;
		}, "Quaternion"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Scale", [this]() {
			return _scale.toString();
		}, [this](std::string s) {
			try {
				_scale = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsNodeMessageType, physics::PhyNode, core::Method::Update, new physics::Physics_Node_Update(_objOwnerID, getID(), _rotation, _scale), core::Subsystem::Object));
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Update, new graphics::Graphics_Node_Update(_objOwnerID, getID(), _position, _rotation, _scale), core::Subsystem::Object));
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Gravity", [this]() {
			return _gravity.toString();
		}, [this](std::string s) {
			try {
				_gravity = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setGravity(_gravity);
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Collision Group", [this]() {
			std::stringstream ss;
			ss << _collisionGroup.responseType << " " << _collisionGroup.crashType << " " << _collisionGroup.crashMask;
			return ss.str();
		}, [this](std::string s) {
			setCollisionFlags(CollisionGroup(s));
			return true;
		}, "String"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Shape Type", [this]() {
			return boost::lexical_cast<std::string>(uint16_t(_shapeType));
		}, [this](std::string s) {
			_shapeType = ShapeType(boost::lexical_cast<uint16_t>(s));
			return true;
		}, "ShapeType"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Shatter Interest", [this]() {
			return boost::lexical_cast<std::string>(uint16_t(_shatterInterest));
		}, [this](std::string s) {
			_shatterInterest = ShatterInterest(boost::lexical_cast<uint16_t>(s));
			return true;
		}, "ShatterInterest"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Sync. Priority", [this]() {
			return boost::lexical_cast<std::string>(_syncPrio);
		}, [this](std::string s) {
			_syncPrio = boost::lexical_cast<uint32_t>(s);
			return true;
		}, "Integer"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
