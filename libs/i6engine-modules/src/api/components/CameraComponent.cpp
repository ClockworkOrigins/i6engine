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

#include "i6engine/api/components/CameraComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	CameraComponent::CameraComponent(const int64_t id, const attributeMap & params) : Component(id, params), _position(), _lookAt(), _nearClip(1), _aspect(), _zOrder(), _viewport(false), _left(), _top(), _width(), _height(), _red(0), _green(0), _blue(0), _alpha(1.0), _fov(45.0 / 180.0 * PI) {
		Component::_objFamilyID = components::CameraComponent;
		Component::_objComponentID = components::CameraComponent;

		parseAttribute<true>(params, "pos", _position);
		parseAttribute<true>(params, "lookAt", _lookAt);
		parseAttribute<true>(params, "nearclip", _nearClip);
		parseAttribute<true>(params, "aspect", _aspect);

		if (params.find("viewport") != params.end()) {
			parseAttribute<true>(params, "zOrder", _zOrder);
			parseAttribute<true>(params, "viewport", _viewport);
			parseAttribute<true>(params, "vp_left", _left);
			parseAttribute<true>(params, "vp_top", _top);
			parseAttribute<true>(params, "vp_width", _width);
			parseAttribute<true>(params, "vp_height", _height);
			parseAttribute<true>(params, "vp_red", _red);
			parseAttribute<true>(params, "vp_green", _green);
			parseAttribute<true>(params, "vp_blue", _blue);
			parseAttribute<true>(params, "vp_alpha", _alpha);
		}
	}

	CameraComponent::~CameraComponent() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Delete, new graphics::Graphics_Camera_Delete(_objOwnerID), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void CameraComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Create, new graphics::Graphics_Camera_Create(_objOwnerID, getID(), _position, _lookAt, _nearClip, _fov), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		if (_viewport) {
			sendViewportUpdateMessage();
		}
	}

	void CameraComponent::setPosition(const Vec3 & relativePosition) {
		_position = relativePosition;

		sendCameraUpdateMessage();
	}

	void CameraComponent::setLookAt(const Vec3 & lookAt) {
		_lookAt = lookAt;

		sendCameraUpdateMessage();
	}

	void CameraComponent::setNearClip(const int32_t n) {
		_nearClip = n;

		sendCameraUpdateMessage();
	}

	void CameraComponent::setAspectRatio(const double ratio) {
		_aspect = ratio;

		sendCameraUpdateMessage();
	}

	void CameraComponent::setFOVy(const double fov) {
		_fov = fov;

		sendCameraUpdateMessage();
	}

	void CameraComponent::setFrustumExtends(double left, double right, double top, double bottom) {
		_frustumLeft = left;
		_frustumRight = right;
		_frustumTop = top;
		_frustumBottom = bottom;

		sendFrustumUpdateMessage();
	}

	void CameraComponent::setViewportDimension(const double left, const double top, const double width, const double height) {
		_viewport = true;

		_left = left;
		_top = top;
		_width = width;
		_height = height;

		sendViewportUpdateMessage();
	}

	void CameraComponent::setViewportBackground(const double red, const double green, const double blue, const double alpha) {
		_viewport = true;

		_red = red;
		_green = green;
		_blue = blue;
		_alpha = alpha;

		sendViewportUpdateMessage();
	}

	void CameraComponent::sendCameraUpdateMessage() {
		if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Update, new graphics::Graphics_Camera_Update(_objOwnerID, _id, _position, _lookAt, _nearClip, _fov), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void CameraComponent::sendViewportUpdateMessage() {
		if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraViewport, core::Method::Update, new graphics::Graphics_Viewport_Update(_objOwnerID, getID(), _zOrder, _left, _top, _width, _height, _red, _green, _blue, _alpha), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void CameraComponent::sendFrustumUpdateMessage() {
		if (getOwnerGO()->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraFrustum, core::Method::Update, new graphics::Graphics_CameraFrustum_Update(_objOwnerID, getID(), _frustumLeft, _frustumRight, _frustumTop, _frustumBottom), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap CameraComponent::synchronize() const {
		attributeMap params;
		_position.insertInMap("pos", params);
		_lookAt.insertInMap("lookAt", params);
		params.insert(std::make_pair("nearclip", std::to_string(_nearClip)));
		params.insert(std::make_pair("aspect", std::to_string(_aspect)));
		params.insert(std::make_pair("viewport", std::to_string(_viewport)));
		params.insert(std::make_pair("zOrder", std::to_string(_zOrder)));
		params.insert(std::make_pair("vp_left", std::to_string(_left)));
		params.insert(std::make_pair("vp_top", std::to_string(_top)));
		params.insert(std::make_pair("vp_width", std::to_string(_width)));
		params.insert(std::make_pair("vp_height", std::to_string(_height)));
		params.insert(std::make_pair("vp_red", std::to_string(_red)));
		params.insert(std::make_pair("vp_green", std::to_string(_green)));
		params.insert(std::make_pair("vp_blue", std::to_string(_blue)));
		params.insert(std::make_pair("vp_alpha", std::to_string(_alpha)));
		return params;
	}

	std::pair<AddStrategy, int64_t> CameraComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> CameraComponent::getComponentOptions() {
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
		}, "Vec3"));
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
		}, "Vec3"));
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
		}, "Integer"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Aspect Ratio", [this]() {
			return boost::lexical_cast<std::string>(_aspect);
		}, [this](std::string s) {
			try {
				_aspect = std::stod(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setAspectRatio(_aspect);
			return true;
		}, "Double"));

		return result;
	}

	void CameraComponent::enableCompositor(const std::string & compositor, bool enabled) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCompositor, core::Method::Update, new graphics::Graphics_Compositor_Update(_objOwnerID, getID(), compositor, enabled), core::Subsystem::Object));
	}

} /* namespace api */
} /* namespace i6engine */
