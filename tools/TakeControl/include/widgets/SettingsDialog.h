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

#ifndef __I6ENGINE_TAKECONTROL_WIDGETS_SETTINGSDIALOG_H__
#define __I6ENGINE_TAKECONTROL_WIDGETS_SETTINGSDIALOG_H__

#include <QDialog>

class QTabWidget;

namespace clockUtils {
namespace iniParser {
	class IniParser;
} /* namespace iniParser */
} /* namespace clockUtils */

namespace i6e {
namespace takeControl {
namespace widgets {

	class SettingsWidget;

	class SettingsDialog : public QDialog {
		Q_OBJECT

	public:
		SettingsDialog(QWidget * par);
		~SettingsDialog();

		clockUtils::iniParser::IniParser * getIniParser() const {
			return _iniParser;
		}

		void addSettingsWidget(QString tabName, SettingsWidget * widget);

	signals:
		void applySettings();
		void rejectSettings();

	private:
		QTabWidget * _tabWidget;
		clockUtils::iniParser::IniParser * _iniParser;
	};

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */

#endif /* __I6ENGINE_TAKECONTROL_WIDGETS_SETTINGSDIALOG_H__ */
