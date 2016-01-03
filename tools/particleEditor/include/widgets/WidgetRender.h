#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__

#include "ui_widgetRender.h"

#include <thread>

namespace Ogre {
	class RenderWindow;
	class Root;
	class SceneManager;
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

	private:
		Ogre::Root * _root;
		modules::ResourceManager * _resourceManager;
		Ogre::RenderWindow * _rWindow;
		Ogre::SceneManager * _sceneManager;
		bool _running;
		std::thread _renderThread;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__ */
