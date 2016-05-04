#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/ObjectConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"

#include "widgets/NPCEditWidget.h"
#include "widgets/NPCListWidget.h"
#include "widgets/NPCRenderWidget.h"

#include <QCloseEvent>

namespace i6e {
namespace tools {
namespace npcCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), RPGApplication(), _npcListWidget(nullptr), _npcEditWidget(nullptr), _npcRenderWidget(nullptr) {
		setupUi(this);

		setWindowTitle(QString("NPCCreator (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		_npcListWidget = new NPCListWidget(this);
		_npcEditWidget = new NPCEditWidget(this);
		_npcRenderWidget = new NPCRenderWidget(this);

		_npcListWidget->setNPCEditWidget(_npcEditWidget);
		_npcEditWidget->setNPCListWidget(_npcListWidget);

		showMaximized();

		gridLayout->addWidget(_npcListWidget, 0, 0);
		gridLayout->addWidget(_npcEditWidget, 0, 1);
		gridLayout->addWidget(_npcRenderWidget, 0, 2);

		gridLayout->setColumnStretch(0, 2);
		gridLayout->setColumnStretch(1, 9);
		gridLayout->setColumnStretch(2, 9);

		api::EngineController::GetSingletonPtr()->registerSubSystem("Graphics", new modules::GraphicsController(reinterpret_cast<HWND>(_npcRenderWidget->winId())), { core::Subsystem::Object });
		api::EngineController::GetSingletonPtr()->registerSubSystem("Object", new modules::ObjectController(), 33000);

		api::EngineController::GetSingletonPtr()->registerApplication(*this);

		std::thread(&api::EngineController::start, api::EngineController::GetSingletonPtr()).detach();

		menuFile->setTitle(QApplication::tr("File"));
		actionExit->setText(QApplication::tr("Exit"));
		actionExit->setIcon(QIcon(":/images/close.png"));
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeCreator() {
		api::EngineController::GetSingleton().stop();
	}

	void MainWindow::closeEvent(QCloseEvent * evt) {
		closeCreator();
		evt->ignore();
	}

	void MainWindow::AfterInitialize() {
		RPGApplication::AfterInitialize();
		api::EngineController::GetSingleton().getGUIFacade()->setMouseVisibility(false);

		// add a camera!
		{
			api::objects::GOTemplate tmpl;
			api::attributeMap paramsSSC;
			paramsSSC.insert(std::make_pair("pos", "0.0 0.0 -10.0"));
			api::attributeMap paramsCamera;
			paramsCamera.insert(std::make_pair("lookAt", "0.0 0.0 -8.0"));
			paramsCamera.insert(std::make_pair("aspect", std::to_string(double(_npcRenderWidget->size().width()) / _npcRenderWidget->size().height())));
			tmpl._components.push_back(api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));
			tmpl._components.push_back(api::objects::GOTemplateComponent("Camera", paramsCamera, "", false, false));
			api::EngineController::GetSingletonPtr()->getObjectFacade()->createObject("EditorCam", tmpl, api::EngineController::GetSingleton().getUUID(), false);
		}

		// add a spot light
		{
			api::objects::GOTemplate tmpl;
			api::attributeMap paramsSSC;
			paramsSSC.insert(std::make_pair("pos", "0.0 0.0 -10.0"));
			api::attributeMap paramsLuminous;
			paramsLuminous.insert(std::make_pair("lightType", "2"));
			paramsLuminous.insert(std::make_pair("direction", "0.0 0.0 1.0"));
			paramsLuminous.insert(std::make_pair("spotLightRangeInner", "35"));
			paramsLuminous.insert(std::make_pair("spotLightRangeOuter", "50"));
			tmpl._components.push_back(api::objects::GOTemplateComponent("StaticState", paramsSSC, "", false, false));
			tmpl._components.push_back(api::objects::GOTemplateComponent("LuminousAppearance", paramsLuminous, "", false, false));
			api::EngineController::GetSingletonPtr()->getObjectFacade()->createObject("Light", tmpl, api::EngineController::GetSingleton().getUUID(), false);
		}

	}

	void MainWindow::Finalize() {
		RPGApplication::Finalize();
		qApp->exit();
	}

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6e */
