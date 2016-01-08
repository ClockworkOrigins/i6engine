#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_OBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_OBSERVERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class ObserverPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ObserverPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~ObserverPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_OBSERVERPROPERTYWINDOW_H__ */
