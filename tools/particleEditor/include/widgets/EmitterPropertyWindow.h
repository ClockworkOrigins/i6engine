#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class EmitterPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~EmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__ */
