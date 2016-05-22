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

#include "i6engine/api/components/MoverCircleComponent.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/configs/NetworkChannels.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace api {

	MoverCircleComponent::MoverCircleComponent(const int64_t id, const attributeMap & params) : MoverComponent(id, params), _circleCenter(), _circleAxis() {
		_objComponentID = components::MoverCircleComponent;

		loadParams(params);
	}

	MoverCircleComponent::~MoverCircleComponent() {
	}

	void MoverCircleComponent::start(Vec3 & startPos) {
		_startTime = EngineController::GetSingleton().getCurrentTime();

		_started = true;

		_moving = true;
		if (_positioning == Positioning::POSITIONING_RELATIVE) {
			// for absolute, startPos will be ignored because it doesn't make any sense
			_realStartPos = startPos;
		}

		_lastPos = _circleCenter;

		Vec3 orth1;
		if (fabs(_circleAxis.getX()) < 1e-15 && fabs(_circleAxis.getY()) < 1e-15) {
			// special case
			orth1 = Vec3(- _circleAxis.getZ(), _circleAxis.getX(), _circleAxis.getX()).normalize();
		} else {
			orth1 = Vec3(_circleAxis.getY(), - _circleAxis.getX(), _circleAxis.getZ()).normalize();
		}
		auto psc = _psc.get();
		if (_positioning == Positioning::POSITIONING_RELATIVE) {
			psc->setPosition(_realStartPos + orth1 * _circleRadius, 2);
		} else {
			psc->setPosition(_circleCenter + orth1 * _circleRadius, 2);
		}
		_lastPos = _circleCenter + orth1 * _circleRadius; // the position is now updated

		// resync
		if (getOwnerGO()->getGOC(components::NetworkSenderComponent) != nullptr) {
			attributeMap am = synchronize();
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComMoverResync, core::Method::Update, new components::Component_MoverResync_Update(_objOwnerID, _id, am), core::Subsystem::Object);
			EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
		}
	}

	void MoverCircleComponent::getNewPosition(const uint64_t t, Vec3 & newPos, Quaternion &) {
		double timeElapsed = double(t % _duration) / _duration;
		double alpha = timeElapsed * 2 * PI;
		double diffCos = std::cos(alpha) * _circleRadius;
		double diffSin = std::sin(alpha) * _circleRadius;

		Vec3 orth1;
		if (fabs(_circleAxis.getX()) < 1e-15 && fabs(_circleAxis.getY()) < 1e-15) {
			// special case
			orth1 = Vec3(- _circleAxis.getZ(), _circleAxis.getX(), _circleAxis.getX()).normalize();
		} else {
			orth1 = Vec3(_circleAxis.getY(), - _circleAxis.getX(), _circleAxis.getZ()).normalize();
		}
		Vec3 orth2 = Vec3::crossProd(_circleAxis, orth1).normalize();

		newPos = _circleCenter + orth1 * diffCos + orth2 * diffSin;
	}

	void MoverCircleComponent::News(const GameMessage::Ptr & msg) {
		uint16_t type = msg->getSubtype();
		if (type == api::components::ComMoverResync) {
			attributeMap am = static_cast<components::Component_MoverResync_Update *>(msg->getContent())->attMap;
			stop();

			loadParams(am);
			Vec3 x(am, "realCenterPos");
			start(x);
		} else {
			Component::News(msg);
		}
	}

	void MoverCircleComponent::loadParams(const attributeMap & params) {
		MoverComponent::loadParams(params);

		parseAttribute<true>(params, "pos", _circleCenter);
		parseAttribute<true>(params, "axis", _circleAxis);
		parseAttribute<true>(params, "radius", _circleRadius);
		parseAttribute<false>(params, "realCenterPos", _realStartPos);

		_lastPos = _circleCenter;
	}

	attributeMap MoverCircleComponent::synchronize() const {
		attributeMap params = MoverComponent::synchronize();

		writeAttribute(params, "pos", _circleCenter);
		writeAttribute(params, "axis", _circleAxis);
		writeAttribute(params, "radius", _circleRadius);
		writeAttribute(params, "lastPos", _lastPos);

		return params;
	}

	void MoverCircleComponent::reset() {
		_lastPos = _circleCenter;
	}

	std::vector<componentOptions> MoverCircleComponent::getComponentOptions() {
		std::vector<componentOptions> result = MoverComponent::getComponentOptions();

		result.push_back(std::make_tuple(AccessState::READWRITE, "Circle Center", [this]() {
			return _circleCenter.toString();
		}, [this](std::string s) {
			try {
				_circleCenter = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Circle Axis", [this]() {
			return _circleAxis.toString();
		}, [this](std::string s) {
			try {
				_circleAxis = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Radius", [this]() {
			return boost::lexical_cast<std::string>(_circleRadius);
		}, [this](std::string s) {
			_circleRadius = boost::lexical_cast<double>(s);
			return true;
		}, "Double"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
