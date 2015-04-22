#ifndef __I6ENGINE_EDITOR_GUI_MESSAGEBOXWIDGET_H__
#define __I6ENGINE_EDITOR_GUI_MESSAGEBOXWIDGET_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace i6engine {
namespace editor {
namespace gui {

	class MessageBoxWidget : public i6engine::api::GUIWidget {
	public:
		MessageBoxWidget(const std::string & name, const std::string & type);

		~MessageBoxWidget();

		void update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) override;

	private:
		void setText(const std::string & text);
	};

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_GUI_MESSAGEBOXWIDGET_H__ */
