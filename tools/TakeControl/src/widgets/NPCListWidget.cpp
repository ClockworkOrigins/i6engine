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

#include "widgets/NPCListWidget.h"

#include <cassert>

#include "plugins/DialogPluginInterface.h"

#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>

namespace i6e {
namespace takeControl {
namespace widgets {

	NPCListWidget::NPCListWidget(QWidget * par) : QWidget(par), _listView(new QListView(this)), _npcList() {
		QVBoxLayout * l = new QVBoxLayout();
		l->addWidget(_listView);
		setLayout(l);

		QStringListModel * model = new QStringListModel(_listView);
		_listView->setModel(model);

		connect(_listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(selectedNPC(const QModelIndex &)));
	}

	NPCListWidget::~NPCListWidget() {
	}

	void NPCListWidget::loadedDialogPlugin(plugins::DialogPluginInterface * plugin) {
		QStringListModel * model = dynamic_cast<QStringListModel *>(_listView->model());
		assert(model);
		QStringList list = model->stringList();
		auto npcList = plugin->getNPCs();
		for (rpg::npc::NPC * npc : npcList) {
			list.append(QString::fromStdString(npc->getIdentifier()));
		}
		qSort(list);
		model->setStringList(list);
		_npcList.insert(_npcList.begin(), npcList.begin(), npcList.end());
		std::sort(_npcList.begin(), _npcList.end(), [](rpg::npc::NPC * a, rpg::npc::NPC * b) {
			return a->getIdentifier() < b->getIdentifier();
		});
	}

	void NPCListWidget::selectedNPC(const QModelIndex & idx) {
		int index = idx.row();
		QStringListModel * model = dynamic_cast<QStringListModel *>(_listView->model());
		assert(model);
		emit selectNPC(model->stringList().at(index));
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
