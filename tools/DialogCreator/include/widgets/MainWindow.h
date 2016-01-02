#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	class DialogListWidget;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	private slots:
		void closeEditor();

	private:
		DialogListWidget * _dialogListWidget;

		void closeEvent(QCloseEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_MAINWINDOW_H__ */
