#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/DialogListWidget.h"
#include "widgets/NpcListWidget.h"

#include <QCloseEvent>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _dialogListWidget(new DialogListWidget(this)), _npcListWidget(new NpcListWidget(this)) {
		setupUi(this);

		showMaximized();

		setWindowTitle(QString("DialogCreator (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		gridLayout->addWidget(_dialogListWidget, 0, 0);
		gridLayout->addWidget(_npcListWidget, 1, 0);
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

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */
