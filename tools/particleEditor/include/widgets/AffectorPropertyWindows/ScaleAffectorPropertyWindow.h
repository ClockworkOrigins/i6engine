#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_XYZ_SCALE = "";
	static QString PRNL_X_SCALE = "";
	static QString PRNL_Y_SCALE = "";
	static QString PRNL_Z_SCALE = "";

	class ScaleAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ScaleAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~ScaleAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEAFFECTORPROPERTYWINDOW_H__ */
