#include "widgets/MainWindow.h"

#include <QApplication>

int main(int argc, char ** argv) {
	QApplication app(argc, argv);
	i6engine::dialogCreator::widgets::MainWindow wnd;
	wnd.show();
	return app.exec();
}
