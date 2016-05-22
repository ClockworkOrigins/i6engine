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

#include "i6engine/rpg/npc/NPCParser.h"

#include "widgets/NPCEditWidget.h"

#include <QMessageBox>
#include <QStringListModel>

namespace i6e {
namespace tools {
namespace npcCreator {
namespace widgets {

	NPCListWidget::NPCListWidget(QWidget * par) : QWidget(par), _iniParser(), _npcFileList(), _filteredNpcFileList(), _filter(), _npcEditWidget(nullptr) {
		setupUi(this);

		if (clockUtils::ClockError::SUCCESS != _iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setText(QApplication::tr("RPG.ini not found!"));
			box.exec();
		}

		refreshNPCList();

		filterLabel->setText(QApplication::tr("Filter"));
	}

	NPCListWidget::~NPCListWidget() {
	}

	void NPCListWidget::selectIndex(QModelIndex index) {
		int idx = index.row();
		_npcEditWidget->parseNPC(_filteredNpcFileList, idx);
	}

	void NPCListWidget::changedFilter(QString filter) {
		_filter = filter;
		refreshNPCList();
	}

	void NPCListWidget::refreshNPCList() {
		std::string NPCDirectory;
		if (_iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setText(QApplication::tr("No entry for npcDirectory in RPG.ini!"));
			box.exec();
		}
		rpg::npc::NPCParser::GetSingleton().loadNPCs(NPCDirectory);
		_npcFileList = rpg::npc::NPCParser::GetSingleton().getNPCList();

		std::sort(_npcFileList.begin(), _npcFileList.end(), [](const std::pair<std::string, std::string> & a, const std::pair<std::string, std::string> & b) {
			return a.first < b.first;
		});

		_filteredNpcFileList.clear();

		for (auto & p : _npcFileList) {
			if (QString::fromStdString(p.first).contains(QRegExp(_filter))) {
				_filteredNpcFileList.push_back(p);
			}
		}

		QStringList list;
		for (auto & p : _filteredNpcFileList) {
			list.append(QString::fromStdString(p.first));
		}
		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6e */
