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

#include <QApplication>
#include <QCheckBox>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>

namespace i6e {
namespace takeControl {
namespace widgets {

	FilterNPCsWithoutDialogModel::FilterNPCsWithoutDialogModel(QWidget * par) : QSortFilterProxyModel(par), _checked(true) {
	}

	bool FilterNPCsWithoutDialogModel::filterAcceptsRow(int source_row, const QModelIndex &) const {
		QStandardItemModel * model = dynamic_cast<QStandardItemModel *>(sourceModel());
		assert(model);
		return !_checked || (model && model->item(source_row, 1) && model->item(source_row, 1)->text() != "0");
	}

	void FilterNPCsWithoutDialogModel::stateChanged(int checkState) {
		_checked = checkState == Qt::CheckState::Checked;
		invalidateFilter();
	}

	NPCListWidget::NPCListWidget(QWidget * par) : QWidget(par), _tableView(new QTableView(this)), _npcList(), _sourceModel(nullptr) {
		QVBoxLayout * l = new QVBoxLayout();
		l->addWidget(_tableView);

		QCheckBox * cb = new QCheckBox(QApplication::tr("Hide NPCs without dialogs"), this);
		cb->setChecked(true);

		l->addWidget(cb);

		setLayout(l);

		FilterNPCsWithoutDialogModel * filterModel = new FilterNPCsWithoutDialogModel(_tableView);
		_sourceModel = new QStandardItemModel(_tableView);
		filterModel->setSourceModel(_sourceModel);
		_sourceModel->setHorizontalHeaderLabels(QStringList() << QApplication::tr("Character") << QApplication::tr("Dialogs") << QApplication::tr("Takes"));
		_tableView->setModel(filterModel);
		_tableView->verticalHeader()->hide();
		_tableView->setCornerButtonEnabled(false);
		_tableView->setGridStyle(Qt::PenStyle::NoPen);

		connect(_tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(selectedNPC(const QModelIndex &)));
		connect(cb, SIGNAL(stateChanged(int)), filterModel, SLOT(stateChanged(int)));
	}

	NPCListWidget::~NPCListWidget() {
	}

	void NPCListWidget::loadedDialogPlugin(plugins::DialogPluginInterface * plugin) {
		assert(_sourceModel);
		QStringList list;
		auto npcList = plugin->getNPCs();
		for (rpg::npc::NPC * npc : npcList) {
			list.append(QString::fromStdString(npc->getIdentifier()));
		}
		qSort(list);
		_sourceModel->removeRows(0, _sourceModel->rowCount());
		auto dlgList = plugin->getDialogs();
		std::map<QString, int> counter;
		std::map<QString, int> dialogCounter;
		for (size_t i = 0; i < dlgList.size(); i++) {
			auto vec = std::get<1>(dlgList[i]);
			for (auto t : vec) {
				counter[QString::fromStdString(std::get<0>(t))]++;
			}
			rpg::dialog::Dialog * dlg = std::get<0>(dlgList[i]);
			for (std::string s : dlg->participants) {
				dialogCounter[QString::fromStdString(s)]++;
			}
		}
		for (int i = 0; i < list.size(); i++) {
			_sourceModel->setItem(i, 0, new QStandardItem(list[i]));
			_sourceModel->setItem(i, 1, new QStandardItem(QString::number(dialogCounter[list[i]])));
			_sourceModel->setItem(i, 2, new QStandardItem(QString::number(counter[list[i]])));
			_tableView->setRowHeight(i, 20);
		}
		_npcList.insert(_npcList.begin(), npcList.begin(), npcList.end());
		std::sort(_npcList.begin(), _npcList.end(), [](rpg::npc::NPC * a, rpg::npc::NPC * b) {
			return a->getIdentifier() < b->getIdentifier();
		});
		connect(dynamic_cast<QObject *>(plugin), SIGNAL(updatedData()), this, SLOT(updateData()));
	}

	void NPCListWidget::selectedNPC(const QModelIndex & idx) {
		FilterNPCsWithoutDialogModel * model = dynamic_cast<FilterNPCsWithoutDialogModel *>(_tableView->model());
		assert(model);
		emit selectNPC(_sourceModel->item(model->mapToSource(idx).row(), 0)->text());
	}

	void NPCListWidget::updateData() {
		plugins::DialogPluginInterface * plugin = dynamic_cast<plugins::DialogPluginInterface *>(sender());

		_npcList.clear();

		assert(_sourceModel);
		QStringList list;
		auto npcList = plugin->getNPCs();
		for (rpg::npc::NPC * npc : npcList) {
			list.append(QString::fromStdString(npc->getIdentifier()));
		}
		_sourceModel->removeRows(0, _sourceModel->rowCount());
		qSort(list);
		auto dlgList = plugin->getDialogs();
		std::map<QString, int> counter;
		std::map<QString, int> dialogCounter;
		for (size_t i = 0; i < dlgList.size(); i++) {
			auto vec = std::get<1>(dlgList[i]);
			for (auto t : vec) {
				counter[QString::fromStdString(std::get<0>(t))]++;
			}
			rpg::dialog::Dialog * dlg = std::get<0>(dlgList[i]);
			for (std::string s : dlg->participants) {
				dialogCounter[QString::fromStdString(s)]++;
			}
		}
		for (int i = 0; i < list.size(); i++) {
			_sourceModel->setItem(i, 0, new QStandardItem(list[i]));
			_sourceModel->setItem(i, 1, new QStandardItem(QString::number(dialogCounter[list[i]])));
			_sourceModel->setItem(i, 2, new QStandardItem(QString::number(counter[list[i]])));
			_tableView->setRowHeight(i, 20);
		}
		_npcList.insert(_npcList.begin(), npcList.begin(), npcList.end());
		std::sort(_npcList.begin(), _npcList.end(), [](rpg::npc::NPC * a, rpg::npc::NPC * b) {
			return a->getIdentifier() < b->getIdentifier();
		});
		_tableView->resizeColumnsToContents();
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
