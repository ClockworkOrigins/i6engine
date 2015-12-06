#ifndef __I6ENGINE_EDITOR_WIDGETS_OBJECTLISTWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_OBJECTLISTWIDGET_H__

#include "ui_widgetObjectList.h"

#include <cstdint>

namespace i6engine {
namespace editor {
namespace widgets {

	class MainWindow;

	class ObjectListWidget : public QWidget, public Ui::objectListWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ObjectListWidget(QWidget * par, QWidget * mainWindow);

		/**
		 * \brief destructor
		 */
		~ObjectListWidget();

	signals:
		void updateObjectList();
		void selectObject(int64_t id);

	private slots:
		void doUpdateObjectList();
		void selectIndex(QModelIndex index);

	private:
		MainWindow * _mainWindow;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_WIDGETS_OBJECTLISTWIDGET_H__ */
