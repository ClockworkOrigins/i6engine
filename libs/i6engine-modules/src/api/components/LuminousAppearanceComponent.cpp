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

#include "i6engine/api/components/LuminousAppearanceComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	LuminousAppearanceComponent::LuminousAppearanceComponent(const int64_t id, const attributeMap & params) : Component(id, params), _lightType(), _diffuseColor(), _specularColor(), _attenuation(), _direction(), _position(), _spotlightRangeInner(0.0), _spotlightRangeOuter(0.0) {
		Component::_objFamilyID = components::LuminousAppearanceComponent;
		Component::_objComponentID = components::LuminousAppearanceComponent;

		parseAttribute<true>(params, "lightType", _lightType);
		parseAttribute<true>(params, "diffuseColor", _diffuseColor);
		parseAttribute<true>(params, "specularColor", _specularColor);
		parseAttribute<true>(params, "attenuation", _attenuation);

		if (_lightType != LightType::POINT) {
			parseAttribute<true>(params, "direction", _direction);
		}
		if (_lightType == LightType::SPOT) {
			parseAttribute<true>(params, "spotLightRangeInner", _spotlightRangeInner);
			parseAttribute<true>(params, "spotLightRangeOuter", _spotlightRangeOuter);
		}

		parseAttribute<false>(params, "pos", _position);
	}

	LuminousAppearanceComponent::~LuminousAppearanceComponent() {
	}

	void LuminousAppearanceComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLuminous, core::Method::Create, new graphics::Graphics_Luminous_Update(_objOwnerID, getID(), uint16_t(_lightType), _diffuseColor, _specularColor, _attenuation, _direction, _spotlightRangeInner, _spotlightRangeOuter, _position), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void LuminousAppearanceComponent::Finalize() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLuminous, core::Method::Delete, new graphics::Graphics_Luminous_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void LuminousAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLuminous, core::Method::Update, new graphics::Graphics_Luminous_Update(_objOwnerID, getID(), uint16_t(_lightType), _diffuseColor, _specularColor, _attenuation, _direction, _spotlightRangeInner, _spotlightRangeOuter, _position), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void LuminousAppearanceComponent::setLightType(const LightType lightType) {
		_lightType = lightType;

		sendUpdateMessage();
	}

	void LuminousAppearanceComponent::setDiffuseColor(const Vec3 & diffuseColor) {
		_diffuseColor = diffuseColor;

		sendUpdateMessage();
	}

	void LuminousAppearanceComponent::setSpecularColor(const Vec3 & specular) {
		_specularColor = specular;

		sendUpdateMessage();
	}

	void LuminousAppearanceComponent::setAttenuation(const Vec4 & attenuation) {
		_attenuation = attenuation;

		sendUpdateMessage();
	}

	void LuminousAppearanceComponent::setDirection(const Vec3 & direction) {
		_direction = direction;

		sendUpdateMessage();
	}

	void LuminousAppearanceComponent::setSpotLightInnerRange(double angle) {
		_spotlightRangeInner = angle;

		sendUpdateMessage();
	}

	void LuminousAppearanceComponent::setSpotLightOuterRange(double angle) {
		_spotlightRangeOuter = angle;

		sendUpdateMessage();
	}

	attributeMap LuminousAppearanceComponent::synchronize() const {
		attributeMap params;
		writeAttribute(params, "diffuseColor", _diffuseColor);
		writeAttribute(params, "attenuation", _attenuation);
		writeAttribute(params, "specularColor", _specularColor);
		writeAttribute(params, "lightType", _lightType);

		if (_lightType != LightType::POINT) {
			writeAttribute(params, "direction", _direction);
		}
		if (_lightType == LightType::SPOT) {
			writeAttribute(params, "spotLightRangeInner", _spotlightRangeInner);
			writeAttribute(params, "spotLightRangeOuter", _spotlightRangeOuter);
		}

		if (_position.isValid()) {
			writeAttribute(params, "pos", _position);
		}
		return params;
	}

	std::pair<AddStrategy, int64_t> LuminousAppearanceComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> LuminousAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "LightType", [this]() {
			return boost::lexical_cast<std::string>(int(_lightType));
		}, [this](std::string s) {
			setLightType(LightType(boost::lexical_cast<int>(s)));
			return true;
		}, "LightType"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Inner Spotlight Range", [this]() {
			return boost::lexical_cast<std::string>(_spotlightRangeInner);
		}, [this](std::string s) {
			_spotlightRangeInner = std::stod(s);
			sendUpdateMessage();
			return true;
		}, "Angle"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Outer Spotlight Range", [this]() {
			return boost::lexical_cast<std::string>(_spotlightRangeOuter);
		}, [this](std::string s) {
			_spotlightRangeOuter = std::stod(s);
			sendUpdateMessage();
			return true;
		}, "Angle"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Position", [this]() {
			return _position.toString();
		}, [this](std::string s) {
			try {
				_position = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			sendUpdateMessage();
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Diffuse Color", [this]() {
			return _diffuseColor.toString();
		}, [this](std::string s) {
			try {
				_diffuseColor = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setDiffuseColor(_diffuseColor);
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Specular Color", [this]() {
			return _specularColor.toString();
		}, [this](std::string s) {
			try {
				_specularColor = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setSpecularColor(_specularColor);
			return true;
		}, "Vec3"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Attenuation", [this]() {
			return _attenuation.toString();
		}, [this](std::string s) {
			try {
				_attenuation = Vec4(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setAttenuation(_attenuation);
			return true;
		}, "Vec4"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Direction", [this]() {
			return _direction.toString();
		}, [this](std::string s) {
			try {
				_direction = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setDirection(_direction);
			return true;
		}, "Vec3"));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
