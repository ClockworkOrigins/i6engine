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

#include "i6engine/modules/gui/guiwidgets/GUIComboBox.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIComboBox::GUIComboBox(const std::string & name, const std::string & style) : api::GUIWidget(name), _selectedCallback() {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setUserData(this);
		_window->setProperty("RiseOnClickEnabled", "False");
		dynamic_cast<CEGUI::Combobox *>(_window)->setReadOnly(true);

		_window->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&GUIComboBox::selectionChanged, this));

		_window->setUsingAutoRenderingSurface(true);

		CEGUI::RenderingSurface* rs = _window->getRenderingSurface();
		if (rs) {
			rs->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueEnded, CEGUI::Event::Subscriber(&api::GUIWidget::renderingEndedHandler, dynamic_cast<api::GUIWidget *>(this)));
		}
	}

	void GUIComboBox::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiAddText) {
			api::gui::GUI_Text * g = dynamic_cast<api::gui::GUI_Text *>(data);
			dynamic_cast<CEGUI::Combobox *>(_window)->addItem(new CEGUI::ListboxTextItem(g->text));
		} else if (type == api::gui::GuiSetSelectedStringCallback) {
			api::gui::GUI_SetSelectedStringCallback * g = dynamic_cast<api::gui::GUI_SetSelectedStringCallback *>(data);
			_selectedCallback = g->callback;
		} else if (type == api::gui::GuiSetText) {
			std::string selection = dynamic_cast<api::gui::GUI_Text *>(data)->text;
			for (size_t i = 0; i < dynamic_cast<CEGUI::Combobox *>(_window)->getItemCount(); i++) {
				CEGUI::ListboxTextItem * lti = dynamic_cast<CEGUI::ListboxTextItem *>(dynamic_cast<CEGUI::Combobox *>(_window)->getListboxItemFromIndex(i));
				if (lti->getText() == selection) {
					lti->setSelected(true);
					dynamic_cast<CEGUI::Combobox *>(_window)->setText(selection);
					break;
				}
			}
		} else {
			GUIWidget::update(type, data);
		}
	}

	bool GUIComboBox::selectionChanged(const CEGUI::EventArgs &) {
		if (_selectedCallback == nullptr) {
			return true;
		}
		_selectedCallback(dynamic_cast<CEGUI::ListboxTextItem *>(dynamic_cast<CEGUI::Combobox *>(_window)->getSelectedItem())->getText().c_str());
		return true;
	}

} /* namespace modules */
} /* namespace i6e */
