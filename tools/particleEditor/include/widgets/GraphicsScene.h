#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAPHICSSCENE_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAPHICSSCENE_H__

#include <QGraphicsScene>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class GraphicsScene : public QGraphicsScene {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		GraphicsScene(QObject * par);

		/**
		 * \brief destructor
		 */
		~GraphicsScene();

	private:
		void dragMoveEvent(QGraphicsSceneDragDropEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAPHICSSCENE_H__ */
