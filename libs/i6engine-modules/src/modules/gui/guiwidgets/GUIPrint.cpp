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

#include "i6engine/modules/gui/guiwidgets/GUIPrint.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "CEGUI/WindowManager.h"
#include "CEGUI/widgets/Listbox.h"
#include "CEGUI/widgets/ListboxTextItem.h"

namespace i6e {
namespace modules {

	GUIPrint::GUIPrint(const std::string & name, const std::string & type) : api::GUIWidget(name), _lbText(new CEGUI::ListboxTextItem("")), _lifeTime(-1), _alignment(api::gui::Alignment::Left), _text(), _realPosX(), _realPosY(), _startTime() {
		_lb = dynamic_cast<CEGUI::Listbox *>(CEGUI::WindowManager::getSingleton().createWindow(type, name));
		_window = _lb;
		_window->setUserData(this);
		_lb->addItem(_lbText);
		enableTicking(true);
	}

	GUIPrint::~GUIPrint() {
		enableTicking(false);
	}

	void GUIPrint::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetText) {
			_text = static_cast<const api::gui::GUI_Text *>(data)->text;
			setText(_text);
		} else if (type == api::gui::GuiSetPosition) {
			api::gui::GUI_Position * guiP = static_cast<api::gui::GUI_Position *>(data);
			setPosition(guiP->x, guiP->y);
			_realPosX = guiP->x;
			_realPosY = guiP->y;
			setText(_text);
		} else if (type == api::gui::GuiSetSize) {
			api::gui::GUI_Size * g = static_cast<api::gui::GUI_Size *>(data);
			setSize(g->width, g->height);
			setText(_text);
		} else if (type == api::gui::GuiSetColour) {
			api::gui::GUI_Colour * guiC = static_cast<api::gui::GUI_Colour *>(data);
			setColour(CEGUI::Colour(float(guiC->red), float(guiC->green), float(guiC->blue), float(guiC->alpha)));
		} else if (type == api::gui::GuiSetFont) {
			std::string text = static_cast<api::gui::GUI_Text *>(data)->text;
			setFont(text);
			setText(_text);
		} else if (type == api::gui::GuiSetLifetime) {
			api::gui::GUI_Lifetime * guiP = static_cast<api::gui::GUI_Lifetime *>(data);
			setTimeToDie(guiP->lifetime);
		} else if (type == api::gui::GuiSetAlignment) {
			_alignment = static_cast<api::gui::GUI_Alignment *>(data)->alignment;
			setText(_text);
		} else {
			GUIWidget::update(type, data);
		}
	}

	void GUIPrint::setText(const std::string & text) {
		_lbText->setText(text);
		if (_alignment == api::gui::Alignment::Left) {
			setPosition(_realPosX, _realPosY);
		} else if (_alignment == api::gui::Alignment::Center) {
			setPosition(_realPosX - (_lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width + 0.01) / 2, _realPosY);
		} else if (_alignment == api::gui::Alignment::Right) {
			setPosition((_realPosX + _window->getSize().d_width.d_scale) - (_lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width + 0.01), _realPosY);
		}
		bool vis = _lb->isVisible();
		_lb->setVisible(false);
		_lb->setVisible(vis);
	}

	void GUIPrint::setColour(const CEGUI::Colour & col) {
		_lbText->setTextColours(col);
	}

	void GUIPrint::setFont(const std::string & font_name) {
		_lbText->setFont(font_name);
	}

	void GUIPrint::setTimeToDie(const int64_t lifetime) {
		_lifeTime = lifetime;
		_startTime = api::EngineController::GetSingleton().getCurrentTime();
	}

	void GUIPrint::tick() {
		if (_lifeTime == -1) {
			return;
		}
		if (uint64_t(int64_t(_startTime) + _lifeTime) <= api::EngineController::GetSingleton().getCurrentTime()) {
			i6eGUIFacade->deleteWidget(_name);
		}
	}

} /* namespace modules */
} /* namespace i6e */
