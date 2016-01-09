#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEVELOCITYAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEVELOCITYAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_VELOCITY_SCALE = "";
	static QString PRNL_STOP_AT_FLIP = "";

	class ScaleVelocityAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ScaleVelocityAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~ScaleVelocityAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEVELOCITYAFFECTORPROPERTYWINDOW_H__ */
