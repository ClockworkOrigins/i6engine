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

#include "i6engine/editor/gui/FileDialogWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "i6engine/editor/EditorMessageTypes.h"

#include "boost/filesystem.hpp"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace editor {
namespace gui {

	FileDialogWidget::FileDialogWidget(const std::string & name, const std::string & style) : GUIWidget(name), _currentPath(), _callback() {
		loadWindowLayout(name, "FileDialog.layout");
		_window->setRiseOnClickEnabled(false);

		_window->getChild("FileDialogList")->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&FileDialogWidget::changedSelection, this));
		_window->getChild("FileDialogButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FileDialogWidget::clickedButton, this));
	}

	FileDialogWidget::~FileDialogWidget() {
	}

	void FileDialogWidget::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetSize) {
			api::gui::GUI_Size * g = static_cast<api::gui::GUI_Size *>(data);
			setSize(g->width, g->height);
		} else if (type == api::gui::GuiSetText) {
			api::gui::GUI_Text * g = static_cast<api::gui::GUI_Text *>(data);
			dynamic_cast<CEGUI::PushButton *>(_window->getChild("FileDialogButton"))->setText(g->text);
		} else if (type == messages::GUIMessageTypes::SetFileDialogBaseDir) {
			_currentPath = dynamic_cast<messages::GUI_SetFileDialogBaseDir *>(data)->baseDir;
			updateView();
		} else if (type == messages::GUIMessageTypes::AddStringCallback) {
			_callback = dynamic_cast<messages::GUI_AddStringCallback *>(data)->callback;
		} else {
			GUIWidget::update(type, data);
		}
	}

	void FileDialogWidget::updateView() {
		CEGUI::Listbox * lb = dynamic_cast<CEGUI::Listbox *>(_window->getChild("FileDialogList"));

		while (lb->getItemCount()) {
			lb->removeItem(lb->getListboxItemFromIndex(0));
		}

		boost::filesystem::directory_iterator iter(_currentPath), dirEnd;
		lb->addItem(new CEGUI::ListboxTextItem(".."));
		lb->addItem(new CEGUI::ListboxTextItem("."));
		while (iter != dirEnd) {
			if (is_regular_file(*iter)) {
				if (iter->path().extension().string() != ".xml") {
					iter++;
					continue;
				}
			}
			lb->addItem(new CEGUI::ListboxTextItem(iter->path().filename().string()));
			iter++;
		}
	}

	bool FileDialogWidget::changedSelection(const CEGUI::EventArgs & e) {
		CEGUI::ListboxTextItem * selected = dynamic_cast<CEGUI::ListboxTextItem *>(dynamic_cast<CEGUI::Listbox *>(_window->getChild("FileDialogList"))->getFirstSelectedItem());
		std::string text = selected->getText().c_str();
		if (text.length() > 4 && text.substr(text.length() - 4, 4) == ".xml") {
			dynamic_cast<CEGUI::Editbox *>(_window->getChild("FileDialogEditbox"))->setText(text);
		} else {
			_currentPath += ("/" + text);
			updateView();
			dynamic_cast<CEGUI::Editbox *>(_window->getChild("FileDialogEditbox"))->setText("");
		}
		return true;
	}

	bool FileDialogWidget::clickedButton(const CEGUI::EventArgs & e) {
		std::string text = std::string(dynamic_cast<CEGUI::Editbox *>(_window->getChild("FileDialogEditbox"))->getText().c_str());
		if (!text.empty()) {
			_callback(_currentPath.string() + "/" + text);
		}
		return true;
	}

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
