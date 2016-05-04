#ifndef __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__

#include "ui_widgetRender.h"

namespace i6e {
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

	private:
		void mouseMoveEvent(QMouseEvent * evt) override;
		bool eventFilter(QObject * obj, QEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */

#endif /* __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__ */
