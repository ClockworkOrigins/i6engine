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

#include "widgets/GraphicsScene.h"

#include "connections/LineConnector.h"

#include "widgets/WidgetEditComponent.h"

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

namespace i6e {
namespace particleEditor {
namespace widgets {

	GraphicsScene::GraphicsScene(QObject * par) : QGraphicsScene(par) {
	}

	GraphicsScene::~GraphicsScene() {
	}

	void GraphicsScene::addItem(QGraphicsItem * item) {
		QGraphicsScene::addItem(item);
		setSceneRect(itemsBoundingRect());
	}

	void GraphicsScene::removeItem(QGraphicsItem * item) {
		QGraphicsScene::removeItem(item);
		setSceneRect(itemsBoundingRect());
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
		wec->getConnection()->updatePath();
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
