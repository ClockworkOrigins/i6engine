#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCRENDERWIDGET_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCRENDERWIDGET_H__

#include "ui_widgetNPCRender.h"

namespace i6engine {
namespace tools {
namespace npcCreator {
namespace widgets {

	class NPCEditWidget;

	class NPCRenderWidget : public QWidget, public Ui::npcRenderWidget {
		Q_OBJECT

	public:
		NPCRenderWidget(QWidget * par = nullptr);
		~NPCRenderWidget();
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6engine */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCRENDERWIDGET_H__ */
