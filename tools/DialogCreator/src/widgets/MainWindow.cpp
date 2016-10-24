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

#include "i6engine/i6engineBuildSettings.h"

#include "plugins/ScriptLanguagePluginInterface.h"

#include "widgets/DialogHeaderWidget.h"
#include "widgets/DialogListWidget.h"
#include "widgets/NpcListWidget.h"
#include "widgets/ScriptTabWidget.h"

#include "widgets/AboutDialog.h"

#include <QCloseEvent>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>

namespace i6e {
namespace dialogCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _dialogListWidget(new DialogListWidget(this)), _npcListWidget(new NpcListWidget(this)), _dialogHeaderWidget(new DialogHeaderWidget(this)), _scriptLanguagePlugins(), _aboutDialog(new tools::common::AboutDialog(this)) {
		setupUi(this);

		setWindowIcon(QIcon(":/icon.png"));

		showMaximized();

		setWindowTitle(QString("DialogCreator (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QWidget * widget = new QWidget(this);
		QVBoxLayout * layout = new QVBoxLayout(widget);
		widget->setLayout(layout);
		layout->addWidget(_dialogListWidget);
		layout->addWidget(_npcListWidget);

		QTabWidget * tabWidget = new QTabWidget(this);
		tabWidget->addTab(_dialogHeaderWidget, "Header");
		
		gridLayout->addWidget(widget, 0, 0);
		gridLayout->addWidget(tabWidget, 0, 1);

		gridLayout->setColumnStretch(0, 1);
		gridLayout->setColumnStretch(1, 3);

		_dialogHeaderWidget->setDialogDirectory(_dialogListWidget->getDialogDirectory());

		connect(_dialogListWidget, SIGNAL(selectDialog(QString)), _dialogHeaderWidget, SLOT(showDialog(QString)));
		connect(_npcListWidget, SIGNAL(selectDialog(QString)), _dialogHeaderWidget, SLOT(showDialog(QString)));

		connect(_dialogHeaderWidget, SIGNAL(refreshLists()), _dialogListWidget, SLOT(refreshLists()));
		connect(_dialogHeaderWidget, SIGNAL(refreshLists()), _npcListWidget, SLOT(refreshLists()));

		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("i6engine.ini")) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Error during startup!"));
			box.setInformativeText(QApplication::tr("i6engine.ini not found!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}

		loadScriptLanguagePlugins();

		for (plugins::ScriptLanguagePluginInterface * slpi : _scriptLanguagePlugins) {
			std::string path;
			if (clockUtils::ClockError::SUCCESS != iniParser.getValue("SCRIPT", slpi->getScriptLanguageEntry().toStdString(), path)) {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Entry not found!"));
				box.setInformativeText(QApplication::tr("ScriptPath for plugin not found in i6engine.ini!"));
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
				return;
			}
			slpi->parseScripts(QString::fromStdString(path));
			ScriptTabWidget * scriptTab = new ScriptTabWidget(tabWidget, slpi);
			tabWidget->addTab(scriptTab, slpi->getName());
		}

		menuFile->setTitle(QApplication::tr("File"));
		actionExit->setText(QApplication::tr("Exit"));
		actionExit->setIcon(QIcon(":/images/close.png"));

		QMenu * helpMenu = new QMenu(QApplication::tr("Help"), this);
		mainMenuBar->addMenu(helpMenu);
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

	void MainWindow::loadScriptLanguagePlugins() {
		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/dialogCreator/scriptLanguages");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_scriptLanguagePlugins.push_back(qobject_cast<plugins::ScriptLanguagePluginInterface *>(plugin));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */
