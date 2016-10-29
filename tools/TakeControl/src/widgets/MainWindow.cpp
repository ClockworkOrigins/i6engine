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

#include "widgets/MainWindow.h"

#include "widgets/AboutDialog.h"
#include "widgets/DialogListWidget.h"
#include "widgets/InfoWidget.h"
#include "widgets/NPCListWidget.h"

#include "i6engine/i6engineBuildSettings.h"

#include <QApplication>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>

namespace i6e {
namespace takeControl {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _aboutDialog(new tools::common::AboutDialog(this)), _npcListWidget(new NPCListWidget(this)), _dialogListWidget(new DialogListWidget(this)), _infoWidget(new InfoWidget(this)) {
		setWindowIcon(QIcon(":/icon.png"));

		setWindowTitle(QString("TakeControl (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QMenu * helpMenu = new QMenu(QApplication::tr("Help"), this);
		menuBar()->addMenu(helpMenu);
		QAction * aboutAction = new QAction(QApplication::tr("About"), helpMenu);
		helpMenu->addAction(aboutAction);

		connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

		QWidget * w = new QWidget(this);
		QHBoxLayout * l = new QHBoxLayout();

		l->addWidget(_npcListWidget);

		QWidget * rightWidget = new QWidget(this);
		QVBoxLayout * rightLayout = new QVBoxLayout();

		rightLayout->addWidget(_dialogListWidget);
		rightLayout->addWidget(_infoWidget);

		rightWidget->setLayout(rightLayout);

		l->addWidget(rightWidget);

		w->setLayout(l);

		l->setStretch(0, 1);
		l->setStretch(1, 3);

		setCentralWidget(w);
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
	}

	void MainWindow::showAboutDialog() {
		_aboutDialog->show();
	}

	void MainWindow::closeEvent(QCloseEvent * evt) {
		closeEditor();
		evt->ignore();
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
