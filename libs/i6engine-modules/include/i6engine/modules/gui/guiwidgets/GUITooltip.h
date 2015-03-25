#ifndef __I6ENGINE_MODULES_GUITOOLTIP_H__
#define __I6ENGINE_MODULES_GUITOOLTIP_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace i6engine {
namespace modules {

	class GUITooltip : public i6engine::api::GUIWidget {
	public:
		GUITooltip(const std::string & name, const std::string & type);

		~GUITooltip();

		void update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) override;

		void tick() override;

	private:
		void setText(const std::string & text);
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUITOOLTIP_H__ */
