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

#include "i6engine/modules/audio/AudioController.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#include "i6engine/editor/plugins/InitializationPluginInterface.h"
#include "i6engine/editor/plugins/RunGamePluginInterface.h"

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
#include <QTimer>

namespace i6engine {
namespace editor {
namespace widgets {

	GameActionHelper::GameActionHelper(QWidget * par, size_t index) : QObject(par), _index(index) {
	}

	void GameActionHelper::triggered() {
		emit triggerGameAction(_index);
	}

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), Editor(), _renderWidget(new RenderWidget(this)), _objectContainerWidget(new ObjectContainerWidget(this)), _templateListWidget(new TemplateListWidget(this)), _engineThread(), _level(), _initializationPlugins(), _runGamePlugins(), _gameActionHelperList(), _startGame(-1), _inGame(false), _resetEngineController(false) {
		setupUi(this);

		qRegisterMetaType<int64_t>("int64_t");

		showMaximized();

		setWindowTitle(QString("i6engine-editor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		gridLayout->addWidget(_templateListWidget, 0, 0);
		gridLayout->addWidget(_renderWidget, 0, 1);
		gridLayout->addWidget(_objectContainerWidget, 0, 2);

		gridLayout->setColumnStretch(0, 1);
		gridLayout->setColumnStretch(1, 9);
		gridLayout->setColumnStretch(2, 2);

		loadPlugins();

		connect(this, SIGNAL(initializeEngine()), this, SLOT(doInitializeEngine()));
		connect(this, SIGNAL(initializeGame()), this, SLOT(doInitializeGame()));
		connect(this, SIGNAL(stopApp()), this, SLOT(doStopApp()));

		emit initializeEngine();

		setMouseTracking(true);
		installEventFilter(this);
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::chooseLoadLevel() {
		QString file = QFileDialog::getOpenFileName(nullptr, "Open file ...", QString::fromStdString(getBasePath()), "Level Files (*.xml)");
		if (!file.isEmpty()) {
			loadLevel(file.toStdString());
			_level = file;
		}
	}

	void MainWindow::chooseSaveLevel() {
		if (!_level.isEmpty()) {
			saveLevel(_level.toStdString());
		}
	}

	void MainWindow::chooseSaveLevelAs() {
		QString file = QFileDialog::getOpenFileName(nullptr, "Save file ...", QString::fromStdString(getBasePath()), "Level Files (*.xml)");
		if (!file.isEmpty()) {
			saveLevel(file.toStdString());
			_level = file;
		}
	}

	void MainWindow::closeEditor() {
		api::EngineController::GetSingleton().stop();
	}

	void MainWindow::triggeredGameAction(int index) {
		if (_level.size() > 0) {
			_startGame = index;
			_resetEngineController = true;
			api::EngineController::GetSingleton().stop();
		} else {
			QMessageBox box;
			box.setWindowTitle(QString("Can't start game!"));
			box.setInformativeText("Actually no level is loaded. Load a level first to start game with it!");
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

	void MainWindow::AfterInitialize() {
		Editor::AfterInitialize();

		for (plugins::InitializationPluginInterface * ipi : _initializationPlugins) {
			ipi->initialize();
		}

		api::EngineController::GetSingleton().getGUIFacade()->setMouseVisibility(false);

		emit _templateListWidget->loadTemplates();

		if (_inGame) {
			loadLevel(_level.toStdString());
			_inGame = false;
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

	void MainWindow::closeEvent(QCloseEvent * evt) {
		closeEditor();
		evt->ignore();
	}

	void MainWindow::keyPressEvent(QKeyEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
			if (convertQtToEngine(evt->key()) == api::KeyCode::KC_ESCAPE) {
				if (_inGame && !_resetEngineController) {
					_resetEngineController = true;
					api::EngineController::GetSingletonPtr()->stop();
				}
			} else {
				api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_PRESSED, convertQtToEngine(evt->key()), evt->text().toUInt()), core::Subsystem::Input));
			}
			evt->accept();
		}
		evt->ignore();
	}

	void MainWindow::keyReleaseEvent(QKeyEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_RELEASED, convertQtToEngine(evt->key()), evt->text().toUInt()), core::Subsystem::Input));
			evt->accept();
		}
		evt->ignore();
	}

	void MainWindow::mouseMoveEvent(QMouseEvent * evt) {
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouMouse, core::Method::Update, new api::input::Input_Mouse_Update(evt->pos().x(), evt->pos().y()), core::Subsystem::Input));
		evt->accept();
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
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_PRESSED, kc, 0), core::Subsystem::Input));
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
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_RELEASED, kc, 0), core::Subsystem::Input));
		evt->accept();
	}

	bool MainWindow::eventFilter(QObject * obj, QEvent * evt) {
		QWidget * srcWidget = qobject_cast<QWidget *>(obj);
		switch (evt->type()) {
		case QEvent::HoverMove:
		case QEvent::NonClientAreaMouseMove:
		case QEvent::MouseMove: {
			QMouseEvent * me = static_cast<QMouseEvent *>(evt);
			mouseMoveEvent(me);
			break;
		}
		}
		return QWidget::eventFilter(obj, evt);
	}

	void MainWindow::loadPlugins() {
		loadInitializationPlugins();
		loadRunGamePlugins();
	}

	void MainWindow::loadInitializationPlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath());
		pluginsDir.cd("plugins/editor/initialization");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_initializationPlugins.push_back(qobject_cast<plugins::InitializationPluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QString("Error loading plugin!"));
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
				box.setWindowTitle(QString("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
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
		_runGamePlugins[_startGame]->initializeSubSystems(reinterpret_cast<HWND>(_renderWidget->winId()));
		api::EngineController::GetSingletonPtr()->registerApplication(*_runGamePlugins[_startGame]);
		_runGamePlugins[_startGame]->setLevel(_level.toStdString());
		_runGamePlugins[_startGame]->setFinalizeCallback(std::bind(&MainWindow::Finalize, this));
		_engineThread = std::thread(&api::EngineController::start, api::EngineController::GetSingletonPtr());
		_startGame = -1;
	}

	void MainWindow::doStopApp() {
		if (_engineThread.joinable()) {
			_engineThread.join();
		}
		qApp->exit();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
