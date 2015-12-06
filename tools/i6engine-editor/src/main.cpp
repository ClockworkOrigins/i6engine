#include "i6engine/editor/widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char ** argv) {
	QApplication app(argc, argv);
	i6engine::editor::widgets::MainWindow wnd;
	wnd.show();
	return app.exec();
}
