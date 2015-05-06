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

#include "LuaGUIExport.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GUIFacade.h"

namespace i6engine {
namespace lua {
namespace gui {

	class Alignment {
	public:
		enum Align {
			Left,
			Center,
			Right
		};
	};

	void addImageset(const std::string & imageset) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addImageset(imageset);
	}

	void addPrint(const std::string & name, const std::string & type, double x, double y, const std::string & message, gui::Alignment::Align alignment, int64_t lifetime) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint(name, type, x, y, message, api::gui::Alignment(alignment), lifetime);
	}

	void addText(const std::string & name, const std::string & text) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addTextToWidget(name, text);
	}

	void setSize(const std::string & name, double w, double h) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setSize(name, w, h);
	}

} /* namespace gui */
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerGUI() {
	return
		def("addImageset", &i6engine::lua::gui::addImageset),
		def("addPrint", &i6engine::lua::gui::addPrint),
		def("addText", &i6engine::lua::gui::addText),
		def("setSize", &i6engine::lua::gui::setSize),
		class_<i6engine::lua::gui::Alignment>("Alignment")
			.def(constructor<>())
			.enum_("Align")
			[
				value("Left", i6engine::lua::gui::Alignment::Align::Left),
				value("Center", i6engine::lua::gui::Alignment::Align::Center),
				value("Right", i6engine::lua::gui::Alignment::Align::Right)
			]
		;
}
