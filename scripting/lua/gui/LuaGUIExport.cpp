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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "i6engine/luabind/luabind.hpp"

namespace i6engine {
namespace lua {
namespace gui {

	void addImageset(const std::string & imageset) {
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addImageset(imageset);
	}

	void addText(const std::string & name, const std::string & text) {
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addTextToWidget(name, text);
	}

} /* namespace gui */
} /* namespace lua */
} /* namespace i6engine */

extern "C" int init(lua_State * L) {
	using namespace luabind;

	open(L);

	module(L)
		[
			def("addImageset", &i6engine::lua::gui::addImageset),
			def("addText", &i6engine::lua::gui::addText)
		];

	return 0;
}
