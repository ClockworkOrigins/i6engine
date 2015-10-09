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

#include "i6engine/api/components/MoverInterpolateComponent.h"

#include "i6engine/utils/Exceptions.h"

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

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	MoverInterpolateComponent::MoverInterpolateComponent(const int64_t id, const attributeMap & params) : MoverComponent(id, params), _keyFrames(), _mode(), _openTime(2), _way(), _totalDistance(0), _currentDist(0), _currentFrame(0), _direction(true), _lock() {
		_objComponentID = components::MoverInterpolateComponent;

		loadParams(params);
	}

	MoverInterpolateComponent::~MoverInterpolateComponent() {
	}

	void MoverInterpolateComponent::addKeyFrame(const Vec3 & position, const Quaternion & rotation) {
		boost::mutex::scoped_lock l(_lock);
		_keyFrames.push_back(keyFrame(position, rotation));
	}

	void MoverInterpolateComponent::removeKeyFrame(const uint32_t id) {
		boost::mutex::scoped_lock l(_lock);
		_keyFrames.erase(_keyFrames.begin() + int(id));
	}

	void MoverInterpolateComponent::start(Vec3 & startPos) {
		addTicker();

		boost::mutex::scoped_lock l(_lock);

		_started = true;

		_moving = true;
		_currentFrame = 0;
		_totalDistance = 0;
		if (_positioning == Positioning::POSITIONING_RELATIVE) {
			// for absolute, startPos will be ignored because it doesn't make any sense
			_realStartPos = startPos;
		}

		_startTime = EngineController::GetSingleton().getCurrentTime();

		if (_keyFrames.size() <= 1) {
			ISIXE_THROW_FAILURE("MoverComponent", "You need at least two keyFrames");
			return;
		}

		if (_keyFrames.size() > 0) {
			if (_direction) {
				for (size_t i = 0; i < _keyFrames.size() - 1; ++i) {
					_totalDistance += (_keyFrames[i + 1].first - _keyFrames[i].first).length();
				}
			} else {
				for (size_t i = _keyFrames.size(); i > 0; --i) {
					_totalDistance += (_keyFrames[i].first - _keyFrames[i - 1].first).length();
				}
			}
		}
		auto psc = _psc.get();
		if (_way == Way::LINEAR) {
			psc->setPosition(_realStartPos, 2);
			if (_direction) {
				_lastPos = _keyFrames[0].first;
			} else {
				_lastPos = _keyFrames.back().first;
			}
		} else if (_way == Way::BEZIER) {
			psc->setPosition(_realStartPos, 2);
			if (_direction) {
				_lastPos = _keyFrames[0].first;
			} else {
				_lastPos = _keyFrames.back().first;
			}
		} else {
			ISIXE_THROW_FAILURE("MoverComponent", "Unknown way");
		}

		// resync
		GOPtr go = getOwnerGO();
		if (go != nullptr && go->getGOC(components::NetworkSenderComponent) != nullptr) {
			attributeMap am = synchronize();
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ComponentMessageType, components::ComMoverResync, core::Method::Update, new components::Component_MoverResync_Update(go->getID(), _id, am), core::Subsystem::Object);
			EngineController::GetSingletonPtr()->getNetworkFacade()->publish(OBJECT_CHANNEL, msg);
		}
	}

	void MoverInterpolateComponent::getNewPosition(const uint64_t t, Vec3 & newPos, Quaternion & newRot) {
		uint64_t timeElapsed = t;

		boost::mutex::scoped_lock l(_lock);

		double tt = 0;
		if (_mode == Mode::NSTATE_LOOP) {
			timeElapsed %= _duration;
			tt = double(timeElapsed) / _duration;
		} else if (_mode == Mode::TWOSTATE_TOGGLE) {
			timeElapsed %= (2 * _duration);
			tt = 1 - double(timeElapsed - _duration) / _duration;
		} else if (_mode == Mode::TWOSTATE_OPENTIME) {
			timeElapsed %= (2 * _duration + _openTime);
			if (timeElapsed < _duration) {
				tt = double(timeElapsed) / _duration;
			} else if (timeElapsed < _duration + _openTime) {
				tt = 1;
			} else {
				tt = 1 - double(timeElapsed - _duration - _openTime) / _duration;
			}
		} else if (_mode == Mode::ONCE) {
			timeElapsed %= _duration;
			tt = double(timeElapsed) / _duration;

			if (t > _duration) {
				stop();
				return;
			}
		}
		switch (_way) {
		case Way::LINEAR: {
			// TODO (Michael): optimierung, dass nicht immer soviel gerechnet werden muss. vllt etwas precalc? oder distanzvektoren zusaetlzlich speichern?
			// gesamtstrecke nach dieser Zeit
			double traveled = _totalDistance * tt;
			// zwischen diesen 2 Frames ist normalerweise der Punkt
			keyFrame last;
			keyFrame next;

			if (_direction) {
				last = _keyFrames[_currentFrame];
				next = _keyFrames[(_currentFrame + 1) % _keyFrames.size()];
			} else {
				last = _keyFrames[_currentFrame];

				if (_currentFrame == 0) {
					next = _keyFrames.back();
				} else {
					next = _keyFrames[(_currentFrame - 1)];
				}
			}

			// this part of the distance between these two frames should be traveled
			double part = (traveled - _currentDist) / (next.first - last.first).length();
			if (_currentDist > traveled) {
				_currentFrame = 0;
				_currentDist = 0;
				part = (traveled - _currentDist) / (next.first - last.first).length();
				if (_direction) {
					last = _keyFrames[0];
					next = _keyFrames[1];
				} else {
					last = _keyFrames.back();
					next = _keyFrames[_keyFrames.size() - 2];
				}
			}
			// point is further than next frame
			while (part >= 1) {
				// go to next frames
				if (_direction) {
					_currentFrame++;
					_currentFrame %= _keyFrames.size();
					_currentDist += (next.first - last.first).length();
					last = next;
					next = _keyFrames[(_currentFrame + 1) % _keyFrames.size()];
				} else {
					_currentFrame--;

					if (_currentFrame == UINT32_MAX) {
						_currentFrame = _keyFrames.size() - 1;
					}
					_currentDist += (next.first - last.first).length();
					last = next;

					if (_currentFrame == 0) {
						next = _keyFrames.back();
					} else {
						next = _keyFrames[(_currentFrame - 1)];
					}
				}
				part = (traveled - _currentDist) / (next.first - last.first).length();
			}

			// calc actual Position
			newPos = last.first + (next.first - last.first) * part;
			// calc actual rotation
			double omega = acos(dotProduct(last.second, next.second));
			newRot = (last.second * sin((1 - part) * omega) + next.second * sin(part * omega)) / sin(omega);
			break;
		}
		case Way::BEZIER: {
			double fak1 = 1, fak2 = 1;
			uint32_t n = _keyFrames.size();
			for (uint32_t i = 0; i < n; i++) {
				fak1 *= (1 - tt);
			}
			for (uint32_t i = 0; i < n + 1; ++i) {
				newPos += _keyFrames[i % n].first * fak1 * fak2 * double(math::binom(n, i));
				fak1 /= (1 - tt);
				fak2 *= tt;
			}
			break;
		}
		default: {
			ISIXE_THROW_FAILURE("MoverLinearComponent", "Invalid way");
			return;
		}
		} /* switch _way */
	}

	void MoverInterpolateComponent::News(const GameMessage::Ptr & msg) {
		uint16_t type = msg->getSubtype();
		if (type == api::components::ComMoverResync) {
			attributeMap am = static_cast<components::Component_MoverResync_Update *>(msg->getContent())->attMap;
			stop();

			{
				boost::mutex::scoped_lock l(_lock);
				_keyFrames.clear();
			}

			MoverComponent::loadParams(am);
			loadParams(am);
			Vec3 x(am, "realCenterPos");
			start(x);
		} else {
			ISIXE_THROW_FAILURE("MoverInterpolateComponent", "Unknown message type");
		}
	}

	void MoverInterpolateComponent::loadParams(const attributeMap & params) {
		parseAttribute<true>(params, "mode", _mode);
		parseAttribute<true>(params, "way", _way);
		parseAttribute<true>(params, "direction", _direction);

		if (_mode == Mode::TWOSTATE_OPENTIME) {
			parseAttribute<true>(params, "openTime", _openTime);
		}

		uint32_t frames;
		parseAttribute<true>(params, "keyframes", frames);
		for (uint32_t i = 0; i < frames; ++i) {
			ISIXE_THROW_API_COND("MoverComponent", "required parameter '" << std::string("keyframe_") + boost::lexical_cast<std::string>(i) + "_pos" << "' not set", params.find(std::string("keyframe_") + boost::lexical_cast<std::string>(i) + "_pos") != params.end());
			ISIXE_THROW_API_COND("MoverComponent", "required parameter '" << std::string("keyframe_") + boost::lexical_cast<std::string>(i) +"_rot" << "' not set", params.find(std::string("keyframe_") + boost::lexical_cast<std::string>(i) +"_rot") != params.end());
			Vec3 pos;
			Quaternion rot;
			parseAttribute<true>(params, std::string("keyframe_") + std::to_string(i) + "_pos", pos);
			parseAttribute<true>(params, std::string("keyframe_") + std::to_string(i) + "_rot", rot);
			addKeyFrame(pos, rot);
		}
		if (_direction) {
			_lastPos = _keyFrames[0].first;
		} else {
			_lastPos = _keyFrames.back().first;
		}
	}

	attributeMap MoverInterpolateComponent::synchronize() const {
		attributeMap params = MoverComponent::synchronize();

		// general attributes
		params["mode"] = boost::lexical_cast<std::string>(uint16_t(_mode));
		params["way"] = boost::lexical_cast<std::string>(uint16_t(_way));
		params["direction"] = std::to_string(_direction);

		params["keyframes"] = boost::lexical_cast<std::string>(_keyFrames.size());
		for (size_t i = 0; i < _keyFrames.size(); ++i) {
			_keyFrames[i].first.insertInMap(std::string("keyframe_") + boost::lexical_cast<std::string>(i) + "_pos", params);
			_keyFrames[i].second.insertInMap(std::string("keyframe_") + boost::lexical_cast<std::string>(i) + "_rot", params);
		}

		return params;
	}

	void MoverInterpolateComponent::reset() {
		boost::mutex::scoped_lock l(_lock);

		_currentFrame = 0;
		_totalDistance = 0;

		if (_keyFrames.size() <= 1) {
			ISIXE_THROW_FAILURE("MoverComponent", "You need at least two keyFrames");
			return;
		}
		auto psc = _psc.get();
		if (_way == Way::LINEAR) {
			psc->setPosition(_realStartPos, 1);
			_lastPos = _keyFrames[0].first;
		} else if (_way == Way::BEZIER) {
			psc->setPosition(_realStartPos, 1);
			_lastPos = _keyFrames[0].first;
		} else {
			ISIXE_THROW_FAILURE("MoverComponent", "Unknown way");
		}
	}

	std::vector<componentOptions> MoverInterpolateComponent::getComponentOptions() {
		std::vector<componentOptions> result = MoverComponent::getComponentOptions();

		result.push_back(std::make_tuple(AccessState::READWRITE, "Mode", [this]() {
			return boost::lexical_cast<std::string>(uint16_t(_mode));
		}, [this](std::string s) {
			_mode = Mode(boost::lexical_cast<uint16_t>(s));
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Way", [this]() {
			return boost::lexical_cast<std::string>(uint16_t(_way));
		}, [this](std::string s) {
			_way = Way(boost::lexical_cast<uint16_t>(s));
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READONLY, "Num. Keyframes", [this]() {
			return boost::lexical_cast<std::string>(_keyFrames.size());
		}, boost::function<bool(std::string)>()));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
