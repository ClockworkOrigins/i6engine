#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/NPCEditWidget.h"
#include "widgets/NPCListWidget.h"

namespace i6engine {
namespace tools {
namespace npcCreator {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _npcListWidget(nullptr), _npcEditWidget(nullptr) {
		setupUi(this);

		setWindowTitle(QString("NPCCreator (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		_npcListWidget = new NPCListWidget(this);
		_npcEditWidget = new NPCEditWidget(this);

		_npcListWidget->setNPCEditWidget(_npcEditWidget);
		_npcEditWidget->setNPCListWidget(_npcListWidget);

		showMaximized();

		gridLayout->addWidget(_npcListWidget, 0, 0);
		gridLayout->addWidget(_npcEditWidget, 0, 1);

		gridLayout->setColumnStretch(0, 2);
		gridLayout->setColumnStretch(1, 9);
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeCreator() {
		qApp->exit();
	}

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6engine */
