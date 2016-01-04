#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__

#include "ui_widgetRender.h"

namespace Ogre {
	class Camera;
	class Overlay;
	class OverlayContainer;
	class RenderWindow;
	class Root;
	class SceneManager;
	class SceneNode;
	class TextAreaOverlayElement;
	class Viewport;
} /* namespace Ogre */

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace modules {
	class ResourceManager;
} /* namespace modules */
namespace particleEditor {
namespace widgets {

	class WidgetRender : public QWidget, public Ui::renderWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetRender(QWidget * par);

		/**
		 * \brief destructor
		 */
		~WidgetRender();

	public slots:
		void play();
		void pause();
		void stop();
		void createNewSystem(const QString & particle);

	signals:
		void triggerRender();

	private slots:
		void render();

	private:
		Ogre::Root * _root;
		modules::ResourceManager * _resourceManager;
		Ogre::RenderWindow * _rWindow;
		Ogre::SceneManager * _sceneManager;
		Ogre::Overlay * _overlay;
		Ogre::OverlayContainer * _textPanel;
		Ogre::TextAreaOverlayElement * _averageFPS;
		Ogre::SceneNode * _cameraNode;
		Ogre::Camera * _camera;
		Ogre::Viewport * _viewport;
		Ogre::SceneNode * _particleNode;
		ParticleUniverse::ParticleSystem * _currentParticleSystemForRenderer;
		size_t _maxNumberOfVisualParticles;
		size_t _maxNumberOfEmittedParticles;

		void updateOverlay();
		void resizeEvent(QResizeEvent * evt) override;
		void wheelEvent(QWheelEvent * evt) override;
		void zoom(double zoomFactor);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__ */
