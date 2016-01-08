#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class RibbonTrailRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RibbonTrailRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~RibbonTrailRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__ */
