#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_VORTEXEXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_VORTEXEXTERNPROPERTYWINDOW_H__

#include "widgets/ExternPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class VortexExternPropertyWindow : public ExternPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		VortexExternPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~VortexExternPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_VORTEXEXTERNPROPERTYWINDOW_H__ */
