#ifndef __I6ENGINE_EDITOR_WIDGETS_OBJECTCONTAINERWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_OBJECTCONTAINERWIDGET_H__

#include "ui_widgetObjectContainer.h"

#include <cstdint>

namespace i6engine {
namespace editor {
namespace widgets {

	class ObjectInfoWidget;
	class ObjectListWidget;

	class ObjectContainerWidget : public QWidget, public Ui::objectContainerWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ObjectContainerWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~ObjectContainerWidget();

		ObjectListWidget * objectListWidget;
		ObjectInfoWidget * objectInfoWidget;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_WIDGETS_OBJECTCONTAINERWIDGET_H__ */
