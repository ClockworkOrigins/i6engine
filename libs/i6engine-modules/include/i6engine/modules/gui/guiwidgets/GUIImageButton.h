#ifndef __I6ENGINE_MODULES_GUIIMAGEBUTTON_H__
#define __I6ENGINE_MODULES_GUIIMAGEBUTTON_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class EventArgs;
} /* namespace CEGUI */

namespace i6engine {
namespace modules {

	class GUIImageButton : public api::GUIWidget {
	public:
		GUIImageButton(const std::string & name, const std::string & type);

		~GUIImageButton();

		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) override;
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUIIMAGEBUTTON_H__ */