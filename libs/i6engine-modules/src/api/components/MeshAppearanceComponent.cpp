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

namespace i6engine {
namespace api {

	MeshAppearanceComponent::MeshAppearanceComponent(const int64_t id, const attributeMap & params) : Component(id, params), _meshName(), _isVisible(), _position(), _rotation(), _scale(), _material(), _boneTransformLock(), _boneTransforms() {
		Component::_objFamilyID = components::MeshAppearanceComponent;
		Component::_objComponentID = components::MeshAppearanceComponent;

		parseAttribute<true>(params, "mesh", _meshName);
		parseAttribute<true>(params, "visibility", _isVisible);
		parseAttribute<true>(params, "pos", _position);
		parseAttribute<true>(params, "rot", _rotation);
		parseAttribute<true>(params, "scale", _scale);
		parseAttribute<false>(params, "material", _material);
	}

	MeshAppearanceComponent::~MeshAppearanceComponent() {
		// TODO: (Daniel) move this to Finalize method
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraMesh, core::Method::Delete, new graphics::Graphics_Mesh_Delete(_objOwnerID, getID()), core::Subsystem::Object);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
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
		params["visibility"] = std::to_string(_isVisible);

		if (!_material.empty()) {
			params["material"] = _material;
		}

		return params;
	}

	std::pair<AddStrategy, int64_t> MeshAppearanceComponent::howToAdd(const ComPtr &) const {
		return std::make_pair(AddStrategy::ADD, 0);
	}

	std::vector<componentOptions> MeshAppearanceComponent::getComponentOptions() {
		std::vector<componentOptions> result;

		result.push_back(std::make_tuple(AccessState::READWRITE, "Mesh", [this]() {
			return _meshName;
		}, [this](std::string s) {
			setMesh(s);
			return true;
		}, "String"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Visibility", [this]() {
			return std::to_string(_isVisible);
		}, [this](std::string s) {
			try {
				_isVisible = std::stoi(s) == 1;
			} catch (boost::bad_lexical_cast &) {
				return false;
			}
			setVisibility(_isVisible);
			return true;
		}, "Bool"));
		result.push_back(std::make_tuple(AccessState::READWRITE, "Material", [this]() {
			return _material;
		}, [this](std::string s) {
			setMaterial(s);
			return true;
		}, "String"));
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
		}, "Vec3"));
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
		}, "Quaternion"));
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
		}, "Vec3"));

		return result;
	}

	void MeshAppearanceComponent::drawBoundingBox(const Vec3 & colour) const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraDrawBB, core::Method::Update, new graphics::Graphics_DrawBB_Update(getID(), _objOwnerID, colour), core::Subsystem::Object));
	}

	void MeshAppearanceComponent::removeBoundingBox() const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraRemoveBB, core::Method::Update, new graphics::Graphics_RemoveBB_Update(getID(), _objOwnerID), core::Subsystem::Object));
	}

	void MeshAppearanceComponent::attachGameObjectToBone(const api::GOPtr & go, const std::string & boneName) {
		if (go->getID() == _objOwnerID) {
			ISIXE_THROW_API("MeshAppearanceComponent", "Can't attach GameObject to itself!");
		}
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraAttachToBone, core::Method::Update, new graphics::Graphics_AttachToBone_Update(getID(), _objOwnerID, go->getID(), boneName), core::Subsystem::Object));
	}

	void MeshAppearanceComponent::detachGameObjectFromBone(const api::GOPtr & go, const std::string & boneName) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraDetachFromBone, core::Method::Update, new graphics::Graphics_DetachFromBone_Update(getID(), _objOwnerID, go->getID(), boneName), core::Subsystem::Object));
	}

	void MeshAppearanceComponent::addAnimationFrameEvent(uint64_t frameTime, const std::function<void(void)> & func) const {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraAnimationFrameEvent, core::Method::Update, new graphics::Graphics_AnimationFrameEvent_Update(getID(), _objOwnerID, frameTime, func), core::Subsystem::Object));
	}

	Transform MeshAppearanceComponent::getBoneTransform(const std::string & name) const {
		std::lock_guard<std::mutex> lg(_boneTransformLock);
		auto it = _boneTransforms.find(name);
		if (it != _boneTransforms.end()) {
			return it->second;
		} else {
			return Transform(Vec3::ZERO, Quaternion::IDENTITY);
		}
	}

	void MeshAppearanceComponent::updateBoneTransforms(const std::map<std::string, Transform> & boneTransformMap) {
		std::lock_guard<std::mutex> lg(_boneTransformLock);
		_boneTransforms = boneTransformMap;
	}

} /* namespace api */
} /* namespace i6engine */
