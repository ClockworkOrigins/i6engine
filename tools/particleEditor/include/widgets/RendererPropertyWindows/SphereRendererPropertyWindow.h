#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERERENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERERENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class SphereRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SphereRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~SphereRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERERENDERERPROPERTYWINDOW_H__ */
