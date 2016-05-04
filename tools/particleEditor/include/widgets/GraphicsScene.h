#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAPHICSSCENE_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAPHICSSCENE_H__

#include <QGraphicsScene>

namespace i6e {
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

		void addItem(QGraphicsItem * item);
		void removeItem(QGraphicsItem * item);

	private:
		void dragMoveEvent(QGraphicsSceneDragDropEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAPHICSSCENE_H__ */
