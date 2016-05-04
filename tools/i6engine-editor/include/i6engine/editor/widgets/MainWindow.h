#ifndef __I6ENGINE_EDITOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_EDITOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

#include <thread>

#include "i6engine/api/facades/InputFacade.h"

#include "i6engine/editor/Editor.h"

class QProgressDialog;

namespace i6e {
namespace editor {
namespace plugins {

	class FlagPluginInterface;
	class InitializationPluginInterface;
	class LoadLevelPluginInterface;
	class RunGamePluginInterface;
	class SaveObjectPluginInterface;

} /* namespace plugins */
namespace widgets {

	class ObjectContainerWidget;
	class RenderWidget;
	class TemplateListWidget;

	class GameActionHelper : public QObject {
		Q_OBJECT

	public:
		GameActionHelper(QWidget * par, size_t index);

	signals:
		void triggerGameAction(int);

	public slots:
		void triggered();

	private:
		size_t _index;
	};

	class MainWindow : public QMainWindow, public Ui::wndMainWindow, public Editor {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

		void updateObjectList() override;

	signals:
		void doChangedLevel();
		void initializeEngine();
		void initializeGame();
		void stopApp();
		void triggerProgressValue(int);
		void triggerProgressMaximum(int);
		void triggerFinishProgress();

	private slots:
		void createNewLevel();
		void chooseLoadLevel();
		void chooseSaveLevel();
		void chooseSaveLevelAs();
		void closeEditor();
		void changedLevel();
		void selectedObject(int64_t id);
		void triggeredGameAction(int index);
		void openOptions();
		void doInitializeEngine();
		void doInitializeGame();
		void doStopApp();
		void finishProgress();

	private:
		const QString WINDOWTITLE;
		RenderWidget * _renderWidget;
		ObjectContainerWidget * _objectContainerWidget;
		TemplateListWidget * _templateListWidget;
		std::thread _engineThread;
		QString _level;
		std::vector<plugins::InitializationPluginInterface *> _initializationPlugins;
		bool _changed;
		std::set<api::KeyCode> _keyStates;
		std::vector<plugins::RunGamePluginInterface *> _runGamePlugins;
		std::vector<plugins::FlagPluginInterface *> _flagPlugins;
		std::vector<GameActionHelper *> _gameActionHelperList;
		std::vector<plugins::SaveObjectPluginInterface *> _saveObjectPlugins;
		std::vector<plugins::LoadLevelPluginInterface *> _loadLevelPlugins;
		bool _resetEngineController;
		int _startGame;
		bool _inGame;
		QProgressDialog * _progressDialog;
		bool _isTmpLevel;
		QString _originalLevel;
		bool _isNewLevel;

		std::string getBasePath() const override {
			return "../media/maps";
		}

		std::vector<std::string> getLevelFlags() const override;

		void AfterInitialize() override;
		void Finalize() override;
		void selectObject(int64_t id) override;
		void removeObject() override;
		void triggerChangedLevel() override;
		void closeEvent(QCloseEvent * evt) override;
		void keyPressEvent(QKeyEvent * evt) override;
		void keyReleaseEvent(QKeyEvent * evt) override;
		void mousePressEvent(QMouseEvent * evt) override;
		void mouseReleaseEvent(QMouseEvent * evt) override;

		void loadPlugins();
		void loadInitializationPlugins();
		void loadRunGamePlugins();
		void loadFlagPlugins();
		void loadSaveObjectPlugins();
		void loadLoadLevelPlugins();
		void saveLevel(const QString & level);

		void setProgressValue(int value) override;
		void setProgressMaximum(int value) override;
		void finishedProgress() override;
		void startLevelWithPlugin(const std::string & level) override;
		bool saveObjectWithPlugin(const api::GOPtr & go, tinyxml2::XMLElement * element) override;
		void finishLevelWithPlugin() override;
		void startLoadLevel(const std::string & level) override;
		void finishLoadLevel(const std::string & level) override;

		static api::KeyCode convertQtToEngine(int key);
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */

#endif /* __I6ENGINE_EDITOR_WIDGETS_MAINWINDOW_H__ */
