#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SYSTEMPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SYSTEMPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class SystemPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SystemPropertyWindow(QWidget * par, QString name);

		/**
		 * \brief destructor
		 */
		~SystemPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SYSTEMPROPERTYWINDOW_H__ */
