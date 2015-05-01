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

#include "i6engine/api/components/Point2PointConstraintComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6engine {
namespace api {

	Point2PointConstraintComponent::Point2PointConstraintComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), _connected(false), _selfIdentifier(), _targetIdentifier(), _selfOffset(), _targetOffset(), _targetID(), _breakingImpulse(DBL_MAX) {
		_objFamilyID = components::Point2PointConstraintComponent;
		_objComponentID = components::Point2PointConstraintComponent;

		_selfIdentifier = params.find("selfIdentifier")->second;
		_targetIdentifier = params.find("targetIdentifier")->second;
		_selfOffset = Vec3(params.find("selfOffset")->second);
		_targetOffset = Vec3(params.find("targetOffset")->second);

		if (params.find("breakingImpulse") != params.end()) {
			_breakingImpulse = std::stod(params.find("breakingImpulse")->second);
		}
	}

	Point2PointConstraintComponent::~Point2PointConstraintComponent() {
	}

	ComPtr Point2PointConstraintComponent::createC(const int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("Point2PointConstraintComponent", "'selfIdentifier' not set!", params.find("selfIdentifier") != params.end());
		ISIXE_THROW_API_COND("Point2PointConstraintComponent", "'targetIdentifier' not set!", params.find("targetIdentifier") != params.end());
		ISIXE_THROW_API_COND("Point2PointConstraintComponent", "'selfOffset' not set!", params.find("selfOffset") != params.end());
		ISIXE_THROW_API_COND("Point2PointConstraintComponent", "'targetOffset' not set!", params.find("targetOffset") != params.end());
		return utils::make_shared<Point2PointConstraintComponent, Component>(id, params);
	}

	void Point2PointConstraintComponent::Init() {
		for (auto & go : EngineController::GetSingleton().getObjectFacade()->getGOList()) {
			auto p2p = go->getGOC<Point2PointConstraintComponent>(components::ComponentTypes::Point2PointConstraintComponent);
			if (p2p != nullptr) {
				if (p2p->_targetIdentifier == _selfIdentifier && p2p->_selfIdentifier == _targetIdentifier) {
					if (!p2p->_connected) {
						ISIXE_THROW_API_COND("Point2PointConstraintComponent", "constraint with selfIdentifier '" << _targetIdentifier << "' and targetIdentifier '" << _selfIdentifier << "' don't match each others offsets!", p2p->_selfOffset == _targetOffset && p2p->_targetOffset == _selfOffset);
						EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyP2PConstraint, core::Method::Update, new physics::Physics_P2PConstraint_Create(_objOwnerID, go->getID(), _selfOffset, _targetOffset), core::Subsystem::Object));
						if (_breakingImpulse != DBL_MAX) {
							EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyConstraintBreakImpulse, core::Method::Update, new physics::Physics_BreakConstraintImpulse_Update(_objOwnerID, go->getID(), _breakingImpulse), core::Subsystem::Object));
						}
						_targetID = go->getID();
						p2p->_connected = true;
						_connected = true;
					} else {
						ISIXE_THROW_API("Point2PointConstraintComponent", "constraint with selfIdentifier '" << _targetIdentifier << "' and targetIdentifier '" << _selfIdentifier << "' already connected. Check your Identifiers.");
					}
					break;
				} else {
					for (auto c : p2p->_subComps) {
						auto p2p2 = utils::dynamic_pointer_cast<Point2PointConstraintComponent>(c);
						if (p2p2->_targetIdentifier == _selfIdentifier && p2p2->_selfIdentifier == _targetIdentifier) {
							if (!p2p2->_connected) {
								ISIXE_THROW_API_COND("Point2PointConstraintComponent", "constraint with selfIdentifier '" << _targetIdentifier << "' and targetIdentifier '" << _selfIdentifier << "' don't match each others offsets!", p2p2->_selfOffset == _targetOffset && p2p2->_targetOffset == _selfOffset);
								EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyP2PConstraint, core::Method::Update, new physics::Physics_P2PConstraint_Create(_objOwnerID, go->getID(), _selfOffset, _targetOffset), core::Subsystem::Object));
								if (_breakingImpulse != DBL_MAX) {
									EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyConstraintBreakImpulse, core::Method::Update, new physics::Physics_BreakConstraintImpulse_Update(_objOwnerID, go->getID(), _breakingImpulse), core::Subsystem::Object));
								}
								_targetID = go->getID();
								p2p2->_connected = true;
								_connected = true;
							} else {
								ISIXE_THROW_API("Point2PointConstraintComponent", "constraint with selfIdentifier '" << _targetIdentifier << "' and targetIdentifier '" << _selfIdentifier << "' already connected. Check your Identifiers.");
							}
							break;
						}
					}
				}
			}
		}
	}

	void Point2PointConstraintComponent::Finalize() {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyConstraint, core::Method::Delete, new physics::Physics_Constraint_Delete(_objOwnerID, _targetID), core::Subsystem::Object));
	}

	attributeMap Point2PointConstraintComponent::synchronize() const {
		attributeMap params;

		params.insert(std::make_pair("selfIdentifier", _selfIdentifier));
		params.insert(std::make_pair("targetIdentifier", _targetIdentifier));
		_selfOffset.insertInMap("selfOffset", params);
		_targetOffset.insertInMap("targetOffset", params);
		if (_breakingImpulse != DBL_MAX) {
			params.insert(std::make_pair("breakingImpulse", std::to_string(_breakingImpulse)));
		}

		return params;
	}

	std::vector<componentOptions> Point2PointConstraintComponent::getComponentOptions() {
		std::vector<componentOptions> result;
		return result;
	}

	std::pair<AddStrategy, int64_t> Point2PointConstraintComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

} /* namespace api */
} /* namespace i6engine */
