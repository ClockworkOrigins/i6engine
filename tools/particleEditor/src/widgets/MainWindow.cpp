#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/WidgetParticleList.h"
#include "widgets/WidgetRender.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _renderWidget(new WidgetRender(this)), _particleListWidget(new WidgetParticleList(this)) {
		setupUi(this);

		showMaximized();

		setWindowTitle(QString("ParticleEditor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		gridLayout->addWidget(_particleListWidget, 0, 0);
		gridLayout->addWidget(_renderWidget, 0, 1);

		gridLayout->setColumnStretch(0, 1);
		gridLayout->setColumnStretch(1, 3);
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
