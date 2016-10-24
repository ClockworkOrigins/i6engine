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

#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_MAINWINDOW_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_MAINWINDOW_H__

#include "ui_wndMainWindow.h"

#include "i6engine/rpg/RPGApplication.h"

namespace i6e {
namespace tools {
namespace common {
	class AboutDialog;
} /* namespace common */
namespace npcCreator {
namespace widgets {

	class NPCEditWidget;
	class NPCListWidget;
	class NPCRenderWidget;

	class MainWindow : public QMainWindow, public Ui::wndMainWindow, public rpg::RPGApplication {
		Q_OBJECT

	public:
		MainWindow(QMainWindow * par = nullptr);
		~MainWindow();

	private slots:
		void closeCreator();
		void showAboutDialog();

	private:
		NPCListWidget * _npcListWidget;
		NPCEditWidget * _npcEditWidget;
		NPCRenderWidget * _npcRenderWidget;
		common::AboutDialog * _aboutDialog;

		void closeEvent(QCloseEvent * evt) override;

		void AfterInitialize() override;

		void Finalize() override;
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_MAINWINDOW_H__ */
