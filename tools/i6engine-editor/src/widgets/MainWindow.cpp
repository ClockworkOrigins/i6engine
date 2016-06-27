/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/editor/widgets/MainWindow.h"

#include <thread>

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/audio/AudioController.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#include "i6engine/editor/plugins/FlagPluginInterface.h"
#include "i6engine/editor/plugins/InitializationPluginInterface.h"
#include "i6engine/editor/plugins/LoadLevelPluginInterface.h"
#include "i6engine/editor/plugins/ObjectTypePluginInterface.h"
#include "i6engine/editor/plugins/RunGamePluginInterface.h"
#include "i6engine/editor/plugins/SaveObjectPluginInterface.h"

#include "i6engine/editor/widgets/ConfigDialog.h"
#include "i6engine/editor/widgets/ObjectContainerWidget.h"
#include "i6engine/editor/widgets/ObjectInfoWidget.h"
#include "i6engine/editor/widgets/ObjectListWidget.h"
#include "i6engine/editor/widgets/RenderWidget.h"
#include "i6engine/editor/widgets/TemplateListWidget.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPluginLoader>
#include <QProgressDialog>
#include <QTimer>

namespace i6e {
namespace editor {
namespace widgets {

	GameActionHelper::GameActionHelper(QWidget * par, size_t index) : QObject(par), _index(index) {
	}

