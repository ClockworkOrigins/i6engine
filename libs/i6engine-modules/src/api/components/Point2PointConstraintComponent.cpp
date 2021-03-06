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

#include "i6engine/api/components/Point2PointConstraintComponent.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include <cfloat>
#endif

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/PhysicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	Point2PointConstraintComponent::Point2PointConstraintComponent(const int64_t id, const api::attributeMap & params) : Component(id, params), _connected(false), _selfIdentifier(), _targetIdentifier(), _selfOffset(), _targetOffset(), _breakingImpulse(DBL_MAX), _targetID() {
		_objFamilyID = components::Point2PointConstraintComponent;
		_objComponentID = components::Point2PointConstraintComponent;

		parseAttribute<true>(params, "selfIdentifier", _selfIdentifier);
		parseAttribute<true>(params, "targetIdentifier", _targetIdentifier);
		parseAttribute<true>(params, "selfOffset", _selfOffset);
		parseAttribute<true>(params, "targetOffset", _targetOffset);
		parseAttribute<false>(params, "breakingImpulse", _breakingImpulse);
	}

	Point2PointConstraintComponent::~Point2PointConstraintComponent() {
	}

	void Point2PointConstraintComponent::Init() {
		for (auto & p : EngineController::GetSingleton().getObjectFacade()->getGOMap()) {
			auto p2p = p.second->getGOC<Point2PointConstraintComponent>(components::ComponentTypes::Point2PointConstraintComponent);
			if (p2p != nullptr) {
				if (p2p->_targetIdentifier == _selfIdentifier && p2p->_selfIdentifier == _targetIdentifier) {
					if (!p2p->_connected) {
						ISIXE_THROW_API_COND("Point2PointConstraintComponent", "constraint with selfIdentifier '" << _targetIdentifier << "' and targetIdentifier '" << _selfIdentifier << "' don't match each others offsets!", p2p->_selfOffset == _targetOffset && p2p->_targetOffset == _selfOffset);
						EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyP2PConstraint, core::Method::Update, new physics::Physics_P2PConstraint_Create(_objOwnerID, p.first, _selfOffset, _targetOffset), core::Subsystem::Object));
						if (_breakingImpulse < DBL_MAX) {
							EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyConstraintBreakImpulse, core::Method::Update, new physics::Physics_BreakConstraintImpulse_Update(_objOwnerID, p.first, _breakingImpulse), core::Subsystem::Object));
						}
						_targetID = p.first;
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
								EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyP2PConstraint, core::Method::Update, new physics::Physics_P2PConstraint_Create(_objOwnerID, p.first, _selfOffset, _targetOffset), core::Subsystem::Object));
								if (_breakingImpulse < DBL_MAX) {
									EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::PhysicsMessageType, physics::PhyConstraintBreakImpulse, core::Method::Update, new physics::Physics_BreakConstraintImpulse_Update(_objOwnerID, p.first, _breakingImpulse), core::Subsystem::Object));
								}
								_targetID = p.first;
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

		writeAttribute(params, "selfIdentifier", _selfIdentifier);
		writeAttribute(params, "targetIdentifier", _targetIdentifier);
		writeAttribute(params, "selfOffset", _selfOffset);
		writeAttribute(params, "targetOffset", _targetOffset);
		if (_breakingImpulse < DBL_MAX) {
			writeAttribute(params, "breakingImpulse", _breakingImpulse);
		}

		return params;
	}

	std::vector<componentOptions> Point2PointConstraintComponent::getComponentOptions() {
		std::vector<componentOptions> result;
		return result;
	}

	std::pair<AddStrategy, int64_t> Point2PointConstraintComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

} /* namespace api */
} /* namespace i6e */
