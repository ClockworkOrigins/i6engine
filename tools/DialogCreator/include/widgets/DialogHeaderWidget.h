#ifndef __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGHEADERWIDGET_H__
#define __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGHEADERWIDGET_H__

#include "ui_widgetDialogHeader.h"

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	class DialogHeaderWidget : public QWidget, public Ui::dialogHeaderWidget {
		Q_OBJECT

	public:
		DialogHeaderWidget(QWidget * par = nullptr);
		~DialogHeaderWidget();

	public slots:
		void showDialog(QString identifier);

	private slots:
		void updatedParticipants();

	private:
		std::vector<QLineEdit *> _participants;
	};

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */

#endif /* __I6ENGINE_DIALOGCREATOR_WIDGETS_DIALOGHEADERWIDGET_H__ */
