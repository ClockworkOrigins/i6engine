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

#include "DragDropApplication.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"

#include "boost/bind.hpp"

namespace sample {

	DragDropApplication::DragDropApplication() : i6e::api::Application() {
	}

	DragDropApplication::~DragDropApplication() {
	}

	void DragDropApplication::Initialize() {
	}

	void DragDropApplication::AfterInitialize() {
		i6e::api::GUIFacade * gf = i6e::api::EngineController::GetSingleton().getGUIFacade();

		// register GUI scheme
		gf->startGUI("RPG.scheme", "", "", "RPG", "MouseArrow");

		// enter buttons for language switching
		gf->addImage("Background", "RPG/StaticImage", "RPG", "MainMenu", 0.0, 0.0, 1.0, 1.0);

		// 20 slots accepting drops
		for (size_t i = 0; i < 20; i++) {
			std::string image = "TbM_Filling";
			if (i / 5 == 2) {
				image = "Drag01";
			} else if (i / 5 == 3) {
				image = "Drag02";
			}
			gf->addImage("Slot" + std::to_string(i), "RPG/StaticImage", "RPG", image, 0.1 + 0.1666666 * (i % 5), 0.125 + 0.2 * (i / 5), 0.1333333, 0.175);
			gf->enableDropTarget("Slot" + std::to_string(i), true, [i](std::string s) {
				if (i / 5 < 2) { // the first two rows accept everything
					return true;
				} else if (i / 5 == 2) { // the third row accepts the two blue and the green image
					auto vec = i6e::utils::split(s, "_");
					if (vec.size() == 2) {
						if (vec[1] == "0" || vec[1] == "1" || vec[1] == "4") {
							return true;
						}
					}
				} else if (i / 5 == 3) { // the last row accepts the two red and the green image
					auto vec = i6e::utils::split(s, "_");
					if (vec.size() == 2) {
						if (vec[1] == "2" || vec[1] == "3" || vec[1] == "4") {
							return true;
						}
					}
				}
				return false;
			});
		}

		// 5 images being dragable
		for (size_t i = 0; i < 5; i++) {
			std::string image = "NewCloseButtonNormal";
			if (i >= 2 && i < 4) {
				image = "NewCloseButtonPressed";
			} else if (i == 4) {
				image = "NewCloseButtonHover";
			}
			gf->addImage("Image_" + std::to_string(i), "RPG/StaticImage", "RPG", image, 0.1 + 0.1666666 * (i % 5), 0.125 + 0.2 * (i / 5), 0.1333333, 0.175);
			gf->setDragable("Image_" + std::to_string(i), true);
		}

		// register ESC to close the application
		i6e::api::EngineController::GetSingletonPtr()->getInputFacade()->subscribeKeyEvent(i6e::api::KeyCode::KC_ESCAPE, i6e::api::KeyState::KEY_PRESSED, boost::bind(&i6e::api::EngineController::stop, i6e::api::EngineController::GetSingletonPtr()));
	}

	void DragDropApplication::Tick() {
	}

	bool DragDropApplication::ShutdownRequest() {
		return true;
	}

	void DragDropApplication::Finalize() {
	}

	void DragDropApplication::ShutDown() {
	}

} /* namespace sample */
