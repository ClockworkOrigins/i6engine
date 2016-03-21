#include "connections/LineConnector.h"

#include <iostream>

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QPainter>
#include <QPen>

namespace i6engine {
namespace particleEditor {
namespace connections {

	LineConnector::LineConnector(QGraphicsWidget * first, QGraphicsWidget * second, QColor colour, Qt::PenStyle lineStyle) : _first(first), _second(second), _colour(colour), _lineStyle(lineStyle), _path() {
	}

	void LineConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) {
		static const qreal xOffsetMin = 24;

		QPen pen(QBrush(_colour), _lineStyle);
		pen.setWidth(2);
		painter->setPen(pen);

		QPoint points[4];
		qreal x1 = _first->pos().x();
		qreal y1 = _first->pos().y();
		qreal x2 = _second->pos().x();
		qreal y2 = _second->pos().y();
		qreal width1 = _first->size().width();
		qreal height1 = _first->size().height();
		qreal width2 = _second->size().width();
		qreal height2 = _second->size().height();
		qreal x3, y3;
		qreal x4, y4;

		y1 = y1 + 0.5 * height1;
		y2 = y2 + 0.5 * height2;
		y3 = y1;
		y4 = y2;

		// Determine widest rectangle
		if (width2 > width1) {
			// Swap
			x1 += x2; // = x1 + x2
			x2 = x1 - x2; // = x1 + x2 - x2 = x1
			x1 -= x2; // = x1 + x2 - x1 = x2

			y1 += y2;
			y2 = y1 - y2;
			y1 -= y2;

			y3 += y4;
			y4 = y3 - y4;
			y3 -= y4;

			width1 += width2;
			width2 = width1 - width2;
			width1 -= width2;

			height1 += height2;
			height2 = height1 - height2;
			height1 -= height2;
		}

		// Calculate the 4 control points
		qreal xOffset = 0.25 * std::abs(x1 - x2);
		xOffset = std::max(xOffset, xOffsetMin);
		if (x1 + width1 > x2 + width2) {
			x3 = x1 - xOffset;
			x4 = x2 - xOffset;
			if (x1 > x2 + width2) {
				x2 += width2;
				x4 = x2 + xOffset;
			}
		} else {
			x1 += width1;
			x3 = x1 + xOffset;
			x4 = x2 - xOffset;
			if (x1 > x2) {
				x2 += width2;
				x4 = x2 + xOffset;
			}
		}

		prepareGeometryChange();
		_path = QPainterPath(QPointF(x1, y1));
		_path.cubicTo(QPointF(x3, y3), QPointF(x4, y4), QPointF(x2, y2));
		painter->drawPath(_path);
	}

	QRectF LineConnector::boundingRect() const {
		return _path.boundingRect();
	}

} /* namespace connections */
} /* namespace particleEditor */
} /* namespace i6engine */
