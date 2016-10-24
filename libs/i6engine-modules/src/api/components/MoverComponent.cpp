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

#include "i6engine/api/components/MoverComponent.h"

#include "i6engine/math/i6eMath.h"

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

	MoverComponent::MoverComponent(const int64_t id, const attributeMap & params) : ShatterComponent(id, params), _duration(0), _positioning(), _psc(), _startTime(), _lastPos(), _realStartPos(), _lastRot(), _moving(false), _initial(true), _started(false), _linkable(false), _linked() {
		_objFamilyID = components::MoverComponent;
	}

	MoverComponent::~MoverComponent() {
	}

	void MoverComponent::Init() {
		addTicker();
		_psc = getOwnerGO()->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);
		if (_psc == nullptr) {
			ISIXE_THROW_FAILURE("MoverComponent", "Object has no PhysicalStateComponent");
			return;
		}
		if (!_started) {
			return;
		}
		if (_initial) { // do nothing. initial respawn will start it
			_initial = false;
			start(_realStartPos);
		} else {
			uint64_t a = _startTime;
			start(_realStartPos);
			_startTime = a;
		}
	}

	void MoverComponent::Finalize() {
		removeTicker();
	}

	void MoverComponent::stop() {
		_moving = false;
		_started = false;
	}

	void MoverComponent::Tick() {
		if (!_started || !_moving) {
			return;
		}
		// elapsed time since start of movement
		uint64_t timeElapsed = EngineController::GetSingleton().getCurrentTime() - _startTime;

		Vec3 newPos;
		Quaternion newRot;
		getNewPosition(timeElapsed, newPos, newRot);

		for (int64_t i : _linked) {
			utils::sharedPtr<PhysicalStateComponent, Component> psc = EngineController::GetSingleton().getObjectFacade()->getObject(i)->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);
			if (psc == nullptr) {
				return;
			}
			psc->addPosition(newPos - _lastPos);
			psc->applyRotation(_lastRot.inverse() * newRot);
		}

		auto psc = _psc.get();

		// Update position
		if (_positioning == Positioning::POSITIONING_ABSOLUTE) {
			psc->setPosition(newPos, 2);
			psc->setRotation(newRot, 2);
			_lastPos = newPos;
			_lastRot = newRot;
		} else {
			psc->setPosition(psc->getPosition() + (newPos - _lastPos), 2);
			psc->setRotation(psc->getRotation() * (_lastRot.inverse() * newRot), 2);
			_lastPos = newPos;
			_lastRot = newRot;
		}
	}

	void MoverComponent::loadParams(const attributeMap & params) {
		parseAttribute<true>(params, "duration", _duration);
		parseAttribute<true>(params, "positioning", _positioning);

		// movement continues (after sync)
		if (params.find("continue") != params.end() && params.at("continue") == "true") {
			_initial = false;
			parseAttribute<true>(params, "startTime", _startTime);
		}

		parseAttribute<false>(params, "lastPos", _lastPos);

		if (_positioning == Positioning::POSITIONING_ABSOLUTE) {
			_realStartPos = _lastPos;
		}

		parseAttribute<true>(params, "started", _started);
		parseAttribute<false>(params, "linkable", _linkable);
	}

	attributeMap MoverComponent::synchronize() const {
		attributeMap params;

		// other Component only needs to resume
		writeAttribute(params, "continue", true);
		writeAttribute(params, "startTime", _startTime);
		writeAttribute(params, "duration", _duration);
		writeAttribute(params, "positioning", _positioning);
		writeAttribute(params, "started", _started);
		writeAttribute(params, "linkable", _linkable);
		writeAttribute(params, "realCenterPos", _realStartPos);

		return params;
	}

	std::pair<AddStrategy, int64_t> MoverComponent::howToAdd(const ComPtr & comp) const {
		if (utils::dynamic_pointer_cast<MoverComponent>(comp)->_positioning == Positioning::POSITIONING_RELATIVE) {
			return std::make_pair(AddStrategy::ADD, 0);
		} else {
			if (_positioning == Positioning::POSITIONING_ABSOLUTE) {
				return std::make_pair(AddStrategy::REPLACE, -1);
			} else {
				for (size_t i = 0; i < _subComps.size(); ++i) {
					if (utils::dynamic_pointer_cast<MoverComponent>(_subComps[i])->_positioning == Positioning::POSITIONING_ABSOLUTE) {
						return std::make_pair(AddStrategy::REPLACE, i);
					}
				}
				return std::make_pair(AddStrategy::ADD, 0);
			}
		}
	}

	void MoverComponent::shatter(const GOPtr & go) {
		if (!_linkable) {
			return;
		}
		bool found = false;
		for (size_t i = 0; i < _linked.size(); i++) {
			if (_linked[i] == go->getID()) {
				_linked.erase(_linked.begin() + int(i));
				found = true;
				break;
			}
		}
		if (!found) {
			_linked.push_back(go->getID());
		}
	}

	std::vector<componentOptions> MoverComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Duration", [this]() {
			return std::to_string(_duration);
		}, [this](std::string s) {
			_duration = std::stoull(s);
			return true;
		}, "Integer"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Positioning", [this]() {
			return std::to_string(uint16_t(_positioning));
		}, [this](std::string s) {
			_positioning = Positioning(std::stoi(s));
			return true;
		}, "MoverPositioning"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "OnStart On", [this]() {
			return std::to_string(_started);
		}, [this](std::string s) {
			_started = std::stoi(s) == 1;
			return true;
		}, "Bool"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Linkable", [this]() {
			return std::to_string(_linkable);
		}, [this](std::string s) {
			_linkable = std::stoi(s) == 1;
			return true;
		}, "Bool"));

		return result;
	}

} /* namespace api */
} /* namespace i6e */
