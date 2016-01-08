#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_VORTEXAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_VORTEXAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class VortexAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		VortexAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~VortexAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_VORTEXAFFECTORPROPERTYWINDOW_H__ */