	void GameActionHelper::triggered() {
		emit triggerGameAction(int(_index));
	}

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), Editor(), WINDOWTITLE(QString("i6engine-editor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")")), _renderWidget(new RenderWidget(this)), _objectContainerWidget(new ObjectContainerWidget(this)), _templateListWidget(new TemplateListWidget(this)), _engineThread(), _level(), _initializationPlugins(), _changed(false), _keyStates(), _runGamePlugins(), _flagPlugins(), _gameActionHelperList(), _saveObjectPlugins(), _loadLevelPlugins(), _objectTypePlugins(), _resetEngineController(false), _startGame(-1), _inGame(false), _progressDialog(nullptr), _isTmpLevel(false), _originalLevel(), _isNewLevel(false) {
		setupUi(this);

		setWindowIcon(QIcon(":/icon.png"));

		qRegisterMetaType<int64_t>("int64_t");

		showMaximized();

		setWindowTitle(WINDOWTITLE);

		loadPlugins();

		for (plugins::ObjectTypePluginInterface * otpi : _objectTypePlugins) {
			plugins::ObjectTypeWidgetInterface * objectTypeWidget = otpi->createWidget(_templateListWidget);
			_templateListWidget->verticalLayout->addWidget(objectTypeWidget);
			objectTypeWidget->setUpdateCallback(std::bind(&MainWindow::doObjectTypeUpdate, this));
		}

		gridLayout->addWidget(_templateListWidget, 0, 0);
		gridLayout->addWidget(_renderWidget, 0, 1);
		gridLayout->addWidget(_objectContainerWidget, 0, 2);

		gridLayout->setColumnStretch(0, 1);
		gridLayout->setColumnStretch(1, 9);
		gridLayout->setColumnStretch(2, 2);

		connect(this, SIGNAL(initializeEngine()), this, SLOT(doInitializeEngine()));
		connect(this, SIGNAL(initializeGame()), this, SLOT(doInitializeGame()));
		connect(this, SIGNAL(stopApp()), this, SLOT(doStopApp()));
		connect(this, SIGNAL(triggerFinishProgress()), this, SLOT(finishProgress()));

		connect(_templateListWidget, SIGNAL(changedLevel()), this, SLOT(changedLevel()));
		connect(_objectContainerWidget->objectInfoWidget, SIGNAL(changedLevel()), this, SLOT(changedLevel()));
		connect(_templateListWidget, SIGNAL(updateObjectList()), _objectContainerWidget->objectListWidget, SLOT(doUpdateObjectList()));
		connect(this, SIGNAL(doChangedLevel()), this, SLOT(changedLevel()));
		connect(_objectContainerWidget->objectInfoWidget, SIGNAL(selectedObject(int64_t)), this, SLOT(selectedObject(int64_t)));

		emit initializeEngine();

		menuFile->setTitle(QApplication::tr("File"));
		actionNew_Level->setText(QApplication::tr("New Level"));
		actionLoad_Level->setText(QApplication::tr("Load Level"));
		actionSave_Level->setText(QApplication::tr("Save Level"));
		actionSave_Level_As->setText(QApplication::tr("Save Level As"));
		actionExit->setText(QApplication::tr("Exit"));
		menuGame->setTitle(QApplication::tr("Game"));
		menuExtras->setTitle(QApplication::tr("Extras"));
		actionOptions->setText(QApplication::tr("Options"));

		actionNew_Level->setIcon(QIcon(":/images/new.png"));
		actionLoad_Level->setIcon(QIcon(":/images/load.png"));
		actionSave_Level->setIcon(QIcon(":/images/save.png"));
		actionExit->setIcon(QIcon(":/images/close.png"));
		actionOptions->setIcon(QIcon(":/images/config_general.png"));
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::createNewLevel() {
		if (_changed) {
			if (QMessageBox::StandardButton::Yes == QMessageBox::warning(this, QApplication::tr("Unsaved changes"), QApplication::tr("There are unsaved changes in your level. Do you want to save the level?"), QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No)) {
				chooseSaveLevelAs();
			}
		}
		QString file = QFileDialog::getSaveFileName(nullptr, QApplication::tr("New file name ..."), QString::fromStdString(getBasePath()), QApplication::tr("Level Files (*.xml)"));
		if (!file.isEmpty()) {
			clearLevel();
			_isNewLevel = true;
			_level = file;
			saveLevel(file);
			setWindowTitle(WINDOWTITLE + " - " + _level);
			_changed = false;
		}
	}

	void MainWindow::chooseLoadLevel() {
		if (_changed) {
			if (QMessageBox::StandardButton::Yes == QMessageBox::warning(this, QApplication::tr("Unsaved changes"), QApplication::tr("There are unsaved changes in your level. Do you want to save the level?"), QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No)) {
				chooseSaveLevelAs();
			}
		}
		QString file = QFileDialog::getOpenFileName(nullptr, QApplication::tr("Open file ..."), QString::fromStdString(getBasePath()), QApplication::tr("Level Files (*.xml)"));
		if (!file.isEmpty()) {
			delete _progressDialog;
			_progressDialog = new QProgressDialog(QApplication::tr("Loading level..."), "", 0, 100, this);
			_progressDialog->setWindowModality(Qt::WindowModal);
			_progressDialog->setCancelButton(nullptr);
			connect(this, SIGNAL(triggerProgressValue(int)), _progressDialog, SLOT(setValue(int)));
			loadLevel(file.toStdString(), [this](uint16_t value) {
				setProgressValue(int(value));
			});
			_level = file;
			setWindowTitle(WINDOWTITLE + " - " + _level);
		}
	}

	void MainWindow::chooseSaveLevel() {
		if (!_level.isEmpty()) {
			if (_changed) {
				saveLevel(_level);
				setWindowTitle(WINDOWTITLE + " - " + _level);
				_changed = false;
			}
		}
	}

	void MainWindow::chooseSaveLevelAs() {
		QString file = QFileDialog::getSaveFileName(nullptr, QApplication::tr("Save file ..."), QString::fromStdString(getBasePath()), QApplication::tr("Level Files (*.xml)"));
		if (!file.isEmpty()) {
			saveLevel(file);
			_level = file;
			setWindowTitle(WINDOWTITLE + " - " + _level);
			_changed = false;
		}
	}

	void MainWindow::closeEditor() {
		if (_changed) {
			if (QMessageBox::StandardButton::Yes == QMessageBox::warning(this, QApplication::tr("Unsaved changes"), QApplication::tr("There are unsaved changes in your level. Do you want to save the level?"), QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No)) {
				chooseSaveLevelAs();
			}
		}
		api::EngineController::GetSingleton().stop();
	}

	void MainWindow::changedLevel() {
		setWindowTitle(WINDOWTITLE + " - " + _level + " *");
		_changed = true;
	}

	void MainWindow::selectedObject(int64_t id) {
		setSelectObject(id);
	}

	void MainWindow::triggeredGameAction(int index) {
		if (_level.size() > 0) {
			_startGame = index;
			_resetEngineController = true;
			if (_changed) {
				_originalLevel = _level;
				_isTmpLevel = true;
				_level = _level.split("/").back().split("\\").back();
				saveLevel(_level);
			} else {
				_isTmpLevel = false;
				api::EngineController::GetSingleton().stop();
			}
		} else {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Can't start game!"));
			box.setInformativeText(QApplication::tr("Actually no level is loaded. Load a level first to start game with it!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}
	}

	void MainWindow::openOptions() {
		ConfigDialog dlg;
		dlg.movementSpeedSlider->setValue(int(_movementSpeed * 20));
		dlg.rotationSpeedSlider->setValue(int(_rotationSpeed * 20));
		if (dlg.exec() == QDialog::Accepted) {
			_movementSpeed = dlg.movementSpeedSlider->value() / 20.0;
			_rotationSpeed = dlg.rotationSpeedSlider->value() / 20.0;
		}
	}

	std::vector<std::string> MainWindow::getLevelFlags() const {
		std::vector<std::string> vec = { "Singleplayer" };
		for (plugins::FlagPluginInterface * fpi : _flagPlugins) {
			for (std::string s : fpi->getFlags()) {
				vec.push_back(s);
			}
		}
		return vec;
	}

	void MainWindow::AfterInitialize() {
		Editor::AfterInitialize();

		for (plugins::InitializationPluginInterface * ipi : _initializationPlugins) {
			ipi->initialize();
		}

		api::EngineController::GetSingleton().getObjectFacade()->allowComponentsTicking(false);

		api::EngineController::GetSingleton().getGUIFacade()->setMouseVisibility(false);

		emit _templateListWidget->loadTemplates();

		if (_inGame) {
			if (_isTmpLevel) {
				_changed = true;
			}
			delete _progressDialog;
			_progressDialog = new QProgressDialog(QApplication::tr("Loading level..."), "", 0, 100, this);
			_progressDialog->setWindowModality(Qt::WindowModal);
			_progressDialog->setCancelButton(nullptr);
			connect(this, SIGNAL(triggerProgressValue(int)), _progressDialog, SLOT(setValue(int)));
			loadLevel(_level.toStdString(), [this](uint16_t value) {
				setProgressValue(int(value));
			});
			if (_changed) {
				QFile f(_level);
				f.remove();
				_level = _originalLevel;
			}
			_inGame = false;
			if (_changed) {
				setWindowTitle(WINDOWTITLE + " - " + _level + " *");
			} else {
				setWindowTitle(WINDOWTITLE + " - " + _level);
			}
		}
	}

	void MainWindow::Finalize() {
		if (!_inGame) {
			Editor::Finalize();
		}
		if (_resetEngineController || _inGame) {
			if (_startGame > -1) {
				emit initializeGame();
				_inGame = true;
			} else {
				emit initializeEngine();
			}
			_resetEngineController = false;
		} else {
			emit stopApp();
		}
	}

	void MainWindow::updateObjectList() {
		emit _objectContainerWidget->objectListWidget->updateObjectList();
	}

	void MainWindow::selectObject(int64_t id) {
		emit _objectContainerWidget->objectInfoWidget->selectObject(id);
	}

	void MainWindow::removeObject() {
		emit _objectContainerWidget->objectInfoWidget->removeObject();
	}

	void MainWindow::triggerChangedLevel() {
		emit doChangedLevel();
	}

	void MainWindow::closeEvent(QCloseEvent * evt) {
		closeEditor();
		evt->ignore();
	}

	void MainWindow::keyPressEvent(QKeyEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
			api::KeyCode kc = convertQtToEngine(evt->key());
			if (convertQtToEngine(evt->key()) == api::KeyCode::KC_ESCAPE) {
				if (_inGame && !_resetEngineController) {
					_resetEngineController = true;
					api::EngineController::GetSingletonPtr()->stop();
				}
			} else {
				api::KeyState ks = api::KeyState::KEY_PRESSED;
				if (_keyStates.find(kc) != _keyStates.end()) {
					ks = api::KeyState::KEY_HOLD;
				} else {
					_keyStates.insert(kc);
				}
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(ks, kc, evt->text().toUInt()), core::Subsystem::Input));
			}
			evt->accept();
		}
		evt->ignore();
	}

	void MainWindow::keyReleaseEvent(QKeyEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
			api::KeyCode kc = convertQtToEngine(evt->key());
			_keyStates.erase(kc);
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_RELEASED, kc, evt->text().toUInt()), core::Subsystem::Input));
			evt->accept();
		}
		evt->ignore();
	}

	void MainWindow::mousePressEvent(QMouseEvent * evt) {
		Qt::MouseButton button = evt->button();
		api::KeyCode kc = api::KeyCode::MOUSEBUTTONS;
		if (button == Qt::MouseButton::LeftButton) {
			kc = api::KeyCode::KC_MBLeft;
		} else if (button == Qt::MouseButton::MiddleButton) {
			kc = api::KeyCode::KC_MBMiddle;
		} else if (button == Qt::MouseButton::RightButton) {
			kc = api::KeyCode::KC_MBRight;
		}
		api::KeyState ks = api::KeyState::KEY_PRESSED;
		if (_keyStates.find(kc) != _keyStates.end()) {
			ks = api::KeyState::KEY_HOLD;
		} else {
			_keyStates.insert(kc);
		}
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(ks, kc, 0), core::Subsystem::Input));
		evt->accept();
	}

	void MainWindow::mouseReleaseEvent(QMouseEvent * evt) {
		Qt::MouseButton button = evt->button();
		api::KeyCode kc = api::KeyCode::MOUSEBUTTONS;
		if (button == Qt::MouseButton::LeftButton) {
			kc = api::KeyCode::KC_MBLeft;
		} else if (button == Qt::MouseButton::MiddleButton) {
			kc = api::KeyCode::KC_MBMiddle;
		} else if (button == Qt::MouseButton::RightButton) {
			kc = api::KeyCode::KC_MBRight;
		}
		_keyStates.erase(kc);
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_RELEASED, kc, 0), core::Subsystem::Input));
		evt->accept();
	}

	void MainWindow::loadPlugins() {
		loadFlagPlugins();
		loadInitializationPlugins();
		loadLoadLevelPlugins();
		loadObjectTypePlugins();
		loadRunGamePlugins();
		loadSaveObjectPlugins();
	}

	void MainWindow::loadInitializationPlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/initialization");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_initializationPlugins.push_back(qobject_cast<plugins::InitializationPluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

	void MainWindow::loadRunGamePlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/runGame");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_runGamePlugins.push_back(qobject_cast<plugins::RunGamePluginInterface *>(plugin));
				QAction * action = menuGame->addAction(_runGamePlugins.back()->getMenuEntry());
				GameActionHelper * gac = new GameActionHelper(this, _runGamePlugins.size() - 1);
				_gameActionHelperList.push_back(gac);
				connect(action, SIGNAL(triggered()), gac, SLOT(triggered()));
				connect(gac, SIGNAL(triggerGameAction(int)), this, SLOT(triggeredGameAction(int)));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

	void MainWindow::loadFlagPlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/flags");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_flagPlugins.push_back(qobject_cast<plugins::FlagPluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

	void MainWindow::loadSaveObjectPlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/saveObject");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_saveObjectPlugins.push_back(qobject_cast<plugins::SaveObjectPluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

	void MainWindow::loadLoadLevelPlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/loadLevel");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_loadLevelPlugins.push_back(qobject_cast<plugins::LoadLevelPluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

	void MainWindow::loadObjectTypePlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/objectTypes");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_objectTypePlugins.push_back(qobject_cast<plugins::ObjectTypePluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

	void MainWindow::saveLevel(const QString & level) {
		delete _progressDialog;
		_progressDialog = new QProgressDialog(QApplication::tr("Saving level..."), "", 0, 1, this);
		_progressDialog->setWindowModality(Qt::WindowModal);
		_progressDialog->setCancelButton(nullptr);
		connect(this, SIGNAL(triggerProgressValue(int)), _progressDialog, SLOT(setValue(int)));
		connect(this, SIGNAL(triggerProgressMaximum(int)), _progressDialog, SLOT(setMaximum(int)));

		std::function<void(void)> callback = std::bind(&Editor::saveLevel, this, level.toStdString());
		i6eEngineController->registerTimer(0, [callback]() {
			callback();
			return false;
		}, false, core::JobPriorities::Prio_Medium);
	}

	void MainWindow::setProgressValue(int value) {
		emit triggerProgressValue(value);
	}

	void MainWindow::setProgressMaximum(int value) {
		emit triggerProgressMaximum(value);
	}

	void MainWindow::finishedProgress() {
		emit triggerFinishProgress();
	}

	api::KeyCode MainWindow::convertQtToEngine(int key) {
		api::KeyCode kc = api::KeyCode::KC_ESCAPE;
		switch (key) {
		case Qt::Key::Key_W: {
			kc = api::KeyCode::KC_W;
			break;
		}
		case Qt::Key::Key_S: {
			kc = api::KeyCode::KC_S;
			break;
		}
		case Qt::Key::Key_A: {
			kc = api::KeyCode::KC_A;
			break;
		}
		case Qt::Key::Key_D: {
			kc = api::KeyCode::KC_D;
			break;
		}
		case Qt::Key::Key_Control: {
			kc = api::KeyCode::KC_LCONTROL;
			break;
		}
		case Qt::Key::Key_Space: {
			kc = api::KeyCode::KC_SPACE;
			break;
		}
		case Qt::Key::Key_Delete: {
			kc = api::KeyCode::KC_DELETE;
			break;
		}
		case Qt::Key::Key_Insert: {
			kc = api::KeyCode::KC_INSERT;
			break;
		}
		case Qt::Key::Key_Home: {
			kc = api::KeyCode::KC_HOME;
			break;
		}
		case Qt::Key::Key_End: {
			kc = api::KeyCode::KC_END;
			break;
		}
		case Qt::Key::Key_PageUp: {
			kc = api::KeyCode::KC_PGUP;
			break;
		}
		case Qt::Key::Key_PageDown: {
			kc = api::KeyCode::KC_PGDOWN;
			break;
		}
		case Qt::Key::Key_Plus: {
			kc = api::KeyCode::KC_ADD;
			break;
		}
		case Qt::Key::Key_Minus: {
			kc = api::KeyCode::KC_SUBTRACT;
			break;
		}
		case Qt::Key::Key_1: {
			kc = api::KeyCode::KC_NUMPAD1;
			break;
		}
		case Qt::Key::Key_2: {
			kc = api::KeyCode::KC_NUMPAD2;
			break;
		}
		case Qt::Key::Key_4: {
			kc = api::KeyCode::KC_NUMPAD4;
			break;
		}
		case Qt::Key::Key_5: {
			kc = api::KeyCode::KC_NUMPAD5;
			break;
		}
		case Qt::Key::Key_7: {
			kc = api::KeyCode::KC_NUMPAD7;
			break;
		}
		case Qt::Key::Key_8: {
			kc = api::KeyCode::KC_NUMPAD8;
			break;
		}
		case Qt::Key::Key_F3: {
			kc = api::KeyCode::KC_F3;
			break;
		}
		case Qt::Key::Key_M: {
			kc = api::KeyCode::KC_M;
			break;
		}
		case Qt::Key::Key_F4: {
			kc = api::KeyCode::KC_F4;
			break;
		}
		case Qt::Key::Key_Escape: {
			kc = api::KeyCode::KC_ESCAPE;
			break;
		}
		default: {
			kc = api::KeyCode::KC_ESCAPE;
			break;
		}
		}
		return kc;
	}

	void MainWindow::doInitializeEngine() {
		if (_engineThread.joinable()) {
			_engineThread.join();
		}
		api::EngineController::GetSingleton().reset();
		api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new modules::GraphicsController(reinterpret_cast<HWND>(_renderWidget->winId())), { core::Subsystem::Object });
		api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new modules::ObjectController(), LNG_OBJECT_FRAME_TIME);
		api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#ifdef ISIXE_WITH_AUDIO
		api::EngineController::GetSingletonPtr()->registerSubSystem("Audio", new modules::AudioController(), LNG_AUDIO_FRAME_TIME);
