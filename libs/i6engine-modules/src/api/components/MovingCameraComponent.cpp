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

#include "i6engine/api/components/MovingCameraComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	MovingCameraComponent::MovingCameraComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _lookAt(), _nearClip(1), _aspect(), _viewport(false), _left(), _top(), _width(), _height(), _red(0), _green(0), _blue(0), _alpha(1.0f), _fov(3.14 / 6) {
		Component::_objFamilyID = components::CameraComponent;
		Component::_objComponentID = components::MovingCameraComponent;

		_position = Vec3(params, "pos");
		_lookAt = Vec3(params, "lookAt");
		_nearClip = boost::lexical_cast<int32_t>(params.find("nearclip")->second);
		_aspect = boost::lexical_cast<double>(params.find("aspect")->second);

		if (params.find("viewport") != params.end()) {
			_viewport = boost::lexical_cast<bool>(params.find("viewport")->second);
			_left = boost::lexical_cast<double>(params.find("vp_left")->second);
			_top = boost::lexical_cast<double>(params.find("vp_top")->second);
			_width = boost::lexical_cast<double>(params.find("vp_width")->second);
			_height = boost::lexical_cast<double>(params.find("vp_height")->second);
			_red = boost::lexical_cast<double>(params.find("vp_red")->second);
			_green = boost::lexical_cast<double>(params.find("vp_green")->second);
			_blue = boost::lexical_cast<double>(params.find("vp_blue")->second);
			_alpha = boost::lexical_cast<double>(params.find("vp_alpha")->second);
		}
	}

	MovingCameraComponent::~MovingCameraComponent() {
		removeTicker();
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Delete, new graphics::Graphics_Camera_Delete(_objOwnerID), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	ComPtr MovingCameraComponent::createC(const int64_t id, const attributeMap & params) {
		return utils::make_shared<MovingCameraComponent, Component>(id, params);
	}

	void MovingCameraComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Create, new graphics::Graphics_Camera_Create(_objOwnerID, getID(), _position, _lookAt, _nearClip, _fov), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		if (_viewport) {
			sendViewportUpdateMessage();
		}

		addTicker();
	}

	void MovingCameraComponent::setPosition(const Vec3 & pos) {
		_position = pos;

		sendCameraUpdateMessage();
	}

	void MovingCameraComponent::setLookAt(const Vec3 & lookAt) {
		_lookAt = lookAt;

		sendCameraUpdateMessage();
	}

	void MovingCameraComponent::setNearClip(const int32_t n) {
		_nearClip = n;

		sendCameraUpdateMessage();
	}

	void MovingCameraComponent::setAspectRatio(const double ratio) {
		_aspect = ratio;

		sendCameraUpdateMessage();
	}

	void MovingCameraComponent::setFOVy(double fov) {
		_fov = fov;

		sendCameraUpdateMessage();
	}

	void MovingCameraComponent::setFrustumExtends(double left, double right, double top, double bottom) {
		_frustumLeft = left;
		_frustumRight = right;
		_frustumTop = top;
		_frustumBottom = bottom;

		sendFrustumUpdateMessage();
	}

	void MovingCameraComponent::setViewportDimension(const double left, const double top, const double width, const double height) {
		_viewport = true;

		_left = left;
		_top = top;
		_width = width;
		_height = height;

		sendViewportUpdateMessage();
	}

	void MovingCameraComponent::setViewportBackground(const double red, const double green, const double blue, const double alpha) {
		_viewport = true;

		_red = red;
		_green = green;
		_blue = blue;
		_alpha = alpha;

		sendViewportUpdateMessage();
	}

	void MovingCameraComponent::sendCameraUpdateMessage() {
		if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Update, new graphics::Graphics_Camera_Update(_objOwnerID, _id, _position, _lookAt, _nearClip, _fov), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MovingCameraComponent::sendViewportUpdateMessage() {
		if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraViewport, core::Method::Update, new graphics::Graphics_Viewport_Update(_objOwnerID, getID(), _left, _top, _width, _height, _red, _green, _blue, _alpha), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MovingCameraComponent::sendFrustumUpdateMessage() {
		if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraFrustum, core::Method::Update, new graphics::Graphics_CameraFrustum_Update(_objOwnerID, getID(), _frustumLeft, _frustumRight, _frustumTop, _frustumBottom), i6engine::core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MovingCameraComponent::Tick() {
		GOPtr go = getOwnerGO();
		if (go->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		utils::sharedPtr<PhysicalStateComponent, Component> psc = go->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);

		// newPos := Bikeposition and LookAt Point
		Vec3 targetPos = psc->getPosition();
		Vec3 camPosTarget = targetPos + math::rotateVector(_position, psc->getRotation());
		Vec3 camPosCur = _oldPos;
		// movePos := position where camera will be placed
		// _oldPOs := old camera position
		Vec3 movePos = camPosCur + (camPosTarget - camPosCur) * 0.2;
		Vec3 newPos = movePos - targetPos;
		Vec3 newPosRotated = math::rotateVector(newPos, psc->getRotation().inverse());
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Update, new graphics::Graphics_Camera_Update(_objOwnerID, _id, newPosRotated, targetPos, _nearClip, _fov), i6engine::core::Subsystem::Object);
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		_oldPos = movePos;
	}

	attributeMap MovingCameraComponent::synchronize() const {
		return attributeMap();
	}

	std::pair<AddStrategy, int64_t> MovingCameraComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> MovingCameraComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Position", [this]() {
			return _position.toString();
		}, [this](std::string s) {
			try {
				_position = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setPosition(_position);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Look At", [this]() {
			return _lookAt.toString();
		}, [this](std::string s) {
			try {
				_lookAt = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setLookAt(_lookAt);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Near Clip", [this]() {
			return boost::lexical_cast<std::string>(_nearClip);
		}, [this](std::string s) {
			try {
				_nearClip = boost::lexical_cast<int32_t>(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setNearClip(_nearClip);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Aspect Ratio", [this]() {
			return boost::lexical_cast<std::string>(_aspect);
		}, [this](std::string s) {
			try {
				_aspect = boost::lexical_cast<double>(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setAspectRatio(_aspect);
			return true;
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
