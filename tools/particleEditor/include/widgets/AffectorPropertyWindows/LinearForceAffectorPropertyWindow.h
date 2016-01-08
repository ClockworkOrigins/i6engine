#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_LINEARFORCEAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_LINEARFORCEAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class LinearForceAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		LinearForceAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~LinearForceAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_LINEARFORCEAFFECTORPROPERTYWINDOW_H__ */
