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

namespace i6e {
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
} /* namespace i6e */
