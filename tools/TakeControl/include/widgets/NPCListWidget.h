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

#ifndef __I6ENGINE_TAKECONTROL_WIDGETS_NPCLISTWIDGET_H__
#define __I6ENGINE_TAKECONTROL_WIDGETS_NPCLISTWIDGET_H__

#include <QSortFilterProxyModel>
#include <QWidget>

class QLabel;
class QStandardItemModel;
class QTableView;

namespace i6e {
namespace rpg {
namespace npc {
	class NPC;
} /* namespace npc */
} /* namespace rpg */
namespace takeControl {
namespace plugins {
	class DialogPluginInterface;
} /* namespace plugins */
namespace widgets {

	class FilterNPCsWithoutDialogModel : public QSortFilterProxyModel {
		Q_OBJECT

	public:
		FilterNPCsWithoutDialogModel(QWidget * par);

		bool filterAcceptsRow(int source_row, const QModelIndex &) const override;

	signals:
		void updateCount(QString);

	public slots:
		void stateChanged(int checkState);

	private slots:
		void countChanged();

	private:
		bool _checked;
	};

	class NPCListWidget : public QWidget {
		Q_OBJECT

	public:
		NPCListWidget(QWidget * par = nullptr);
		~NPCListWidget();

	signals:
		void selectNPC(QString);

	public slots:
		void loadedDialogPlugin(plugins::DialogPluginInterface * plugin);

	private slots:
		void selectedNPC(const QModelIndex & idx);
		void updateData();

	private:
		QTableView * _tableView;
		QLabel * _amountLabel;
		QStandardItemModel * _sourceModel;
		std::vector<rpg::npc::NPC *> _npcList;
	};

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */

#endif /* __I6ENGINE_TAKECONTROL_WIDGETS_NPCLISTWIDGET_H__ */
