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

#include "i6engine/editor/widgets/ObjectListWidget.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/editor/widgets/MainWindow.h"

#include <QStringListModel>

namespace i6e {
namespace editor {
namespace widgets {

	ObjectListWidget::ObjectListWidget(QWidget * par, QWidget * mainWindow) : QWidget(par), _mainWindow(dynamic_cast<MainWindow *>(mainWindow)) {
		setupUi(this);

		connect(this, SIGNAL(updateObjectList()), this, SLOT(doUpdateObjectList()));
	}

	ObjectListWidget::~ObjectListWidget() {
	}

	void ObjectListWidget::doUpdateObjectList() {
		QStringList list;
		for (auto & p : api::EngineController::GetSingleton().getObjectFacade()->getGOMap()) {
			if (p.second->getType() != "EditorCam") {
				list.append(QString::fromStdString(p.second->getType()));
			}
		}
		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

	void ObjectListWidget::selectIndex(QModelIndex index) {
		auto goMap = api::EngineController::GetSingleton().getObjectFacade()->getGOMap();
		int idx = index.row();
		for (auto it = goMap.begin(); it != goMap.end(); it++) {
			if (it->second->getType() != "EditorCam") {
				if (idx == 0) {
					emit selectObject(it->second->getID());
					break;
				}
				idx--;
			}
		}

		_mainWindow->setFocus();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */
