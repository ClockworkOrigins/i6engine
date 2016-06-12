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

#include "i6engine/api/configs/GUIConfig.h"

namespace i6e {
namespace api {
namespace gui {

	GUI_Add_Create::GUI_Add_Create(const std::string & s, const std::string & f, const std::string & df, const std::string & dmis, const std::string & dmin) : GUIUpdateMessageStruct("GUIManager"), scheme(s), font(f), defFont(df), defMouseImageSet(dmis), defMouseImageName(dmin) {
	}

	GUI_AddToRoot_Update::GUI_AddToRoot_Update(const std::string & c) : GUIUpdateMessageStruct("GUIManager"), child(c) {
	}

	GUI_MouseVisible_Update::GUI_MouseVisible_Update(const bool b) : GUIUpdateMessageStruct("GUIManager"), visible(b) {
	}

	GUI_SubscribeEvent_Update::GUI_SubscribeEvent_Update(const std::string & n, SubscribeEvent e, const boost::function<void(void)> & f) : GUIUpdateMessageStruct("GUIManager"), name(n), event(e), func(f) {
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
} /* namespace i6e */
