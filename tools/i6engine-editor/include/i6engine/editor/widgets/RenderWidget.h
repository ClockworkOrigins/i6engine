#ifndef __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__

#include "ui_widgetRender.h"

namespace i6engine {
namespace editor {
namespace widgets {

	class RenderWidget : public QWidget, public Ui::renderWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RenderWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~RenderWidget();
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__ */
