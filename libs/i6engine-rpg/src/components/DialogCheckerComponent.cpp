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

#include "i6engine/rpg/components/DialogCheckerComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/components/Config.h"
#include "i6engine/rpg/config/ExternalConstants.h"
#include "i6engine/rpg/dialog/DialogManager.h"

namespace i6engine {
namespace rpg {
namespace components {

	DialogCheckerComponent::DialogCheckerComponent(int64_t id, const api::attributeMap & params) : Component(id, params), _psc(), _player(), _identifier() {
		_objFamilyID = config::ComponentTypes::DialogCheckerComponent;
		_objComponentID = config::ComponentTypes::DialogCheckerComponent;

		_identifier = params.find("ident")->second;
	}

	api::ComPtr DialogCheckerComponent::createC(int64_t id, const api::attributeMap & params) {
		ISIXE_THROW_API_COND("DialogCheckerComponent", "ident not found!", params.find("ident") != params.end());
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

				if ((psc->getPosition() - playerPSC->getPosition()).length() < i6engine::rpg::config::NPC_CHECK_TALK_DISTANCE) {
					std::cout << "Check" << std::endl;
					dialog::DialogManager::GetSingleton().checkImportantDialogs(_identifier);
				}
			}
		}
	}

	api::attributeMap DialogCheckerComponent::synchronize() const {
		api::attributeMap params;
		return params;
	}

} /* namespace components */
} /* namespace rpg */
} /* namespace i6engine */
