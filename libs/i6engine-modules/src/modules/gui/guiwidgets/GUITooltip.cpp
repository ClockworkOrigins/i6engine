#include "i6engine/modules/gui/guiwidgets/GUITooltip.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace modules {

	GUITooltip::GUITooltip(const std::string & name, const std::string & style) : GUIWidget(name) {
		loadWindowLayout(name, "Tooltip.layout");
		_window->setRiseOnClickEnabled(false);
		enableTicking(true);
	}

	GUITooltip::~GUITooltip() {
		enableTicking(false);
	}

	void GUITooltip::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetText) {
			api::gui::GUI_Text * g = dynamic_cast<api::gui::GUI_Text *>(data);
			setText(g->text);
		} else {
			GUIWidget::update(type, data);
		}
	}

	void GUITooltip::setText(const std::string & text) {
		CEGUI::Listbox * lb = dynamic_cast<CEGUI::Listbox *>(_window->getChild("Text"));
		while (lb->getItemCount() > 0) {
			lb->removeItem(lb->getListboxItemFromIndex(0));
		}

		auto vec = utils::split(text, "~");

		for (std::string s : vec) {
			lb->addItem(new CEGUI::ListboxTextItem(s));
		}
		lb->setVisible(false);
		lb->setVisible(true);

		setSize(lb->getWidestItemWidth() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width + 0.01, (lb->getTotalItemsHeight() / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height) + 0.015);
	}

	void GUITooltip::tick() {
		CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();

		int x = int(mousePos.d_x) - 50;

		if (x < 0) {
			x = 0;
		}

		if (x + 0.3 * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width > CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width) {
			x = int(0.7 * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width);
		}

		int y = int(mousePos.d_y + _window->getSize().d_height.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height);

		if (y + 0.05 * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height > CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height) {
			y = int(mousePos.d_y - _window->getSize().d_height.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height);
		}

		setPosition(x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width, y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height);
	}

} /* namespace modules */
} /* namespace i6engine */
