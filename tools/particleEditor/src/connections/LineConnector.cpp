/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "connections/LineConnector.h"

#include <QBrush>
#include <QGraphicsWidget>
#include <QPainter>
#include <QPen>

namespace i6e {
namespace particleEditor {
namespace connections {

	const qreal xOffsetMin = 24;

	LineConnector::LineConnector(QGraphicsWidget * first, QGraphicsWidget * second, QColor colour, Qt::PenStyle lineStyle) : _first(first), _second(second), _colour(colour), _lineStyle(lineStyle) {
		updatePath();

		QPen pen(QBrush(_colour), _lineStyle);
		pen.setWidth(2);
		setPen(pen);
	}

	void LineConnector::updatePath() {
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
		qreal xOffset = 0.25 * qAbs(x1 - x2);
		xOffset = qMax(xOffset, xOffsetMin);
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

		QPainterPath path = QPainterPath(QPointF(x1, y1));
		path.cubicTo(QPointF(x3, y3), QPointF(x4, y4), QPointF(x2, y2));
		setPath(path);
	}

} /* namespace connections */
} /* namespace particleEditor */
} /* namespace i6e */
