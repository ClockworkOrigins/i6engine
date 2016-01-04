#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/WidgetParticleList.h"
#include "widgets/WidgetRender.h"

#include <QToolBar>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _renderWidget(new WidgetRender(this)), _particleListWidget(new WidgetParticleList(this)), _tabWidget(new QTabWidget(this)) {
		setupUi(this);

		showMaximized();

		setWindowTitle(QString("ParticleEditor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QToolBar * tb = new QToolBar(this);
		QAction * playAction = tb->addAction(QIcon("../media/textures/control_play_blue.png"), "Play");

		connect(playAction, SIGNAL(triggered()), _renderWidget, SLOT(play()));

		gridLayout->addWidget(tb, 0, 0);

		QWidget * hWidget = new QWidget(this);
		QHBoxLayout * hLayout = new QHBoxLayout(hWidget);

		gridLayout->addWidget(hWidget, 1, 0);

		hLayout->addWidget(_particleListWidget);
		hLayout->addWidget(_tabWidget);

		_tabWidget->addTab(_renderWidget, "Render");

		hLayout->setStretch(0, 1);
		hLayout->setStretch(1, 3);
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
