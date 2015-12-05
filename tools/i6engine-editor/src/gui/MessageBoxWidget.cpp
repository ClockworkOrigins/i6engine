#include "i6engine/editor/gui/MessageBoxWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace editor {
namespace gui {

	MessageBoxWidget::MessageBoxWidget(const std::string & name, const std::string & style) : GUIWidget(name) {
		loadWindowLayout(name, "MessageBox.layout");
		_window->getChild("Yes")->setName(name + "Yes");
		_window->getChild("No")->setName(name + "No");
		_window->setRiseOnClickEnabled(false);
	}

	MessageBoxWidget::~MessageBoxWidget() {
	}

	void MessageBoxWidget::update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) {
		if (type == i6engine::api::gui::GuiSetText) {
			i6engine::api::gui::GUI_Text * g = static_cast<i6engine::api::gui::GUI_Text *>(data);
			setText(g->text);
		} else {
			api::GUIWidget::update(type, data);
		}
	}

	void MessageBoxWidget::setText(const std::string & text) {
		CEGUI::Listbox * lb = dynamic_cast<CEGUI::Listbox *>(_window->getChild("Text"));
		size_t count = lb->getItemCount();
		if (count == 0) {
			lb->addItem(new CEGUI::ListboxTextItem(text, 0, 0, false, false));
		} else {
			CEGUI::ListboxTextItem * lbti = dynamic_cast<CEGUI::ListboxTextItem *>(lb->getListboxItemFromIndex(0));
			lbti->setText(text);
		}
	}

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
