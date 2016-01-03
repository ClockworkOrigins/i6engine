#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetParticleList;
	class WidgetRender;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	private slots:
		void closeEditor();

	private:
		WidgetRender * _renderWidget;
		WidgetParticleList * _particleListWidget;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__ */
