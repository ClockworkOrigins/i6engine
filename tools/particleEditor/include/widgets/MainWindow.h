#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetEdit;
	class WidgetParticleList;
	class WidgetRender;
	class WidgetScript;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	signals:
		void triggerPlay();
		void triggerPause();
		void triggerStop();
		void triggerCreateNewSystem(const QString & particle);

	private slots:
		void closeEditor();
		void handlePlayAction();
		void handlePauseAction();
		void handleStopAction();
		void createNewSystem(const QString & particle);
		void tabChanged(int index);

	private:
		enum class CurrentTab {
			Render,
			Edit,
			Script
		};
		QWidget * _renderWrapper;
		WidgetRender * _renderWidget;
		WidgetParticleList * _particleListWidget;
		WidgetEdit * _editWidget;
		WidgetScript * _scriptWidget;
		QTabWidget * _tabWidget;
		bool _playing;
		std::map<QString, QAction *> _toolbarActions;
		QString _currentParticleTemplate;
		CurrentTab _currentTab;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_MAINWINDOW_H__ */
