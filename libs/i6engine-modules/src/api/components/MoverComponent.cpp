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

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	MoverComponent::MoverComponent(const int64_t id, const attributeMap & params) : ShatterComponent(id, params), _duration(0), _positioning(), _psc(), _startTime(), _lastPos(), _realStartPos(), _lastRot(), _moving(false), _initial(true), _started(false), _linkable(false), _linked() {
		_objFamilyID = components::MoverComponent;

		loadParams(params);
	}

	MoverComponent::~MoverComponent() {
		removeTicker();
	}

	void MoverComponent::Init() {
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
			auto a = _startTime;
			start(_realStartPos);
			_startTime = a;
		}
	}

	void MoverComponent::stop() {
		_moving = false;
		removeTicker();

		_started = false;
	}

	void MoverComponent::Tick() {
		// Zeit vergangen seit beginn der Bewegung
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
		if (params.find("duration") == params.end()) {
			ISIXE_THROW_API("MoverComponent", "required parameter 'duration' not set");
		}
		_duration = boost::lexical_cast<uint64_t>(params.at("duration"));

		if (params.find("positioning") == params.end()) {
			ISIXE_THROW_API("MoverComponent", "required parameter 'positioning' not set");
		}
		_positioning = Positioning(boost::lexical_cast<uint16_t>(params.at("positioning")));

		// movement continues (after sync)
		if (params.find("continue") != params.end() && params.at("continue") == "true") {
			_initial = false;
			if (params.find("startTime") == params.end()) {
				ISIXE_THROW_API("MoverComponent", "optional required parameter 'startTime' not set, but 'continue' set to true");
			}
			_startTime = boost::lexical_cast<uint64_t>(params.at("startTime"));
		}

		// override lastpos if set
		if (params.find("lastPos") != params.end()) {
			_lastPos = Vec3(params, "lastPos");
		}
		if (_positioning == Positioning::POSITIONING_ABSOLUTE) {
			_realStartPos = _lastPos;
		}

		if (params.find("started") == params.end()) {
			ISIXE_THROW_API("MoverComponent", "required paramter 'started' to set");
		}
		_started = boost::lexical_cast<bool>(params.find("started")->second);

		if (params.find("linkable") != params.end()) {
			_linkable = boost::lexical_cast<bool>(params.find("linkable")->second);
		}
	}

	attributeMap MoverComponent::synchronize() {
		attributeMap params;

		// other Component only needs to resume
		params["continue"] = "true";
		params["startTime"] = boost::lexical_cast<std::string>(_startTime);

		// general attributes
		params["duration"] = boost::lexical_cast<std::string>(_duration);
		params["positioning"] = boost::lexical_cast<std::string>(uint16_t(_positioning));
		params["started"] = boost::lexical_cast<std::string>(_started);
		params["linkable"] = boost::lexical_cast<std::string>(_linkable);

		_realStartPos.insertInMap("realCenterPos", params);

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
			return boost::lexical_cast<std::string>(_duration);
		}, [this](std::string s) {
			_duration = boost::lexical_cast<uint64_t>(s);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Positioning", [this]() {
			return boost::lexical_cast<std::string>(uint16_t(_positioning));
		}, [this](std::string s) {
			_positioning = Positioning(boost::lexical_cast<int>(s));
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "OnStart On", [this]() {
			return boost::lexical_cast<std::string>(_started);
		}, [this](std::string s) {
			_started = boost::lexical_cast<bool>(s);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Linkable", [this]() {
			return boost::lexical_cast<std::string>(_linkable);
		}, [this](std::string s) {
			_linkable = boost::lexical_cast<bool>(s);
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
