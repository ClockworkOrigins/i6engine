#include "i6engine/modules/gui/guiwidgets/GUIImageButton.h"

#include "i6engine/api/configs/GUIConfig.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace modules {

	GUIImageButton::GUIImageButton(const std::string & name, const std::string & style) : GUIWidget(name) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		_window = wmgr.createWindow(style, name);
		_window->setUserData(this);
		_window->setRiseOnClickEnabled(false);

		_window->setUsingAutoRenderingSurface(true);

		CEGUI::RenderingSurface* rs = _window->getRenderingSurface();
		if (rs) {
			rs->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueEnded, CEGUI::Event::Subscriber(&api::GUIWidget::renderingEndedHandler, dynamic_cast<api::GUIWidget *>(this)));
		}
	}

	GUIImageButton::~GUIImageButton() {
	}

	void GUIImageButton::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetProperty) {
			api::gui::GUI_SetProperty * g = dynamic_cast<api::gui::GUI_SetProperty *>(data);
			_window->setProperty(g->prop, g->value);
		} else {
			GUIWidget::update(type, data);
		}
	}

} /* namespace modules */
} /* namespace i6e */
