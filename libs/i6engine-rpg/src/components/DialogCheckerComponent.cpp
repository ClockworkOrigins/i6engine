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

#include "i6engine/rpg/components/DialogCheckerComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/config/ExternalConstants.h"
#include "i6engine/rpg/dialog/DialogManager.h"

namespace i6e {
namespace rpg {
namespace components {

	DialogCheckerComponent::DialogCheckerComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _psc(), _player(), _identifier() {
		_objFamilyID = config::ComponentTypes::DialogCheckerComponent;
		_objComponentID = config::ComponentTypes::DialogCheckerComponent;

		api::parseAttribute<true>(params, "ident", _identifier);
	}

	api::ComPtr DialogCheckerComponent::createC(int64_t id, const api::attributeMap & params) {
		return utils::make_shared<DialogCheckerComponent, api::Component>(id, params);
	}

	void DialogCheckerComponent::Init() {
		_psc = getOwnerGO()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
		auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
		if (!playerList.empty()) {
			_player = *playerList.begin();
		}
		addTicker();
	}

	void DialogCheckerComponent::Finalize() {
		removeTicker();
	}

	void DialogCheckerComponent::Tick() {
		auto psc = _psc.get();
		if (psc != nullptr) {
			auto player = _player.get();
			if (player == nullptr) {
				auto playerList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("Player");
				if (!playerList.empty()) {
					_player = *playerList.begin();
				}
			}
			player = _player.get();
			if (player != nullptr) {
				auto playerPSC = player->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);

				if ((psc->getPosition() - playerPSC->getPosition()).length() < i6e::rpg::config::NPC_CHECK_TALK_DISTANCE) {
					dialog::DialogManager::GetSingleton().checkImportantDialogs(_identifier);
				}
			}
		}
	}

	api::attributeMap DialogCheckerComponent::synchronize() const {
		api::attributeMap params;
		api::writeAttribute(params, "ident", _identifier);
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6e */
