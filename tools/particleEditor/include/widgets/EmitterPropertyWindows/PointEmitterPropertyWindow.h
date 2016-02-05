#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_POINTEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_POINTEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class PointEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PointEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~PointEmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_POINTEMITTERPROPERTYWINDOW_H__ */
