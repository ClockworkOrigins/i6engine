#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLLISIONAVOIDANCEAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLLISIONAVOIDANCEAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class CollisionAvoidanceAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		CollisionAvoidanceAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~CollisionAvoidanceAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLLISIONAVOIDANCEAFFECTORPROPERTYWINDOW_H__ */
