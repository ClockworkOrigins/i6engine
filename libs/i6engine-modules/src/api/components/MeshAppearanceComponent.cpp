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

#include "i6engine/api/components/MeshAppearanceComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
namespace api {

	MeshAppearanceComponent::MeshAppearanceComponent(const int64_t id, const attributeMap & params) : Component(id, params), _meshName(params.find("mesh")->second), _isVisible(boost::lexical_cast<bool>(params.find("visibility")->second)), _position(Vec3(params, "pos")), _rotation(Quaternion(params, "rot")), _scale(Vec3(params, "scale")), _material() {
		Component::_objFamilyID = components::MeshAppearanceComponent;
		Component::_objComponentID = components::MeshAppearanceComponent;

		if (params.find("material") != params.end()) {
			_material = params.find("material")->second;
		}
	}

	MeshAppearanceComponent::~MeshAppearanceComponent() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMesh, core::Method::Delete, new graphics::Graphics_Mesh_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	ComPtr MeshAppearanceComponent::createC(const int64_t id, const attributeMap & params) {
		ISIXE_THROW_API_COND("MeshAppearanceComponent", "position not set!", params.find("pos") != params.end());
		ISIXE_THROW_API_COND("MeshAppearanceComponent", "rotation not set!", params.find("rot") != params.end());
		ISIXE_THROW_API_COND("MeshAppearanceComponent", "scale not set!", params.find("scale") != params.end());
		ISIXE_THROW_API_COND("MeshAppearanceComponent", "mesh not set!", params.find("mesh") != params.end());
		ISIXE_THROW_API_COND("MeshAppearanceComponent", "visibility not set!", params.find("visibility") != params.end());
		return utils::make_shared<MeshAppearanceComponent, Component>(id, params);
	}

	void MeshAppearanceComponent::Init() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMesh, core::Method::Create, new graphics::Graphics_Mesh_Create(_objOwnerID, getID(), _meshName, _isVisible, _position, _rotation, _scale), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		if (!_material.empty()) {
			setMaterial(_material);
		}
	}

	void MeshAppearanceComponent::setVisibility(const bool visible) {
		_isVisible = visible;

		sendUpdateMessage();
	}

	void MeshAppearanceComponent::setMesh(const std::string & meshName) {
		_meshName = meshName;
		sendUpdateMessage();
	}

	void MeshAppearanceComponent::sendUpdateMessage() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMesh, core::Method::Update, new graphics::Graphics_Mesh_Update(_objOwnerID, getID(), _meshName, _isVisible), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MeshAppearanceComponent::setMaterial(const std::string & materialName) {
		_material = materialName;

		if (!_material.empty()) {
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMaterial, core::Method::Update, new graphics::Graphics_Material_Update(getID(), _objOwnerID, materialName), core::Subsystem::Object);

			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
		}
	}

	void MeshAppearanceComponent::setCustomParameter(uint32_t num, const Vec4 & value) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCusParam, core::Method::Update, new graphics::Graphics_CusParam_Update(getID(), _objOwnerID, num, value), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MeshAppearanceComponent::playAnimation(const std::string & anim, bool looping, double offsetPercent) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraPlayAnimation, core::Method::Update, new graphics::Graphics_PlayAnimation_Update(getID(), _objOwnerID, anim, looping, offsetPercent), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MeshAppearanceComponent::setAnimationSpeed(double animationSpeed) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraSetAnimationSpeed, core::Method::Update, new graphics::Graphics_SetAnimationSpeed_Update(getID(), _objOwnerID, animationSpeed), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void MeshAppearanceComponent::stopAnimation() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraStopAnimation, core::Method::Update, new graphics::Graphics_StopAnimation_Update(getID(), _objOwnerID), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	attributeMap MeshAppearanceComponent::synchronize() const {
		attributeMap params;

		_position.insertInMap("pos", params);
		_rotation.insertInMap("rot", params);
		_scale.insertInMap("scale", params);

		params["mesh"] = _meshName;
		params["visibility"] = boost::lexical_cast<std::string>(_isVisible);

		if (!_material.empty()) {
			params["material"] = _material;
		}

		return params;
	}

	std::pair<AddStrategy, int64_t> MeshAppearanceComponent::howToAdd(const ComPtr & comp) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> MeshAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Mesh", [this]() {
			return _meshName;
		}, [this](std::string s) {
			setMesh(s);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Visibility", [this]() {
			return boost::lexical_cast<std::string>(_isVisible);
		}, [this](std::string s) {
			try {
				_isVisible = boost::lexical_cast<bool>(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setVisibility(_isVisible);
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Material", [this]() {
			return _material;
		}, [this](std::string s) {
			setMaterial(s);
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
			// TODO: (Daniel) add setPosition method
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Rotation", [this]() {
			std::stringstream ss;
			ss << _rotation;
			return ss.str();
		}, [this](std::string s) {
			try {
				_rotation = Quaternion(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) add setRotation method
			return true;
		}));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Scale", [this]() {
			return _scale.toString();
		}, [this](std::string s) {
			try {
				_scale = Vec3(s);
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			// TODO: (Daniel) add setScale method
			return true;
		}));

		return result;
	}

	void MeshAppearanceComponent::drawBoundingBox(const Vec3 & colour) const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraDrawBB, core::Method::Update, new graphics::Graphics_DrawBB_Update(getID(), _objOwnerID, colour), core::Subsystem::Object));
	}

	void MeshAppearanceComponent::removeBoundingBox() const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraRemoveBB, core::Method::Update, new graphics::Graphics_RemoveBB_Update(getID(), _objOwnerID), core::Subsystem::Object));
	}

} /* namespace api */
} /* namespace i6engine */
