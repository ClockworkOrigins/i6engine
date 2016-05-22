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

#ifndef __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCLISTWIDGET_H__
#define __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCLISTWIDGET_H__

#include "ui_widgetNPCList.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6e {
namespace tools {
namespace npcCreator {
namespace widgets {

	class NPCEditWidget;

	class NPCListWidget : public QWidget, public Ui::npcListWidget {
		Q_OBJECT

	public:
		NPCListWidget(QWidget * par = nullptr);
		~NPCListWidget();

		void setNPCEditWidget(NPCEditWidget * npcEditWidget) {
			_npcEditWidget = npcEditWidget;
		}

		void refreshNPCList();

	private slots:
		void selectIndex(QModelIndex index);
		void changedFilter(QString filter);

	private:
		clockUtils::iniParser::IniParser _iniParser;
		std::vector<std::pair<std::string, std::string>> _npcFileList;
		std::vector<std::pair<std::string, std::string>> _filteredNpcFileList;
		QString _filter;
		NPCEditWidget * _npcEditWidget;
	};

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_NPCCREATOR_WIDGETS_NPCLISTWIDGET_H__ */
