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

#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCEDITWIDGET_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCEDITWIDGET_H__

#include "ui_widgetNPCEdit.h"

namespace i6e {
namespace tools {
namespace npcCreator {
namespace widgets {

	class MainWindow;
	class NPCListWidget;

	class NPCEditWidget : public QWidget, public Ui::npcEditWidget {
		Q_OBJECT

	public:
		NPCEditWidget(QWidget * par = nullptr);
		~NPCEditWidget();

		void parseNPC(const std::vector<std::pair<std::string, std::string>> & npcFileList, int index);

		void setNPCListWidget(NPCListWidget * npcListWidget) {
			_npcListWidget = npcListWidget;
		}

	private slots:
		void changedIdentifier();
		void changedEntry();
		void saveNewNPC();
		void saveChanges();
		void deleteNPC();
		void selectModel();

	private:
		MainWindow * _parent;
		NPCListWidget * _npcListWidget;
		bool _changed;
		bool _changedIdentifier;
		QString _windowTitle;
		QString _currentIdentifier;
		std::string _currentFile;
		std::vector<std::pair<std::string, std::string>> _npcFileList;

		void saveToFile(const std::string & file);
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCEDITWIDGET_H__ */
