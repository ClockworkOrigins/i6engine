#include "Config.h"

#include "widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char ** argv) {
	QApplication app(argc, argv);
	i6engine::tools::Config::Init();
	i6engine::tools::npcCreator::widgets::MainWindow wnd;
	wnd.show();
	return app.exec();
}
