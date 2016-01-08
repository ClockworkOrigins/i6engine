#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCOUNTOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCOUNTOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class OnCountObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnCountObserverPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnCountObserverPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCOUNTOBSERVERPROPERTYWINDOW_H__ */
