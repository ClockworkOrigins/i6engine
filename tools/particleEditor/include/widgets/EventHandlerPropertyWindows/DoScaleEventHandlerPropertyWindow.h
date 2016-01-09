#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOSCALEEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOSCALEEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_SCALE_FRACTION = "";
	static QString PRNL_SCALE_TYPE = "";
	static QString SC_TIME_TO_LIVE = "";
	static QString SC_VELOCITY = "";

	class DoScaleEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoScaleEventHandlerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoScaleEventHandlerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOSCALEEVENTHANDLERPROPERTYWINDOW_H__ */
