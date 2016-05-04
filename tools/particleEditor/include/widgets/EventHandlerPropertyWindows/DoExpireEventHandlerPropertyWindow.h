#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOEXPIREEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOEXPIREEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	class DoExpireEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoExpireEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoExpireEventHandlerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOEXPIREEVENTHANDLERPROPERTYWINDOW_H__ */
