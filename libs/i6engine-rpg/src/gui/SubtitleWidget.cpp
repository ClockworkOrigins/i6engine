#include "i6engine/rpg/gui/SubtitleWidget.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/rpg/gui/Config.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace rpg {
namespace gui {

	SubtitleWidget::SubtitleWidget(const std::string & name, const std::string & style) : GUIWidget(name), _text(), _font(), _lines(), _listboxStyle() {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();
		_window = wmgr.createWindow(style, name);
		_window->setRiseOnClickEnabled(true);
		_window->setMousePassThroughEnabled(true);
	}

	SubtitleWidget::~SubtitleWidget() {
	}

	void SubtitleWidget::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetText) {
			api::gui::GUI_Text * g = dynamic_cast<api::gui::GUI_Text *>(data);
			setText(g->text);
		} else if (type == api::gui::GuiSetFont) {
			api::gui::GUI_Text * g = dynamic_cast<api::gui::GUI_Text *>(data);
			_font = g->text;
			setText(_text);
		} else if (type == rpg::gui::GUIMessageTypes::SetListboxStyle) {
			GUI_SetListboxStyle * g = dynamic_cast<GUI_SetListboxStyle *>(data);
			_listboxStyle = g->style;
		} else {
			GUIWidget::update(type, data);
		}
	}

	void SubtitleWidget::setText(const std::string & text) {
		for (CEGUI::Window * lb : _lines) {
			_window->destroyChild(lb);
		}
		_text = text;
		_lines.clear();
		auto vec = utils::split(text, " ");
		if (vec.empty()) {
			return;
		}
		CEGUI::Listbox * lb = dynamic_cast<CEGUI::Listbox *>(_window->createChild(_listboxStyle, _name + "_Text_0"));
		lb->addItem(new CEGUI::ListboxTextItem(""));
		if (!_font.empty()) {
			dynamic_cast<CEGUI::ListboxTextItem *>(lb->getListboxItemFromIndex(0))->setFont(_font);
		}
		std::string last = "";
		std::string current = vec[0];
		for (size_t i = 1; i < vec.size(); i++) {
			last = current;
			current += " " + vec[i];
			dynamic_cast<CEGUI::ListboxTextItem *>(lb->getListboxItemFromIndex(0))->setText(current);
			if (lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width >= _window->getSize().d_width.d_scale) {
				dynamic_cast<CEGUI::ListboxTextItem *>(lb->getListboxItemFromIndex(0))->setText(last);
				// center listbox
				lb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f - ((lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width) / 2) / _window->getSize().d_width.d_scale, 0.0f), CEGUI::UDim((_window->getPosition().d_y.d_scale + 0.01f + (lb->getTotalItemsHeight() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height) * _lines.size()) / _window->getSize().d_height.d_scale, 0.0f)));
				float realWorldScale = lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width + 0.015f;
				realWorldScale = (realWorldScale / _window->getSize().d_width.d_scale);
				lb->setSize(CEGUI::USize(CEGUI::UDim(realWorldScale, 0.0f), CEGUI::UDim((lb->getTotalItemsHeight() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height + 0.015f) / _window->getSize().d_height.d_scale, 0.0)));
				_lines.push_back(lb);
				lb = dynamic_cast<CEGUI::Listbox *>(_window->createChild(_listboxStyle, _name + "_Text_" + std::to_string(_lines.size())));
				lb->addItem(new CEGUI::ListboxTextItem(""));
				if (!_font.empty()) {
					dynamic_cast<CEGUI::ListboxTextItem *>(lb->getListboxItemFromIndex(0))->setFont(_font);
				}
				last = "";
				current = vec[i];
			}
		}
		dynamic_cast<CEGUI::ListboxTextItem *>(lb->getListboxItemFromIndex(0))->setText(current);
		lb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f - ((lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width) / 2) / _window->getSize().d_width.d_scale, 0.0f), CEGUI::UDim((_window->getPosition().d_y.d_scale + 0.01f + (lb->getTotalItemsHeight() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height) * _lines.size()) / _window->getSize().d_height.d_scale, 0.0f)));
		float realWorldScale = lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width + 0.015f;
		realWorldScale = (realWorldScale / _window->getSize().d_width.d_scale);
		lb->setSize(CEGUI::USize(CEGUI::UDim(realWorldScale, 0.0f), CEGUI::UDim((lb->getTotalItemsHeight() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height + 0.015f) / _window->getSize().d_height.d_scale, 0.0)));
		_lines.push_back(lb);
	}

} /* namespace gui */
} /* namespace rpg */
} /* namespace i6e */
