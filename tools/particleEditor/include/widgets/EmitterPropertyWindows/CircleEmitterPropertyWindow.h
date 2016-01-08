#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_CIRCLEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_CIRCLEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class CircleEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		CircleEmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~CircleEmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_CIRCLEEMITTERPROPERTYWINDOW_H__ */
