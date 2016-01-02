#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_NPCLISTWIDGET_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_NPCLISTWIDGET_H__

#include "ui_widgetNpcList.h"

#include <set>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	class NpcListWidget : public QWidget, public Ui::npcListWidget {
		Q_OBJECT

	public:
		NpcListWidget(QWidget * par = nullptr);
		~NpcListWidget();

	signals:
		void selectDialog(QString identifier);

	public slots:
		void refreshLists();

	private slots:
		void selectedDialog(QTreeWidgetItem * item);

	private:
		std::set<QTreeWidgetItem *> _dialogItems;

		void refreshNpcList();
	};

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_NPCLISTWIDGET_H__ */
