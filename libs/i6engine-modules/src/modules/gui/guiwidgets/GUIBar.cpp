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

#include "i6engine/modules/gui/guiwidgets/GUIBar.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace modules {

	GUIBar::GUIBar(const std::string & name, const std::string & style) : api::GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
	}

	void GUIBar::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetPosition) {
			const api::gui::GUI_Position * g = static_cast<api::gui::GUI_Position *>(data);
			setPosition(g->x, g->y);
		} else if (type == api::gui::GuiSetProgress) {
			const api::gui::GUI_Progress * g = static_cast<api::gui::GUI_Progress *>(data);
			setProgress(g->progress);
		} else if (type == api::gui::GuiSetSize) {
			const api::gui::GUI_Size * g = static_cast<api::gui::GUI_Size *>(data);
			setSize(g->width, g->height);
		} else if (type == api::gui::GuiSetVisible) {
			bool vis = static_cast<api::gui::GUI_Visibility *>(data)->visible;
			_window->setVisible(vis);
		} else {
			ISIXE_THROW_API("GUI", "Don't know what to do with " << type);
		}
	}

	void GUIBar::setPosition(double x, double y) {
		_window->setPosition(CEGUI::UVector2(CEGUI::UDim(float(x), 0.0f), CEGUI::UDim(float(y), 0.0f)));
	}

	void GUIBar::setProgress(double f) {
		static_cast<CEGUI::ProgressBar *>(_window)->setProgress(float(f));
	}

	void GUIBar::setSize(double w, double h) {
		_window->setSize(CEGUI::USize(CEGUI::UDim(float(w), 0.0f), CEGUI::UDim(float(h), 0.0f)));
	}

} /* namespace modules */
} /* namespace i6engine */
