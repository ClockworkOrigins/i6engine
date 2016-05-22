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

#include "widgets/NpcListWidget.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include <QMessageBox>

namespace i6e {
namespace dialogCreator {
namespace widgets {

	NpcListWidget::NpcListWidget(QWidget * par) : QWidget(par), _dialogItems() {
		setupUi(this);

		refreshNpcList();
	}

	NpcListWidget::~NpcListWidget() {
	}

	void NpcListWidget::refreshLists() {
		refreshNpcList();
	}

	void NpcListWidget::selectedDialog(QTreeWidgetItem * item) {
		auto it = _dialogItems.find(item);
		if (it != _dialogItems.end()) {
			emit selectDialog(item->text(0));
		}
	}

	void NpcListWidget::refreshNpcList() {
		treeWidget->clear();
		_dialogItems.clear();

		std::map<std::string, std::vector<rpg::dialog::Dialog *>> npcDialogs = rpg::dialog::DialogManager::GetSingletonPtr()->getNpcDialogs();

		for (const auto & p : npcDialogs) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(p.first) });
			treeWidget->addTopLevelItem(twi);
			std::vector<rpg::dialog::Dialog *> dialogs = p.second;
			std::sort(dialogs.begin(), dialogs.end(), [](rpg::dialog::Dialog * a, rpg::dialog::Dialog * b) {
				return a->nr < b->nr;
			});
			for (rpg::dialog::Dialog * d : dialogs) {
				QTreeWidgetItem * twi2 = new QTreeWidgetItem(twi, { QString::fromStdString(d->identifier) });
				twi->addChild(twi2);
				_dialogItems.insert(twi2);
			}
		}
		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectedDialog(QTreeWidgetItem *)));
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */
