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

#pragma once

#include <map>

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class Combobox;
	class EventArgs;
} /* namespace CEGUI */

namespace i6engine {
namespace editor {
namespace gui {

	class MenuBarWidget : public api::GUIWidget {
	public:
		MenuBarWidget(const std::string & name, const std::string & type);

		~MenuBarWidget();

		void update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) override;

	private:
		std::map<std::string, CEGUI::Combobox *> _menus;
		std::map<std::pair<std::string, std::string>, boost::function<void(void)>> _callbacks;

		void setSize(double w, double h);

		bool clickedFileMenu(const CEGUI::EventArgs & e);
	};

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
