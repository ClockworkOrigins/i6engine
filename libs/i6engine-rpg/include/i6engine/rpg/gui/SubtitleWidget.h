#ifndef __I6ENGINE_RPG_GUI_SUBTITLEWIDGET_H__
#define __I6ENGINE_RPG_GUI_SUBTITLEWIDGET_H__

#include <vector>

#include "i6engine/api/gui/GUIWidget.h"

namespace i6engine {
namespace rpg {
namespace gui {

	class SubtitleWidget : public api::GUIWidget {
	public:
		/**
		 * \brief type has to be the style for the image, e.g. RPG/StaticImage
		 */
		SubtitleWidget(const std::string & name, const std::string & type);

		~SubtitleWidget();

		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) override;

	private:
		std::string _text;
		std::string _font;
		std::vector<CEGUI::Window *> _lines;
		std::string _listboxStyle;

		void setText(const std::string & text);
	};

} /* namespace gui */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_GUI_SUBTITLEWIDGET_H__ */
