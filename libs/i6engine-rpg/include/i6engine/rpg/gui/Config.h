#ifndef __I6ENGINE_RPG_GUI_CONFIG_H__
#define __I6ENGINE_RPG_GUI_CONFIG_H__

#include "i6engine/api/configs/GUIConfig.h"

namespace i6engine {
namespace rpg {
namespace gui {

	enum GUIMessageTypes {
		SetListboxStyle = api::gui::GuiMessageTypesCount
	};

	/**
	 * \brief set listbox style for SubtitleWidget
	 */
	typedef struct GUI_SetListboxStyle : api::gui::GUIUpdateMessageStruct {
		std::string style;
		GUI_SetListboxStyle(const std::string & name, const std::string & s) : api::gui::GUIUpdateMessageStruct(name), style(s) {
		}
		GUI_SetListboxStyle * copy() {
			return new GUI_SetListboxStyle(*this);
		}
	} GUI_SetListboxStyle;

} /* namespace gui */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_GUI_CONFIG_H__ */
