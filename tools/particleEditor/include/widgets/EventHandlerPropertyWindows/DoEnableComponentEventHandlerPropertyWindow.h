#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOENABLECOMPONENTEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOENABLECOMPONENTEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class DoEnableComponentEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoEnableComponentEventHandlerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoEnableComponentEventHandlerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOENABLECOMPONENTEVENTHANDLERPROPERTYWINDOW_H__ */
