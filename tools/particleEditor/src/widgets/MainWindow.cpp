#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/WidgetParticleList.h"
#include "widgets/WidgetRender.h"

#include <QToolBar>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _renderWidget(new WidgetRender(this)), _particleListWidget(new WidgetParticleList(this)), _tabWidget(new QTabWidget(this)), _playing(false), _toolbarActions() {
		setupUi(this);

		showMaximized();

		setWindowTitle(QString("ParticleEditor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QToolBar * tb = new QToolBar(this);
		QAction * playAction = tb->addAction(QIcon("../media/textures/control_play.png"), "Play");
		QAction * pauseAction = tb->addAction(QIcon("../media/textures/control_pause.png"), "Pause");
		QAction * stopAction = tb->addAction(QIcon("../media/textures/control_stop.png"), "Stop");

		connect(playAction, SIGNAL(triggered()), this, SLOT(handlePlayAction()));
		connect(this, SIGNAL(triggerPlay()), _renderWidget, SLOT(play()));
		connect(pauseAction, SIGNAL(triggered()), this, SLOT(handlePauseAction()));
		connect(this, SIGNAL(triggerPause()), _renderWidget, SLOT(pause()));
		connect(stopAction, SIGNAL(triggered()), this, SLOT(handleStopAction()));
		connect(this, SIGNAL(triggerStop()), _renderWidget, SLOT(stop()));

		connect(_particleListWidget, SIGNAL(createNewSystem(const QString &)), this, SLOT(createNewSystem(const QString &)));
		connect(this, SIGNAL(triggerCreateNewSystem(const QString &)), _renderWidget, SLOT(createNewSystem(const QString &)));

		gridLayout->addWidget(tb, 0, 0);

		QWidget * hWidget = new QWidget(this);
		QHBoxLayout * hLayout = new QHBoxLayout(hWidget);

		gridLayout->addWidget(hWidget, 1, 0);

		hLayout->addWidget(_particleListWidget);
		hLayout->addWidget(_tabWidget);

		_tabWidget->addTab(_renderWidget, "Render");

		hLayout->setStretch(0, 1);
		hLayout->setStretch(1, 3);

		_toolbarActions.insert(std::make_pair("Play", playAction));
		_toolbarActions.insert(std::make_pair("Pause", pauseAction));
		_toolbarActions.insert(std::make_pair("Stop", stopAction));
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
	}

	void MainWindow::handlePlayAction() {
		if (_playing || _currentParticleTemplate.size() == 0) {
			return;
		}
		_playing = true;
		_toolbarActions["Play"]->setIcon(QIcon("../media/textures/control_play.png"));
		_toolbarActions["Pause"]->setIcon(QIcon("../media/textures/control_pause_blue.png"));
		_toolbarActions["Stop"]->setIcon(QIcon("../media/textures/control_stop_blue.png"));
		emit triggerPlay();
	}

	void MainWindow::handlePauseAction() {
		if (!_playing) {
			return;
		}
		_playing = false;
		_toolbarActions["Play"]->setIcon(QIcon("../media/textures/control_play_blue.png"));
		_toolbarActions["Pause"]->setIcon(QIcon("../media/textures/control_pause.png"));
		_toolbarActions["Stop"]->setIcon(QIcon("../media/textures/control_stop.png"));
		emit triggerPause();
	}

	void MainWindow::handleStopAction() {
		if (!_playing) {
			return;
		}
		_playing = false;
		_toolbarActions["Play"]->setIcon(QIcon("../media/textures/control_play_blue.png"));
		_toolbarActions["Pause"]->setIcon(QIcon("../media/textures/control_pause.png"));
		_toolbarActions["Stop"]->setIcon(QIcon("../media/textures/control_stop.png"));
		emit triggerStop();
	}

	void MainWindow::createNewSystem(const QString & particle) {
		if (_currentParticleTemplate.size() == 0) {
			_toolbarActions["Play"]->setIcon(QIcon("../media/textures/control_play_blue.png"));
		}
		_currentParticleTemplate = particle;
		emit triggerCreateNewSystem(particle);
		if (_playing) {
			emit triggerPlay();
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
