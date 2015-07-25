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

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	LuminousAppearanceComponent::LuminousAppearanceComponent(const int64_t id, const attributeMap & params) : Component(id, params), _lightType(LightType(std::stoul(params.find("lightType")->second))), _diffuseColor(Vec3(params, "diffuseColor")), _specularColor(Vec3(params, "specularColor")), _attenuation(Vec4(params, "attenuation")), _direction(), _position(), _spotlightRangeInner(0.0), _spotlightRangeOuter(0.0) {
		Component::_objFamilyID = components::LuminousAppearanceComponent;
		Component::_objComponentID = components::LuminousAppearanceComponent;

		if (_lightType != LightType::POINT) {
			_direction = Vec3(params, "direction");
		}
		if (_lightType == LightType::SPOT) {
			_spotlightRangeInner = std::stod(params.find("spotLightRangeInner")->second);
			_spotlightRangeOuter = std::stod(params.find("spotLightRangeOuter")->second);
		}

		if (params.find("pos") != params.end()) {
			_position = Vec3(params, "pos");
		}
	}

	LuminousAppearanceComponent::~LuminousAppearanceComponent() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLuminous, core::Method::Delete, new graphics::Graphics_Luminous_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	ComPtr LuminousAppearanceComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("LuminousAppearanceComponent", "diffuseColor not set!", params.find("diffuseColor") != params.end());
		ISIXE_THROW_API_COND("LuminousAppearanceComponent", "attenuation not set!", params.find("attenuation") != params.end());
		ISIXE_THROW_API_COND("LuminousAppearanceComponent", "specularColor not set!", params.find("specularColor") != params.end());
		ISIXE_THROW_API_COND("LuminousAppearanceComponent", "lightType not set!", params.find("lightType") != params.end());

		if (params.find("lightType")->second != "0") {
			ISIXE_THROW_API_COND("LuminousAppearanceComponent", "direction not set!", params.find("direction") != params.end());
		}
		if (params.find("lightType")->second == "2") {
			ISIXE_THROW_API_COND("LuminousAppearanceComponent", "spotLightRangeInner not set!", params.find("spotLightRangeInner") != params.end());
			ISIXE_THROW_API_COND("LuminousAppearanceComponent", "spotLightRangeOuter not set!", params.find("spotLightRangeOuter") != params.end());
		}
		return utils::make_shared<LuminousAppearanceComponent, Component>(id, params);
	}

	void LuminousAppearanceComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLuminous, core::Method::Create, new graphics::Graphics_Luminous_Update(_objOwnerID, getID(), int(_lightType), _diffuseColor, _specularColor, _attenuation, _direction, _spotlightRangeInner, _spotlightRangeOuter, _position), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void LuminousAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraLuminous, core::Method::Update, new graphics::Graphics_Luminous_Update(_objOwnerID, getID(), int(_lightType), _diffuseColor, _specularColor, _attenuation, _direction, _spotlightRangeInner, _spotlightRangeOuter, _position), core::Subsystem::Object);

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

	attributeMap LuminousAppearanceComponent::synchronize() const {
		attributeMap params;
		_diffuseColor.insertInMap("diffuseColor", params);
		_attenuation.insertInMap("attenuation", params);
		_specularColor.insertInMap("specularColor", params);
		_direction.insertInMap("direction", params);
		params.insert(std::make_pair("lightType", boost::lexical_cast<std::string>(uint32_t(_lightType))));
		params.insert(std::make_pair("spotLightRangeInner", boost::lexical_cast<std::string>(_spotlightRangeInner)));
		params.insert(std::make_pair("spotLightRangeOuter", boost::lexical_cast<std::string>(_spotlightRangeOuter)));

		if (_position.isValid()) {
			_position.insertInMap("pos", params);
		}
		return params;
	}

	std::pair<AddStrategy, int64_t> LuminousAppearanceComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> LuminousAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "LightType", [this]() {
			return boost::lexical_cast<std::string>(int(_lightType));
		}, [this](std::string s) {
			setLightType(LightType(boost::lexical_cast<int>(s)));
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Inner Spotlight Range", [this]() {
			return boost::lexical_cast<std::string>(_spotlightRangeInner);
		}, [this](std::string s) {
			_spotlightRangeInner = std::stod(s);
			sendUpdateMessage();
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Outer Spotlight Range", [this]() {
			return boost::lexical_cast<std::string>(_spotlightRangeOuter);
		}, [this](std::string s) {
			_spotlightRangeOuter = std::stod(s);
			sendUpdateMessage();
			return true;
		}));
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
		}));
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
		}));
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
		}));
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
		}));
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
		}));

		return result;
	}

} /* namespace api */
} /* namespace i6engine */
