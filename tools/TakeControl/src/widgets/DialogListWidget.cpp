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

#include "widgets/DialogListWidget.h"

#include "plugins/DialogPluginInterface.h"

#include <QTreeWidget>
#include <QVBoxLayout>

namespace i6e {
namespace takeControl {
namespace widgets {

	DialogListWidget::DialogListWidget(QWidget * par) : QWidget(par), _treeWidget(new QTreeWidget(this)), _dialogList(), _activeDialogList(), _npcIdentifier() {
		QVBoxLayout * l = new QVBoxLayout();
		l->addWidget(_treeWidget);
		setLayout(l);

		_treeWidget->clear();
		_treeWidget->setHeaderHidden(true);
		_treeWidget->setColumnCount(2);

		connect(_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectedDialog(QTreeWidgetItem *, int)));
	}

	DialogListWidget::~DialogListWidget() {
	}

	void DialogListWidget::loadedDialogPlugin(plugins::DialogPluginInterface * plugin) {
		auto dialogs = plugin->getDialogs();
		for (auto & t : dialogs) {
			_dialogList.push_back(std::make_tuple(std::get<0>(t), std::get<1>(t), plugin));
		}
		connect(dynamic_cast<QObject *>(plugin), SIGNAL(updatedData()), this, SLOT(updateData()));
	}

	void DialogListWidget::selectedNPC(QString identifier) {
		_treeWidget->clear();
		_activeDialogList.clear();
		for (auto & t : _dialogList) {
			std::vector<std::string> participants = std::get<0>(t)->participants;
			for (auto s : participants) {
				if (QString::fromStdString(s).compare(identifier, Qt::CaseSensitivity::CaseInsensitive) == 0) {
					QTreeWidgetItem * twi = new QTreeWidgetItem(_treeWidget, { QString::fromStdString(std::get<0>(t)->identifier) });
					_treeWidget->addTopLevelItem(twi);
					for (auto & t2 : std::get<1>(t)) {
						twi->addChild(new QTreeWidgetItem(twi, { QString::fromStdString(std::get<0>(t2)), std::get<2>(t)->getSubtitleText(QString::fromStdString(std::get<1>(t2))) }));
					}
					_activeDialogList.push_back(t);
				}
			}
		}
		_treeWidget->resizeColumnToContents(0);
		_npcIdentifier = identifier;
	}

	void DialogListWidget::selectedDialog(QTreeWidgetItem * item, int) {
		if (item != _treeWidget->topLevelItem(_treeWidget->indexOfTopLevelItem(item))) {
			emit selectDialog(item->text(1));
			emit selectNPC(item->text(0));
		} else {
			emit selectDialog(QString());
			emit selectNPC(_npcIdentifier);
		}
	}

	void DialogListWidget::updateData() {
		plugins::DialogPluginInterface * plugin = dynamic_cast<plugins::DialogPluginInterface *>(sender());
		_dialogList.clear();
		auto dialogs = plugin->getDialogs();
		for (auto & t : dialogs) {
			_dialogList.push_back(std::make_tuple(std::get<0>(t), std::get<1>(t), plugin));
		}
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
