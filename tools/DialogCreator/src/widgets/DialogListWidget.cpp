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

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include <QMessageBox>

namespace i6e {
namespace dialogCreator {
namespace widgets {

	DialogListWidget::DialogListWidget(QWidget * par) : QWidget(par), _iniParser(), _dialogItems(), _dialogDirectory() {
		setupUi(this);

		if (clockUtils::ClockError::SUCCESS != _iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Error during startup!"));
			box.setInformativeText(QApplication::tr("RPG.ini not found!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}
		if (_iniParser.getValue("SCRIPT", "dialogDirectory", _dialogDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Error during startup!"));
			box.setInformativeText(QApplication::tr("'dialogDirectory' in section 'SCRIPT' in RPG.ini not found!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}

		refreshDialogList();
	}

	DialogListWidget::~DialogListWidget() {
	}

	void DialogListWidget::refreshLists() {
		refreshDialogList();
	}

	void DialogListWidget::selectedDialog(QTreeWidgetItem * item) {
		auto it = _dialogItems.find(item);
		if (it != _dialogItems.end()) {
			emit selectDialog(item->text(0));
		}
	}

	void DialogListWidget::refreshDialogList() {
		rpg::dialog::DialogManager::GetSingletonPtr()->loadDialogs(_dialogDirectory);

		treeWidget->clear();
		_dialogItems.clear();

		std::map<std::string, rpg::dialog::Dialog *> dialogs = rpg::dialog::DialogManager::GetSingletonPtr()->getDialogs();

		for (const auto & p : dialogs) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(p.first) });
			treeWidget->addTopLevelItem(twi);
			_dialogItems.insert(twi);

			for (std::string participant : p.second->participants) {
				twi->addChild(new QTreeWidgetItem(twi, { QString::fromStdString(participant) }));
			}
		}
		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectedDialog(QTreeWidgetItem *)));
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */
