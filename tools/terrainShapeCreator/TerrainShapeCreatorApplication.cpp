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

#include "TerrainShapeCreatorApplication.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/TerrainAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace tools {

	void TerrainShapeCreatorApplication::Initialize() {
	}

	void TerrainShapeCreatorApplication::AfterInitialize() {
		std::cout << "Loading Template '" << _goTemplate << "'" << std::endl;
		api::EngineController::GetSingleton().getObjectFacade()->createGO(_goTemplate, api::objects::GOTemplate(), api::EngineController::GetSingleton().getUUID(), false, [this](api::GOPtr go) {
			go->getGOC<api::TerrainAppearanceComponent>(api::components::ComponentTypes::TerrainAppearanceComponent)->saveCollisionShape(_outFile, std::bind(&api::EngineController::stop, api::EngineController::GetSingletonPtr()));
		});
	}

} /* namespace tools */
} /* namespace i6e */
