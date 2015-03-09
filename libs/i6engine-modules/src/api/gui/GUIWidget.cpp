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

#include "i6engine/api/gui/GUIWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace api {

	GUIWidget::~GUIWidget() {
		_window->destroy();
	}

	void GUIWidget::update(uint16_t type, gui::GUIUpdateMessageStruct * message) {
		if (type == gui::GUIMessageTypes::GuiSetDropTarget) {
			_dropable = dynamic_cast<gui::GUI_SetDropTarget *>(message)->dropable;
		}
	}

	void GUIWidget::loadWindowLayout(const std::string & name, const std::string & filename) {
		try {
			CEGUI::Window * objTemp = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename);
			_window = objTemp;
			objTemp->setName(name);
		} catch(CEGUI::Exception & e) {
			std::string strMsg = e.getMessage().c_str();
			ISIXE_THROW_API("GUI", strMsg);
		}
	}

	void GUIWidget::enableTicking(bool enabled) {
		if (enabled) {
			EngineController::GetSingletonPtr()->getGUIFacade()->addTicker(this);
		} else {
			EngineController::GetSingletonPtr()->getGUIFacade()->removeTicker(this);
		}
	}

	void GUIWidget::subscribeEvent(const std::string & windowName, const std::string & eventType, const boost::function<void(void)> & callback) {
		EngineController::GetSingletonPtr()->getGUIFacade()->subscribeEvent(windowName, eventType, callback);
	}

} /* namespace api */
} /* namespace i6engine */
