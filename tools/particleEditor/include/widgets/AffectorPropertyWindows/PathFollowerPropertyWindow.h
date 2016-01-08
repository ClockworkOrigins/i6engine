#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_PATHFOLLOWERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_PATHFOLLOWERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class PathFollowerPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PathFollowerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~PathFollowerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_PATHFOLLOWERPROPERTYWINDOW_H__ */
