#ifndef __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_LINECONNECTOR_H__
#define __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_LINECONNECTOR_H__

#include <QGraphicsItem>

namespace i6e {
namespace particleEditor {
namespace connections {

	class LineConnector : public QObject, public QGraphicsItem {
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

	public:
		LineConnector(QGraphicsWidget * first, QGraphicsWidget * second, QColor color, Qt::PenStyle lineStyle);

	private:
		QGraphicsWidget * _first;
		QGraphicsWidget * _second;
		QColor _colour;
		Qt::PenStyle _lineStyle;
		QPainterPath _path;

		void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

		QRectF boundingRect() const override;
	};

} /* namespace connections */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_LINECONNECTOR_H__ */
