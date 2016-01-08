#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_POSITIONEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_POSITIONEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class PositionEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PositionEmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~PositionEmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_POSITIONEMITTERPROPERTYWINDOW_H__ */
