#include "widgets/GraphicsScene.h"

#include "widgets/WidgetEditComponent.h"

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GraphicsScene::GraphicsScene(QObject * par) : QGraphicsScene(par) {
	}

	GraphicsScene::~GraphicsScene() {
	}

	void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent * evt) {
		QByteArray ba = evt->mimeData()->data("widget");
		QDataStream ds(ba);
		quint64 i;
		QPointF offset;
		ds >> i >> offset;
		WidgetEditComponent * wec = (WidgetEditComponent *) i;
		wec->setPos(evt->scenePos() + offset);
		update(sceneRect());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
