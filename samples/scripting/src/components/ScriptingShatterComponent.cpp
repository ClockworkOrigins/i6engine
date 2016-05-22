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

#include "components/ScriptingShatterComponent.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "components/Config.h"

namespace sample {
namespace components {

	ScriptingShatterComponent::ScriptingShatterComponent(int64_t id, const i6e::api::attributeMap & params) : ShatterComponent(id, params) {
		_objComponentID = config::ComponentTypes::ScriptingShatterComponent;
	}

	i6e::api::ComPtr ScriptingShatterComponent::createC(int64_t id, const i6e::api::attributeMap & params) {
		return i6e::utils::make_shared<ScriptingShatterComponent, i6e::api::Component>(id, params);
	}

	void ScriptingShatterComponent::Init() {
		ShatterComponent::Init();

		addTicker();

		i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>("MoveScript", "init", _objOwnerID);
	}

	void ScriptingShatterComponent::Finalize() {
		removeTicker();
	}

	i6e::api::attributeMap ScriptingShatterComponent::synchronize() const {
		i6e::api::attributeMap params = ShatterComponent::synchronize();

		return params;
	}

	void ScriptingShatterComponent::Tick() {
		i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>("MoveScript", "tick", _objOwnerID);
	}

	void ScriptingShatterComponent::shatter(const i6e::api::GOPtr & other) {
		i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>("MoveScript", "shatter", _objOwnerID, other->getID());
	}

} /* namespace components */
} /* namespace sample */
