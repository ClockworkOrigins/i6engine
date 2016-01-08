#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class RendererPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~RendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__ */
