#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_LIGHTRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_LIGHTRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class LightRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		LightRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~LightRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_LIGHTRENDERERPROPERTYWINDOW_H__ */
