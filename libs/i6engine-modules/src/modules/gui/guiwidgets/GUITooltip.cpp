#include "i6engine/modules/gui/guiwidgets/GUITooltip.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUITooltip::GUITooltip(const std::string & name, const std::string &) : GUIWidget(name) {
		loadWindowLayout(name, "Tooltip.layout");
		_window->setRiseOnClickEnabled(false);
		_window->setMousePassThroughEnabled(true);
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

		float tooltipWidth = _window->getSize().d_width.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width;
		float tooltipHeight = _window->getSize().d_height.d_scale * CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height;

		float x = mousePos.d_x - tooltipWidth / 2.0f;
		if (x < 0) {
			x = 0;
		} else if (x > CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - tooltipWidth) {
			x = CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - tooltipWidth;
		}

		float y = mousePos.d_y;
		if (y < CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height / 2.0f) {
			y += 25; // 25 pixels below the cursors start position (would be better to use size of the cursor image)
		} else {
			y -= tooltipHeight + 5; // five pixels above the cursor is the end of the tooltip box
		}
		setPosition(x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width, y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height);
	}

} /* namespace modules */
} /* namespace i6e */
