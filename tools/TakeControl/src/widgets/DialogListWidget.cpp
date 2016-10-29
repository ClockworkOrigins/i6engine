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

#include <QTreeWidget>
#include <QVBoxLayout>

namespace i6e {
namespace takeControl {
namespace widgets {

	DialogListWidget::DialogListWidget(QWidget * par) : QWidget(par), _treeWidget(new QTreeWidget(this)) {
		QVBoxLayout * l = new QVBoxLayout();
		l->addWidget(_treeWidget);
		setLayout(l);

		_treeWidget->clear();
		_treeWidget->setHeaderHidden(true);

		for (int i = 0; i < 10; i++) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(_treeWidget, { "Dialog_" + QString::number(i) });
			_treeWidget->addTopLevelItem(twi);

			for (int j = 0; j < 5; j++) {
				twi->addChild(new QTreeWidgetItem(twi, { "This is sentence " + QString::number(j) }));
			}
		}
	}

	DialogListWidget::~DialogListWidget() {
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
