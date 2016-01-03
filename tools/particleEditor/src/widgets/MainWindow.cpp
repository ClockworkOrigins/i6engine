#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par) {
		setupUi(this);

		showMaximized();

		setWindowTitle(QString("ParticleEditor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
