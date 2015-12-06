#include "i6engine/editor/widgets/MainWindow.h"

#include <thread>

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/audio/AudioController.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#include "i6engine/editor/widgets/ObjectContainerWidget.h"
#include "i6engine/editor/widgets/ObjectInfoWidget.h"
#include "i6engine/editor/widgets/ObjectListWidget.h"
#include "i6engine/editor/widgets/RenderWidget.h"
#include "i6engine/editor/widgets/TemplateListWidget.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>

namespace i6engine {
namespace editor {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), Editor(), _renderWidget(new RenderWidget(this)), _objectContainerWidget(new ObjectContainerWidget(this)), _templateListWidget(new TemplateListWidget(this)), _level() {
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

		api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new modules::GraphicsController(reinterpret_cast<HWND>(_renderWidget->winId())), { core::Subsystem::Object });
		api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new modules::ObjectController(), LNG_OBJECT_FRAME_TIME);
		api::EngineController::GetSingletonPtr()->registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#ifdef ISIXE_WITH_AUDIO
		api::EngineController::GetSingletonPtr()->registerSubSystem("Audio", new modules::AudioController(), LNG_AUDIO_FRAME_TIME);
#endif
		api::EngineController::GetSingletonPtr()->registerApplication(*this);

		std::thread(&api::EngineController::start, api::EngineController::GetSingletonPtr()).detach();

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

	void MainWindow::AfterInitialize() {
		Editor::AfterInitialize();

		api::EngineController::GetSingleton().getGUIFacade()->setMouseVisibility(false);

		std::thread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			emit _templateListWidget->loadTemplates();
		}).detach();
	}

	void MainWindow::Finalize() {
		Editor::Finalize();
		qApp->exit();
	}

	void MainWindow::updateObjectList() {
		emit _objectContainerWidget->objectListWidget->updateObjectList();
	}

	void MainWindow::selectObject(int64_t id) {
		emit _objectContainerWidget->objectInfoWidget->selectObject(id);
	}

	void MainWindow::closeEvent(QCloseEvent * evt) {
		closeEditor();
		evt->ignore();
	}

	void MainWindow::keyPressEvent(QKeyEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_PRESSED, api::KeyCode(evt->nativeScanCode()), evt->text().toUInt()), core::Subsystem::Input));
			evt->accept();
		}
		evt->ignore();
	}

	void MainWindow::keyReleaseEvent(QKeyEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
			api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::keyboard::KeyKeyboard, core::Method::Update, new api::input::Input_Keyboard_Update(api::KeyState::KEY_RELEASED, api::KeyCode(evt->nativeScanCode()), evt->text().toUInt()), core::Subsystem::Input));
			evt->accept();
		}
		evt->ignore();
	}

	void MainWindow::mouseMoveEvent(QMouseEvent * evt) {
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouMouse, core::Method::Update, new api::input::Input_Mouse_Update(evt->pos().x(), evt->pos().y()), core::Subsystem::Input));
		evt->accept();
	}

	void MainWindow::mousePressEvent(QMouseEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
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
		evt->ignore();
	}

	void MainWindow::mouseReleaseEvent(QMouseEvent * evt) {
		if (_renderWidget->isActiveWindow()) {
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
		evt->ignore();
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

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
