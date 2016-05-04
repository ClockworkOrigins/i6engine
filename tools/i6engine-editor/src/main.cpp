#include "Config.h"

#include "i6engine/editor/widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char ** argv) {
	QApplication app(argc, argv);
	i6e::tools::Config::Init();
	i6e::editor::widgets::MainWindow wnd;
	wnd.show();
	return app.exec();
}
