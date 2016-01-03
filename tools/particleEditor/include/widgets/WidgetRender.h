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
	class TextAreaOverlayElement;
	class Viewport;
} /* namespace Ogre */

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
		Ogre::Camera * _camera;
		Ogre::Viewport * _viewport;

		void updateOverlay();
		void resizeEvent(QResizeEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__ */
