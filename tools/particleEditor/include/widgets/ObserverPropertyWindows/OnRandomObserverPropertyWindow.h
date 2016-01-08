#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONRANDOMOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONRANDOMOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class OnRandomObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnRandomObserverPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnRandomObserverPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONRANDOMOBSERVERPROPERTYWINDOW_H__ */
