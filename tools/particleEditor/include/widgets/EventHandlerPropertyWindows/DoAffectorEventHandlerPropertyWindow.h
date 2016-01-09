#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOAFFECTOREVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOAFFECTOREVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_PRE_POST = "";

	class DoAffectorEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoAffectorEventHandlerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoAffectorEventHandlerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOAFFECTOREVENTHANDLERPROPERTYWINDOW_H__ */
