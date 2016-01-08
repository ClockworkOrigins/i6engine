#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class SphereColliderPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SphereColliderPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~SphereColliderPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDERPROPERTYWINDOW_H__ */
