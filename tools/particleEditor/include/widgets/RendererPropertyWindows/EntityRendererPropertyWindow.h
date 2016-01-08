#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ENTITYRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ENTITYRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class EntityRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EntityRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~EntityRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ENTITYRENDERERPROPERTYWINDOW_H__ */
