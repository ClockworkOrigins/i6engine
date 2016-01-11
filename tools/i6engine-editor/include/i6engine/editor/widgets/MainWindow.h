#ifndef __I6ENGINE_EDITOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_EDITOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

#include "i6engine/api/facades/InputFacade.h"

#include "i6engine/editor/Editor.h"

namespace i6engine {
namespace editor {
namespace plugins {

	class InitializationPluginInterface;

} /* namespace plugins */
namespace widgets {

	class ObjectContainerWidget;
	class RenderWidget;
	class TemplateListWidget;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow, public Editor {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

		void updateObjectList() override;

	private slots:
		void chooseLoadLevel();
		void chooseSaveLevel();
		void chooseSaveLevelAs();
		void closeEditor();

	private:
		const QString WINDOWTITLE;
		RenderWidget * _renderWidget;
		ObjectContainerWidget * _objectContainerWidget;
		TemplateListWidget * _templateListWidget;
		QString _level;
		std::vector<plugins::InitializationPluginInterface *> _initializationPlugins;

		std::string getBasePath() const override {
			return "../media/maps";
		}

		std::vector<std::string> getLevelFlags() const override {
			return { "Singleplayer" };
		}

		void AfterInitialize() override;
		void Finalize() override;
		void selectObject(int64_t id) override;
		void removeObject() override;
		void closeEvent(QCloseEvent * evt) override;
		void keyPressEvent(QKeyEvent * evt) override;
		void keyReleaseEvent(QKeyEvent * evt) override;
		void mouseMoveEvent(QMouseEvent * evt) override;
		void mousePressEvent(QMouseEvent * evt) override;
		void mouseReleaseEvent(QMouseEvent * evt) override;
		bool eventFilter(QObject * obj, QEvent * evt);

		void loadPlugins();
		void loadInitializationPlugins();

		static api::KeyCode convertQtToEngine(int key);
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_WIDGETS_MAINWINDOW_H__ */
