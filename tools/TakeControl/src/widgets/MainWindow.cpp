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

#include "i6engine/i6engineBuildSettings.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>

namespace i6e {
namespace dialogCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _aboutDialog(new tools::common::AboutDialog(this)) {
		setWindowIcon(QIcon(":/icon.png"));

		setWindowTitle(QString("TakeControl (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QMenu * helpMenu = new QMenu(QApplication::tr("Help"), this);
		menuBar()->addMenu(helpMenu);
		QAction * aboutAction = new QAction(QApplication::tr("About"), helpMenu);
		helpMenu->addAction(aboutAction);

		connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
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
} /* namespace dialogCreator */
} /* namespace i6e */
