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

#ifndef __I6ENGINE_TAKECONTROL_WIDGETS_DIALOGLISTWIDGET_H__
#define __I6ENGINE_TAKECONTROL_WIDGETS_DIALOGLISTWIDGET_H__

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

namespace i6e {
namespace rpg {
namespace dialog {
	struct Dialog;
} /* namespace dialog */
} /* namespace rpg */
namespace takeControl {
namespace plugins {
	class DialogPluginInterface;
} /* namespace plugins */
namespace widgets {

	class DialogListWidget : public QWidget {
		Q_OBJECT

	public:
		DialogListWidget(QWidget * par = nullptr);
		~DialogListWidget();

	signals:
		void selectDialog(QString);
		void selectNPC(QString);

	public slots:
		void loadedDialogPlugin(plugins::DialogPluginInterface * plugin);
		void selectedNPC(QString identifier);

	private slots:
		void selectedDialog(QTreeWidgetItem * item, int column);
		void updateData();

	private:
		QTreeWidget * _treeWidget;
		std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>, plugins::DialogPluginInterface *>> _dialogList;
		std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>, plugins::DialogPluginInterface *>> _activeDialogList;
		QString _npcIdentifier;
	};

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */

#endif /* __I6ENGINE_TAKECONTROL_WIDGETS_DIALOGLISTWIDGET_H__ */
