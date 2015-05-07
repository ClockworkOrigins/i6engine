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

#include "components/ScriptingShatterComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "components/Config.h"

namespace sample {
namespace components {

	ScriptingShatterComponent::ScriptingShatterComponent(int64_t id, const i6engine::api::attributeMap & params) : ShatterComponent(id, params) {
		_objComponentID = config::ComponentTypes::ScriptingShatterComponent;
	}

	i6engine::api::ComPtr ScriptingShatterComponent::createC(int64_t id, const i6engine::api::attributeMap & params) {
		return i6engine::utils::make_shared<ScriptingShatterComponent, i6engine::api::Component>(id, params);
	}

	void ScriptingShatterComponent::Init() {
		ShatterComponent::Init();

		addTicker();
	}

	void ScriptingShatterComponent::Finalize() {
		removeTicker();
	}

	i6engine::api::attributeMap ScriptingShatterComponent::synchronize() const {
		i6engine::api::attributeMap params = ShatterComponent::synchronize();

		return params;
	}

	void ScriptingShatterComponent::Tick() {
		i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>("MoveScript", "tick", _objOwnerID);
	}

	void ScriptingShatterComponent::shatter(const i6engine::api::GOPtr & other) {
		i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>("MoveScript", "shatter", _objOwnerID, other->getID());
	}

} /* namespace components */
} /* namespace sample */
