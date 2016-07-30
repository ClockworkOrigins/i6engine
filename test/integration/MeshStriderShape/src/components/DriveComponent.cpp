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

#include "components/DriveComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "MeshStriderShapeIntegrationTestApplication.h"

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

		i6eGUIFacade->addPrint("JumpCounter", "RPG/Blanko", 0.01, 0.01, "0", api::gui::Alignment::Left, -1);
	}

	void DriveComponent::Finalize() {
		removeTicker();

		i6eGUIFacade->deleteWidget("JumpCounter");
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
				i6eGUIFacade->setText("JumpCounter", std::to_string(_counter));
				if (_counter >= 100) {
					dynamic_cast<MeshStriderShapeIntegrationTestApplication *>(i6eAppl)->testFailed = true;
					i6eEngineController->stop();
				}
			}
		}
	}

} /* namespace components */
} /* namespace integration */
} /* namespace i6e */
