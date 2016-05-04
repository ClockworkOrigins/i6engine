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

#include "i6engine/modules/gui/guiwidgets/GUIImage.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIImage::GUIImage(const std::string & name, const std::string & style) : api::GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		// style has to support parameter "image"
		_window = wmgr.createWindow(style, name);
		_window->setProperty("RiseOnClickEnabled", "False");
	}

	void GUIImage::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetImage) {
			api::gui::GUI_Image * g = static_cast<api::gui::GUI_Image *>(data);
			setImage(g->imageSetName, g->imageName);
		} else {
			GUIWidget::update(type, data);
		}
	}

	void GUIImage::setImage(const std::string & imageSetName, const std::string & imageName) {
		_window->setProperty("Image", imageSetName + "/" + imageName);
	}

} /* namespace modules */
} /* namespace i6e */
