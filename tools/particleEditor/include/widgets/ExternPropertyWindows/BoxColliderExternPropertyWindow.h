#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDEREXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDEREXTERNPROPERTYWINDOW_H__

#include "widgets/ExternPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class BoxColliderExternPropertyWindow : public ExternPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BoxColliderExternPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~BoxColliderExternPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDEREXTERNPROPERTYWINDOW_H__ */
