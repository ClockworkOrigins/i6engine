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
} /* namespace i6engine */
