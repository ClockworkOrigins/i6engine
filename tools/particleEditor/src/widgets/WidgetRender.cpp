#include "widgets/WidgetRender.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "ParticleUniverseSystemManager.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"

#include <QMessageBox>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetRender::WidgetRender(QWidget * par) : QWidget(par), _root(nullptr), _resourceManager(nullptr), _rWindow(nullptr), _sceneManager(nullptr), _running(true), _renderThread() {
		setupUi(this);

		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("i6engine.ini")) {
			QMessageBox box;
			box.setWindowTitle(QString("Error initializing render widget!"));
			box.setInformativeText("i6engine.ini not found!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}

		std::string ogrePath;
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue("GRAPHIC", "ogreConfigsPath", ogrePath)) {
			QMessageBox box;
			box.setWindowTitle(QString("Error initializing render widget!"));
			box.setInformativeText("value ogreConfigsPath in section GRAPHIC not found!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}

		Ogre::LogManager * lm = new Ogre::LogManager();
		lm->createLog("ogre.log", true, false, false);
		_root = new Ogre::Root(ogrePath + "/plugins.cfg", ogrePath + "/ogre.cfg", "");

		// make sure, Resourcemanager is initialized
		_resourceManager = new modules::ResourceManager(ogrePath);

		_root->restoreConfig();

		_root->initialise(false, "ParticleEditor");
		Ogre::NameValuePairList misc;
		misc["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(winId()));
		misc["parentWindowHandle"] = Ogre::StringConverter::toString(size_t(winId()));
		_rWindow = _root->createRenderWindow("ParticleEditor", size().width(), size().height(), false, &misc);
		
		_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		_renderThread = std::thread([this]() {
			while (_running) {
				_root->renderOneFrame();
			}
		});
	}

	WidgetRender::~WidgetRender() {
		_running = false;
		_renderThread.join();
		delete _resourceManager;
		_rWindow->destroy();
		_root->shutdown();
		delete _root;
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
