#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/DialogHeaderWidget.h"
#include "widgets/DialogListWidget.h"
#include "widgets/NpcListWidget.h"

#include <QCloseEvent>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _dialogListWidget(new DialogListWidget(this)), _npcListWidget(new NpcListWidget(this)), _dialogHeaderWidget(new DialogHeaderWidget(this)) {
		setupUi(this);

		showMaximized();

		setWindowTitle(QString("DialogCreator (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QWidget * widget = new QWidget(this);
		QVBoxLayout * layout = new QVBoxLayout(widget);
		widget->setLayout(layout);
		layout->addWidget(_dialogListWidget);
		layout->addWidget(_npcListWidget);
		
		gridLayout->addWidget(widget, 0, 0);
		gridLayout->addWidget(_dialogHeaderWidget, 0, 1);

		gridLayout->setColumnStretch(0, 1);
		gridLayout->setColumnStretch(1, 2);

		_dialogHeaderWidget->setDialogDirectory(_dialogListWidget->getDialogDirectory());

		connect(_dialogListWidget, SIGNAL(selectDialog(QString)), _dialogHeaderWidget, SLOT(showDialog(QString)));
		connect(_npcListWidget, SIGNAL(selectDialog(QString)), _dialogHeaderWidget, SLOT(showDialog(QString)));
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
