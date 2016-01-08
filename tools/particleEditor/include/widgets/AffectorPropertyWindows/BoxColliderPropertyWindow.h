#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class BoxColliderPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BoxColliderPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~BoxColliderPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDERPROPERTYWINDOW_H__ */
