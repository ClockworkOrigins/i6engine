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

#include "i6engine/api/configs/GUIConfig.h"

namespace i6engine {
namespace api {
namespace gui {

	GUI_Add_Create::GUI_Add_Create(const std::string & s, const std::string & f, const std::string & df, const std::string & dmis, const std::string & dmin) : GUIUpdateMessageStruct("GUIManager"), scheme(s), font(f), defFont(df), defMouseImageSet(dmis), defMouseImageName(dmin) {
	}

	GUI_AddToRoot_Update::GUI_AddToRoot_Update(const std::string & c) : GUIUpdateMessageStruct("GUIManager"), child(c) {
	}

	GUI_MouseVisible_Update::GUI_MouseVisible_Update(const bool b) : GUIUpdateMessageStruct("GUIManager"), visible(b) {
	}

	GUI_SubscribeEvent_Update::GUI_SubscribeEvent_Update(const std::string & n, const std::string & e, const boost::function<void(void)> & f) : GUIUpdateMessageStruct("GUIManager"), name(n), event(e), func(f) {
	}

	GUI_Child_Update::GUI_Child_Update(const std::string & n, const std::string & c) : GUIUpdateMessageStruct("GUIManager"), name(n), child(c) {
	}

	GUI_Event_Update::GUI_Event_Update(const std::string & n, bool b) : GUIUpdateMessageStruct("GUIManager"), name(n), enabled(b) {
	}

	GUI_WidgetTemplate_Create::GUI_WidgetTemplate_Create(const std::string & n, const boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> & f) : GUIUpdateMessageStruct("GUIManager"), name(n), func(f) {
	}

	GUI_Window_Create::GUI_Window_Create(void * w) : GUIUpdateMessageStruct("GUIManager"), window(w) {}

	GUI_MouseCursorImage_Update::GUI_MouseCursorImage_Update(const std::string & i) : GUIUpdateMessageStruct("GUIManager"), image(i) {
	}

	GUI_Resolution_Update::GUI_Resolution_Update(const graphics::Resolution & r) : GUIUpdateMessageStruct("GUIManager"), resolution(r) {
	}

	GUI_AddImageset_Create::GUI_AddImageset_Create(const std::string & i) : GUIUpdateMessageStruct("GUIManager"), imageset(i) {
	}

	GUI_LoadCanvas_Create::GUI_LoadCanvas_Create(const std::string & n, const std::string & f) : GUIUpdateMessageStruct("GUIManager"), name(n), file(f) {
	}

} /* namespace gui */
} /* namespace api */
} /* namespace i6engine */
