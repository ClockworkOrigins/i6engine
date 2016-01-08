#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BILLBOARDRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BILLBOARDRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class BillboardRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BillboardRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~BillboardRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BILLBOARDRENDERERPROPERTYWINDOW_H__ */
