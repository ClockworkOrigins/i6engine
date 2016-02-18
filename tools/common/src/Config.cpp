#include "Config.h"

#include "clockUtils/iniParser/iniParser.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

namespace i6engine {
namespace tools {

	void Config::Init() {
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load(qApp->applicationDirPath().toStdString() + "/i6engine.ini");
		std::string language;
		if (iniParser.getValue("GENERAL", "language", language) != clockUtils::ClockError::SUCCESS) {
			QLocale locale = QLocale::system();
			if (locale.language() == QLocale::Language::German) {
				language = "Deutsch";
			} else {
				language = "English";
			}
			iniParser.setValue("GENERAL", "language", language);
			iniParser.save(qApp->applicationDirPath().toStdString() + "/i6engine.ini");
		}
		QTranslator * translator = new QTranslator(qApp);
		if (language == "Deutsch") {
			QLocale::setDefault(QLocale("de_DE"));
			translator->load(qApp->applicationDirPath() + "/de_DE");
		} else {
			QLocale::setDefault(QLocale("en_US"));
			translator->load(qApp->applicationDirPath() + "/en_US");
		}
		qApp->installTranslator(translator);
	}

} /* namespace tools */
} /* namespace i6engine */
