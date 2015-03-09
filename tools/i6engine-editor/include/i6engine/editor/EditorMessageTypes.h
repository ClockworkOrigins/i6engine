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

#include "i6engine/api/configs/GUIConfig.h"

namespace i6engine {
namespace editor {
namespace messages {

	enum GUIMessageTypes {
		EditorReset = i6engine::api::gui::GuiMessageTypesCount + 1,
		AddMenuBarEntry,
		SetFileDialogBaseDir,
		AddStringCallback,
		RemoveEntries,
		AddEntry,
		AddComponent,
		AddComponentOption,
		ApplyChanges
	};

	typedef struct GUI_MenuBarEntry : public i6engine::api::gui::GUIUpdateMessageStruct {
		std::string menu;
		std::string submenu;
		boost::function<void(void)> callback;
		GUI_MenuBarEntry(const std::string & name, const std::string & m, const std::string & sm, const boost::function<void(void)> & cb) : GUIUpdateMessageStruct(name), menu(m), submenu(sm), callback(cb) {
		}
		GUI_MenuBarEntry * copy() {
			return new GUI_MenuBarEntry(*this);
		}
	} GUI_MenuBarEntry;

	typedef struct GUI_SetFileDialogBaseDir : public i6engine::api::gui::GUIUpdateMessageStruct {
		std::string baseDir;
		GUI_SetFileDialogBaseDir(const std::string & name, const std::string & b) : GUIUpdateMessageStruct(name), baseDir(b) {
		}
		GUI_SetFileDialogBaseDir * copy() {
			return new GUI_SetFileDialogBaseDir(*this);
		}
	} GUI_SetFileDialogBaseDir;

	typedef struct GUI_AddStringCallback : public i6engine::api::gui::GUIUpdateMessageStruct {
		boost::function<void(std::string)> callback;
		GUI_AddStringCallback(const std::string & name, const boost::function<void(std::string)> & cb) : GUIUpdateMessageStruct(name), callback(cb) {
		}
		GUI_AddStringCallback * copy() {
			return new GUI_AddStringCallback(*this);
		}
	} GUI_AddStringCallback;

	typedef struct GUI_RemoveEntries : public i6engine::api::gui::GUIUpdateMessageStruct {
		GUI_RemoveEntries(const std::string & name) : GUIUpdateMessageStruct(name) {
		}
		GUI_RemoveEntries * copy() {
			return new GUI_RemoveEntries(*this);
		}
	} GUI_RemoveEntries;

	typedef struct GUI_AddEntry : public i6engine::api::gui::GUIUpdateMessageStruct {
		std::string objName;
		boost::function<void(void)> callback;
		GUI_AddEntry(const std::string & name, const std::string & o, const boost::function<void(void)> & cb) : GUIUpdateMessageStruct(name), objName(o), callback(cb) {
		}
		GUI_AddEntry * copy() {
			return new GUI_AddEntry(*this);
		}
	} GUI_AddEntry;

	typedef struct GUI_AddComponent : public i6engine::api::gui::GUIUpdateMessageStruct {
		std::string component;
		std::string identifier;
		GUI_AddComponent(const std::string & name, const std::string & c, const std::string & i) : GUIUpdateMessageStruct(name), component(c), identifier(i) {
		}
		GUI_AddComponent * copy() {
			return new GUI_AddComponent(*this);
		}
	} GUI_AddComponent;

	typedef struct GUI_AddComponentOption : public i6engine::api::gui::GUIUpdateMessageStruct {
		bool writeable;
		std::string optionName;
		boost::function<std::string(void)> getter;
		boost::function<bool(std::string)> setter;
		GUI_AddComponentOption(const std::string & name, bool w, const std::string & o, const boost::function<std::string(void)> & g, const boost::function<bool(std::string)> & s) : GUIUpdateMessageStruct(name), writeable(w), optionName(o), getter(g), setter(s) {
		}
		GUI_AddComponentOption * copy() {
			return new GUI_AddComponentOption(*this);
		}
	} GUI_AddComponentOption;

	typedef struct GUI_ApplyChanges : public i6engine::api::gui::GUIUpdateMessageStruct {
		GUI_ApplyChanges(const std::string & name) : GUIUpdateMessageStruct(name) {
		}
		GUI_ApplyChanges * copy() {
			return new GUI_ApplyChanges(*this);
		}
	} GUI_ApplyChanges;

} /* namespace messages */
} /* namespace editor */
} /* namespace i6engine */
