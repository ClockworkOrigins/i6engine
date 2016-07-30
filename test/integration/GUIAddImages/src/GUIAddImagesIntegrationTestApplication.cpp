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

#include "GUIAddImagesIntegrationTestApplication.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GUIFacade.h"

namespace i6e {
namespace integration {

	GUIAddImagesIntegrationTestApplication::GUIAddImagesIntegrationTestApplication() : CommonApplication(true, false) {
	}

	GUIAddImagesIntegrationTestApplication::~GUIAddImagesIntegrationTestApplication() {
	}

	void GUIAddImagesIntegrationTestApplication::AfterInitialize() {
		CommonApplication::AfterInitialize();

		for (size_t i = 0; i < 400; i++) {
			i6eGUIFacade->addImage("Image_" + std::to_string(i), "RPG/StaticImage", "RPG_Inventory_Slot", "Slot", 0.0, 0.0, 1.0, 1.0);
		}
		i6eEngineController->registerTimer(10000000, [this]() {
			i6eEngineController->stop();
			return false;
		}, false, i6e::core::JobPriorities::Prio_Low);
	}

} /* namespace integration */
} /* namespace i6e */
