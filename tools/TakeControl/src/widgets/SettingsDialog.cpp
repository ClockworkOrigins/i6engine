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

#include "widgets/SettingsDialog.h"

#include "widgets/SettingsWidget.h"

#include "clockUtils/iniParser/iniParser.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QVBoxLayout>

namespace i6e {
namespace takeControl {
namespace widgets {

	SettingsDialog::SettingsDialog(QWidget * par) : QDialog(par), _tabWidget(nullptr), _iniParser(new clockUtils::iniParser::IniParser()) {
		_iniParser->load(qApp->applicationDirPath().toStdString() + "/takeControl.ini");

		setWindowTitle(QApplication::tr("Settings"));

		QScrollArea * scrollArea = new QScrollArea(this);

		QVBoxLayout * scrollLayout = new QVBoxLayout();

		_tabWidget = new QTabWidget(scrollArea);
		_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		scrollLayout->addWidget(_tabWidget);

		QDialogButtonBox * dbb = new QDialogButtonBox(QDialogButtonBox::StandardButton::Apply | QDialogButtonBox::StandardButton::Abort, scrollArea);
		QPushButton * applyButton = dbb->button(QDialogButtonBox::StandardButton::Apply);
		QPushButton * abortButton = dbb->button(QDialogButtonBox::StandardButton::Abort);

		connect(applyButton, SIGNAL(clicked()), this, SIGNAL(applySettings()));
		connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));

		connect(abortButton, SIGNAL(clicked()), this, SIGNAL(rejectSettings()));
		connect(abortButton, SIGNAL(clicked()), this, SLOT(reject()));

		scrollLayout->addWidget(dbb);
		
		scrollArea->setLayout(scrollLayout);

		QVBoxLayout * l = new QVBoxLayout();

		l->addWidget(scrollArea);
		l->addStretch();

		setLayout(l);
	}

	SettingsDialog::~SettingsDialog() {
		_iniParser->save(qApp->applicationDirPath().toStdString() + "/takeControl.ini");
	}

	void SettingsDialog::addSettingsWidget(QString tabName, SettingsWidget * widget) {
		_tabWidget->addTab(widget, tabName);
		connect(this, SIGNAL(applySettings()), widget, SLOT(saveSettings()));
		connect(this, SIGNAL(rejectSettings()), widget, SLOT(rejectChanges()));
		adjustSize();
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
