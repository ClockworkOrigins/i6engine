#include "widgets/WidgetRender.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/Overlay/OgreOverlay.h"
#include "OGRE/Overlay/OgreOverlayManager.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"

#include <QMessageBox>
#include <QResizeEvent>

#undef max

namespace i6engine {
namespace particleEditor {
namespace widgets {

	std::string CURRENT_PS_NAME = "currentParticleSystemName";

	WidgetRender::WidgetRender(QWidget * par) : QWidget(par), _root(nullptr), _resourceManager(nullptr), _rWindow(nullptr), _sceneManager(nullptr), _overlay(nullptr), _textPanel(nullptr), _averageFPS(nullptr), _cameraNode(nullptr), _camera(nullptr), _viewport(nullptr), _particleNode(nullptr), _currentParticleSystemForRenderer(nullptr), _maxNumberOfVisualParticles(0), _maxNumberOfEmittedParticles(0) {
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

		_sceneManager->addRenderQueueListener(new Ogre::OverlaySystem());

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		_cameraNode = _sceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");
		_particleNode = _sceneManager->getRootSceneNode()->createChildSceneNode("ParticleNode");

		_camera = _sceneManager->createCamera("MainCamera");
		_camera->setPosition(Ogre::Vector3(0.0, 0.0, 30.0));
		_camera->setNearClipDistance(0.1);
		_camera->setFarClipDistance(99999.0);

		_cameraNode->attachObject(_camera);

		_viewport = _rWindow->addViewport(_camera, 0, 0.0f, 0.0f, 1.0f, 1.0f);

		Ogre::FontManager * fontManager = Ogre::FontManager::getSingletonPtr();
		Ogre::ResourcePtr font = fontManager->create("MyFont", "General");
		font->setParameter("type", "truetype");
		font->setParameter("source", "bluehigh.ttf");
		font->setParameter("size", "32");
		font->load();

		_textPanel = static_cast<Ogre::OverlayContainer *>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "ParticleUniverseText"));
		_averageFPS = static_cast<Ogre::TextAreaOverlayElement *>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "ParticleUniverseStats"));
		_averageFPS->setPosition(0.0, 0.02);
		_averageFPS->setDimensions(0.3, 0.4);
		_averageFPS->setColour(Ogre::ColourValue::White);
		_averageFPS->setParameter("horz_align", "left");
		_averageFPS->setFontName("MyFont");
		_textPanel->addChild(_averageFPS);
		_overlay = Ogre::OverlayManager::getSingleton().create("Core/DebugOverlay");
		_overlay->setZOrder(500);
		_overlay->add2D(_textPanel);
		_overlay->show();

		connect(this, SIGNAL(triggerRender()), this, SLOT(render()), Qt::QueuedConnection);

		emit triggerRender();
	}

	WidgetRender::~WidgetRender() {
		delete _resourceManager;
		_rWindow->destroy();
		_root->shutdown();
		delete _root;
	}

	void WidgetRender::play() {
		if (_currentParticleSystemForRenderer) {
			if (_currentParticleSystemForRenderer->getState() == ParticleUniverse::ParticleSystem::PSS_PAUSED) {
				_currentParticleSystemForRenderer->resume();
			} else {
				_maxNumberOfVisualParticles = 0;
				_maxNumberOfEmittedParticles = 0;
				_currentParticleSystemForRenderer->start();
			}
		}
	}

	void WidgetRender::pause() {
		if (_currentParticleSystemForRenderer) {
			_currentParticleSystemForRenderer->pause();
		}
	}

	void WidgetRender::stop() {
		if (_currentParticleSystemForRenderer) {
			_currentParticleSystemForRenderer->stop();
		}
	}

	void WidgetRender::createNewSystem(const QString & particle) {
		if (_currentParticleSystemForRenderer) {
			_particleNode->detachObject(_currentParticleSystemForRenderer);
			ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(CURRENT_PS_NAME, _sceneManager);
		}
		_currentParticleSystemForRenderer = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(CURRENT_PS_NAME, particle.toStdString(), _sceneManager);
		_particleNode->attachObject(_currentParticleSystemForRenderer);

		emit loadScript(_currentParticleSystemForRenderer);
	}

	void WidgetRender::render() {
		_root->renderOneFrame();
		updateOverlay();
		emit triggerRender();
	}

	void WidgetRender::updateOverlay() {
		ParticleUniverse::String cameraProjectionType = "Perspective";
		ParticleUniverse::String cameraProjection = " Camera projection: " + cameraProjectionType + "\n";
		ParticleUniverse::String caption = cameraProjection;

		// Average FPS
		const Ogre::RenderTarget::FrameStats & stats = _rWindow->getStatistics();
		ParticleUniverse::String averageFPSString = " Average FPS: " + ParticleUniverse::StringConverter::toString(stats.avgFPS) + "\n";

		// Position of the selected node
		Ogre::Vector3 pivot = Ogre::Vector3::ZERO;
		ParticleUniverse::String positionString = " Position: " + ParticleUniverse::StringConverter::toString(pivot.x) + " " + ParticleUniverse::StringConverter::toString(pivot.y) + " " + ParticleUniverse::StringConverter::toString(pivot.z) + "\n";
		caption = caption + averageFPSString + positionString;

		// Distance of the selected node
		Ogre::Vector3 d = _camera->getPosition() - pivot;
		ParticleUniverse::Real l = d.length();
		ParticleUniverse::String distanceString = " Camera distance: " + ParticleUniverse::StringConverter::toString(l) + "\n";
		caption = caption + distanceString;

		if (_currentParticleSystemForRenderer) {
			// Number of emitted visual particles
			size_t numberOfVisualParticles = _currentParticleSystemForRenderer->getNumberOfEmittedParticles(ParticleUniverse::Particle::PT_VISUAL);
			_maxNumberOfVisualParticles = std::max(_maxNumberOfVisualParticles, numberOfVisualParticles);
			ParticleUniverse::String numberOfVisualParticlesString = "\n Total visual particles: " + ParticleUniverse::StringConverter::toString(numberOfVisualParticles) + "\n";
			caption = caption + numberOfVisualParticlesString;
			ParticleUniverse::String maxNumberOfVisualParticlesString = " Max visual particles: " + ParticleUniverse::StringConverter::toString(_maxNumberOfVisualParticles) + "\n";
			caption = caption + maxNumberOfVisualParticlesString;

			// Number of emitted non-visual particles
			size_t numberOfEmittedParticles = _currentParticleSystemForRenderer->getNumberOfEmittedParticles() - _currentParticleSystemForRenderer->getNumberOfEmittedParticles(ParticleUniverse::Particle::PT_VISUAL);
			_maxNumberOfEmittedParticles = std::max(_maxNumberOfEmittedParticles, numberOfEmittedParticles);
			ParticleUniverse::String numberOfNonVisualParticlesString = " Total non-visual particles: " + ParticleUniverse::StringConverter::toString(numberOfEmittedParticles) + "\n";
			caption = caption + numberOfNonVisualParticlesString;
			ParticleUniverse::String maxNumberOfNonVisualParticlesString = " Max non-visual particles: " + ParticleUniverse::StringConverter::toString(_maxNumberOfEmittedParticles) + "\n";
			caption = caption + maxNumberOfNonVisualParticlesString;
		}

		_averageFPS->setCaption(caption);
	}

	void WidgetRender::resizeEvent(QResizeEvent * evt) {
		_rWindow->resize(static_cast<unsigned int>(evt->size().width()), static_cast<unsigned int>(evt->size().height()));
		_rWindow->windowMovedOrResized();
		_camera->setAspectRatio(double(evt->size().width()) / double(evt->size().height()));
		evt->ignore();
	}

	void WidgetRender::wheelEvent(QWheelEvent * evt) {
		if (evt->delta() > 0) {
			// Zoom in
			zoom(0.95);
		} else if (evt->delta() < 0) {
			// Zoom out
			zoom(1.05);
		}
		evt->accept();
	}

	void WidgetRender::zoom(double zoomFactor) {
		Ogre::Vector3 cameraPosition = _camera->getPosition();
		Ogre::Vector3 pivot = Ogre::Vector3::ZERO;
		Ogre::Vector3 direction = cameraPosition - pivot;
		_camera->setPosition(pivot + zoomFactor * direction);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
