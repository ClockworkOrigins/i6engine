#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class GravityAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		GravityAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~GravityAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYAFFECTORPROPERTYWINDOW_H__ */
