#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONPOSITIONOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONPOSITIONOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class OnPositionObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnPositionObserverPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnPositionObserverPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONPOSITIONOBSERVERPROPERTYWINDOW_H__ */
