#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYEXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYEXTERNPROPERTYWINDOW_H__

#include "widgets/ExternPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class GravityExternPropertyWindow : public ExternPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		GravityExternPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~GravityExternPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYEXTERNPROPERTYWINDOW_H__ */
