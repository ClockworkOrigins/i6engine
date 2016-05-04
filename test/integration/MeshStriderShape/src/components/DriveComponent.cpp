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

#include "components/DriveComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "components/Config.h"

namespace i6e {
namespace integration {
namespace components {

	DriveComponent::DriveComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _counter(0), _sum(0.0), _sumCounter(0) {
		_objComponentID = config::ComponentTypes::DriveComponent;
	}

	api::ComPtr DriveComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<DriveComponent, api::Component>(id, params);
	}

	void DriveComponent::Init() {
		addTicker();

		api::EngineController::GetSingleton().getGUIFacade()->addPrint("JumpCounter", "RPG/Blanko", 0.01, 0.01, "0", api::gui::Alignment::Left, -1);
	}

	void DriveComponent::Finalize() {
		removeTicker();

		api::EngineController::GetSingleton().getGUIFacade()->deleteWidget("JumpCounter");
	}

	api::attributeMap DriveComponent::synchronize() const {
		return api::attributeMap();
	}

	void DriveComponent::Tick() {
		auto psc = getOwnerGO()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
		psc->applyCentralForce(Vec3(0.0, 0.0, -150.0), true);

		if (psc->getPosition().getX() < -100.0) {
			psc->setPosition(Vec3(100.0, 0.3, 0.0), 10);
		}

		if (_sumCounter < 1000) {
			_sum += psc->getPosition().getY();
			_sumCounter++;
		} else {
			if (std::abs(psc->getPosition().getY() - _sum / 1000.0) > 0.1 * _sum / 1000.0) {
				_counter++;
				api::EngineController::GetSingleton().getGUIFacade()->setText("JumpCounter", std::to_string(_counter));
			}
		}
	}

} /* namespace components */
} /* namespace integration */
} /* namespace i6e */