#endif
		api::EngineController::GetSingletonPtr()->registerApplication(*this);

		_engineThread = std::thread(&api::EngineController::start, api::EngineController::GetSingletonPtr());
	}

	void MainWindow::doInitializeGame() {
		if (_engineThread.joinable()) {
			_engineThread.join();
		}
		api::EngineController::GetSingleton().reset();
		assert(_startGame >= 0);
		_runGamePlugins[size_t(_startGame)]->initializeSubSystems(reinterpret_cast<HWND>(_renderWidget->winId()));
		api::EngineController::GetSingletonPtr()->registerApplication(*_runGamePlugins[size_t(_startGame)]);
		_runGamePlugins[size_t(_startGame)]->setLevel(_level.toStdString());
		_runGamePlugins[size_t(_startGame)]->setFinalizeCallback(std::bind(&MainWindow::Finalize, this));
		_engineThread = std::thread(&api::EngineController::start, api::EngineController::GetSingletonPtr());
		_startGame = -1;
	}

	void MainWindow::doStopApp() {
		if (_engineThread.joinable()) {
			_engineThread.join();
		}
		qApp->exit();
	}

	void MainWindow::finishProgress() {
		delete _progressDialog;
		_progressDialog = nullptr;

		if (_startGame != -1) {
			api::EngineController::GetSingleton().stop();
		}
		if (_isNewLevel) {
			_progressDialog = new QProgressDialog(QApplication::tr("Creating new level..."), "", 0, 100, this);
			_progressDialog->setWindowModality(Qt::WindowModal);
			_progressDialog->setCancelButton(nullptr);
			connect(this, SIGNAL(triggerProgressValue(int)), _progressDialog, SLOT(setValue(int)));
			loadLevel(_level.toStdString(), [this](uint16_t value) {
				setProgressValue(int(value));
			});
		}
	}

	void MainWindow::startLevelWithPlugin(const std::string & level) {
		for (plugins::SaveObjectPluginInterface * sopi : _saveObjectPlugins) {
			sopi->startLevel(QString::fromStdString(level));
		}
	}

	bool MainWindow::saveObjectWithPlugin(const api::GOPtr & go, tinyxml2::XMLElement * element) {
		bool handled = false;
		for (plugins::SaveObjectPluginInterface * sopi : _saveObjectPlugins) {
			handled = handled || sopi->saveObject(go, element);
		}
		return handled;
	}

	void MainWindow::finishLevelWithPlugin() {
		for (plugins::SaveObjectPluginInterface * sopi : _saveObjectPlugins) {
			sopi->finishLevel();
		}
	}

	void MainWindow::startLoadLevel(const std::string & level) {
		for (plugins::LoadLevelPluginInterface * llpi : _loadLevelPlugins) {
			llpi->startLoadLevel(QString::fromStdString(level));
		}
	}

	void MainWindow::finishLoadLevel(const std::string & level) {
		for (plugins::LoadLevelPluginInterface * llpi : _loadLevelPlugins) {
			llpi->finishLoadLevel(QString::fromStdString(level));
		}
	}

	void MainWindow::doObjectTypeUpdate() {
		setFocus();
		updateObjectList();
		triggerChangedLevel();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */
