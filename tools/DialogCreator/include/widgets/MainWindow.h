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

#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

namespace i6e {
namespace tools {
namespace common {
	class AboutDialog;
} /* namespace common */
} /* namespace tools */
namespace dialogCreator {
namespace plugins {
	class ScriptLanguagePluginInterface;
} /* namespace plugins */
namespace widgets {

	class DialogHeaderWidget;
	class DialogListWidget;
	class NpcListWidget;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	private slots:
		void closeEditor();
		void showAboutDialog();

	private:
		DialogListWidget * _dialogListWidget;
		NpcListWidget * _npcListWidget;
		DialogHeaderWidget * _dialogHeaderWidget;
		std::vector<plugins::ScriptLanguagePluginInterface *> _scriptLanguagePlugins;
		tools::common::AboutDialog * _aboutDialog;

		void closeEvent(QCloseEvent * evt) override;
		void loadScriptLanguagePlugins();
	};

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_MAINWINDOW_H__ */
