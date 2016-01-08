#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCLEAROBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCLEAROBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class OnClearObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnClearObserverPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnClearObserverPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCLEAROBSERVERPROPERTYWINDOW_H__ */
