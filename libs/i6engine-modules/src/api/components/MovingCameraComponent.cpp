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

namespace i6engine {
namespace api {

	MovingCameraComponent::MovingCameraComponent(const int64_t id, const attributeMap & params) : CameraComponent(id, params), _oldPos() {
		Component::_objComponentID = components::MovingCameraComponent;
	}

	MovingCameraComponent::~MovingCameraComponent() {
	}

	void MovingCameraComponent::Init() {
		CameraComponent::Init();
		addTicker();
	}

	void MovingCameraComponent::Finalize() {
		removeTicker();
		CameraComponent::Finalize();
	}

	void MovingCameraComponent::Tick() {
		GOPtr go = getOwnerGO();
		if (go->getOwner() != EngineController::GetSingletonPtr()->getNetworkFacade()->getIP()) {
			return;
		}

		utils::sharedPtr<PhysicalStateComponent, Component> psc = go->getGOC<PhysicalStateComponent>(components::PhysicalStateComponent);

		// newPos := target position and LookAt Point
		Vec3 targetPos = psc->getPosition();
		Vec3 camPosTarget = targetPos + math::rotateVector(_position, psc->getRotation());
		Vec3 camPosCur = _oldPos;
		// movePos := position where camera will be placed
		// _oldPOs := old camera position
		Vec3 movePos = camPosCur + (camPosTarget - camPosCur) * 1.0;
		if (_oldPos != movePos) {
			Vec3 newPos = movePos - targetPos;
			Vec3 newPosRotated = math::rotateVector(newPos, psc->getRotation().inverse());
			GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraCamera, core::Method::Update, new graphics::Graphics_Camera_Update(_objOwnerID, _id, newPosRotated, targetPos, _nearClip, _aspect, _fov), core::Subsystem::Object);
			EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
			_oldPos = movePos;
		}
	}

} /* namespace api */
} /* namespace i6engine */
