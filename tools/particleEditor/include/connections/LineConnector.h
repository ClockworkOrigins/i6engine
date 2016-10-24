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

#ifndef __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_LINECONNECTOR_H__
#define __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_LINECONNECTOR_H__

#include <QGraphicsItem>

namespace i6e {
namespace particleEditor {
namespace connections {

	class LineConnector : public QObject, public QGraphicsPathItem {
		Q_OBJECT

	public:
		LineConnector(QGraphicsWidget * first, QGraphicsWidget * second, QColor color, Qt::PenStyle lineStyle);

		void updatePath();

		QGraphicsWidget * getFrom() const {
			return _first;
		}

		QGraphicsWidget * getTo() const {
			return _second;
		}

	private:
		QGraphicsWidget * _first;
		QGraphicsWidget * _second;
		QColor _colour;
		Qt::PenStyle _lineStyle;
	};

} /* namespace connections */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_CONNECTIONS_LINECONNECTOR_H__ */
