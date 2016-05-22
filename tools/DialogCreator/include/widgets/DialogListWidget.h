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

#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGLISTWIDGET_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGLISTWIDGET_H__

#include "ui_widgetDialogList.h"

#include <set>

#include "clockUtils/iniParser/iniParser.h"

namespace i6e {
namespace dialogCreator {
namespace widgets {

	class DialogListWidget : public QWidget, public Ui::dialogListWidget {
		Q_OBJECT

	public:
		DialogListWidget(QWidget * par = nullptr);
		~DialogListWidget();

		std::string getDialogDirectory() const {
			return _dialogDirectory;
		}

	signals:
		void selectDialog(QString identifier);

	public slots:
		void refreshLists();

	private slots:
		void selectedDialog(QTreeWidgetItem * item);

	private:
		clockUtils::iniParser::IniParser _iniParser;
		std::set<QTreeWidgetItem *> _dialogItems;
		std::string _dialogDirectory;

		void refreshDialogList();
	};

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGLISTWIDGET_H__ */
