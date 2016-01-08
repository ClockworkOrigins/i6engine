#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class BoxEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BoxEmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~BoxEmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXEMITTERPROPERTYWINDOW_H__ */
