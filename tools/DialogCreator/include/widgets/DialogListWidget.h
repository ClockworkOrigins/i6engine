#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGLISTWIDGET_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGLISTWIDGET_H__

#include "ui_widgetDialogList.h"

#include <set>

#include "clockUtils/iniParser/iniParser.h"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGLISTWIDGET_H__ */
