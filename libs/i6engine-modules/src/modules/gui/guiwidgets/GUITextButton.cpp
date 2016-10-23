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

#include "i6engine/modules/gui/guiwidgets/GUITextButton.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUITextButton::GUITextButton(const std::string & name, const std::string & type) : api::GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(type, name);
		_window->setUserData(this);

		_window->setProperty("RiseOnClickEnabled", "False");
	}

	GUITextButton::~GUITextButton() {
	}

	void GUITextButton::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetText) {
			_window->setText(static_cast<const api::gui::GUI_Text *>(data)->text);
		} else if (type == api::gui::GuiSetFont) {
			std::string font = static_cast<api::gui::GUI_Text *>(data)->text;
			_window->setFont(font);
		} else {
			GUIWidget::update(type, data);
		}
	}

} /* namespace modules */
} /* namespace i6e */
