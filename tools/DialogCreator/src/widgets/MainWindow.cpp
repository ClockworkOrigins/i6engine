#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "plugins/ScriptLanguagePluginInterface.h"

#include "widgets/DialogHeaderWidget.h"
#include "widgets/DialogListWidget.h"
#include "widgets/NpcListWidget.h"
#include "widgets/ScriptTabWidget.h"

#include <QCloseEvent>
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _dialogListWidget(new DialogListWidget(this)), _npcListWidget(new NpcListWidget(this)), _dialogHeaderWidget(new DialogHeaderWidget(this)), _scriptLanguagePlugins() {
		setupUi(this);

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
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
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
} /* namespace i6engine */
