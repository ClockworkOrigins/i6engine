/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Config.h"

#include "clockUtils/iniParser/iniParser.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

namespace i6e {
namespace tools {
namespace common {

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

} /* namespace common */
} /* namespace tools */
} /* namespace i6e */
