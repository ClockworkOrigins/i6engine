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

#include "i6engine/editor/widgets/TemplateListWidget.h"

#include <thread>

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/editor/widgets/MainWindow.h"

#include <QStringListModel>

namespace i6e {
namespace editor {
namespace widgets {

	TemplateListWidget::TemplateListWidget(QWidget * par) : QWidget(par), _editor(dynamic_cast<MainWindow *>(par)) {
		setupUi(this);

		connect(this, SIGNAL(loadTemplates()), this, SLOT(doLoadTemplates()));
	}

	TemplateListWidget::~TemplateListWidget() {
	}

	void TemplateListWidget::doLoadTemplates() {
		QStringList list;
		for (auto & p : api::EngineController::GetSingleton().getObjectFacade()->getGOTemplates()) {
			list.append(QString::fromStdString(p.first));
		}
		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

	void TemplateListWidget::selectIndex(QModelIndex index) {
		if (_editor->getCamera() == nullptr) {
			return;
		}
		auto tmpl = api::EngineController::GetSingleton().getObjectFacade()->getGOTemplates()[listView->model()->data(index).toString().toStdString()];

		for (auto & c : tmpl._components) {
			if (c._template == "PhysicalState" || c._template == "StaticState") {
				utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _editor->getCamera()->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				Vec3 newPos = ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, 5.0), ssc->getRotation());
				newPos.insertInMap("pos", c._params);
			}
		}

		api::EngineController::GetSingleton().getObjectFacade()->createGO(listView->model()->data(index).toString().toStdString(), tmpl, api::EngineController::GetSingleton().getUUID(), false, [this](api::GOPtr) {
			emit updateObjectList();
		});

		emit changedLevel();

		dynamic_cast<MainWindow *>(_editor)->setFocus();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */
