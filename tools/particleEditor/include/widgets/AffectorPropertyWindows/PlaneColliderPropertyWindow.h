#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_PLANECOLLIDERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_PLANECOLLIDERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class PlaneColliderPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PlaneColliderPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~PlaneColliderPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_PLANECOLLIDERPROPERTYWINDOW_H__ */
