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

#include "i6engine/modules/gui/guiwidgets/GUIBar.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIBar::GUIBar(const std::string & name, const std::string & style) : api::GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setUserData(this);
	}

	void GUIBar::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetProgress) {
			api::gui::GUI_Progress * g = static_cast<api::gui::GUI_Progress *>(data);
			setProgress(g->progress);
		} else {
			GUIWidget::update(type, data);
		}
	}

	void GUIBar::setProgress(double f) {
		static_cast<CEGUI::ProgressBar *>(_window)->setProgress(float(f));
	}

} /* namespace modules */
} /* namespace i6e */
