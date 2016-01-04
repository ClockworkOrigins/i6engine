#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__

#include "ui_widgetEdit.h"

class QGraphicsScene;
class QGraphicsView;

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetEdit : public QWidget, public Ui::editWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetEdit(QWidget * par, QWidget * renderWidget);

		/**
		 * \brief destructor
		 */
		~WidgetEdit();

	private:
		QGraphicsScene * _graphicsScene;
		QGraphicsView * _graphicsView;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__ */
