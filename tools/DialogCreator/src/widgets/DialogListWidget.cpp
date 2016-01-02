#include "widgets/DialogListWidget.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include <QMessageBox>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	DialogListWidget::DialogListWidget(QWidget * par) : QWidget(par), _iniParser(), _dialogItems(), _dialogDirectory() {
		setupUi(this);

		if (clockUtils::ClockError::SUCCESS != _iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setWindowTitle(QString("Error during startup!"));
			box.setInformativeText("RPG.ini not found!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}
		if (_iniParser.getValue("SCRIPT", "dialogDirectory", _dialogDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setWindowTitle(QString("Error during startup!"));
			box.setInformativeText("'dialogDirectory' in section 'SCRIPT' in RPG.ini not found!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}

		refreshDialogList();
	}

	DialogListWidget::~DialogListWidget() {
	}

	void DialogListWidget::refreshLists() {
		refreshDialogList();
	}

	void DialogListWidget::selectedDialog(QTreeWidgetItem * item) {
		auto it = _dialogItems.find(item);
		if (it != _dialogItems.end()) {
			emit selectDialog(item->text(0));
		}
	}

	void DialogListWidget::refreshDialogList() {
		rpg::dialog::DialogManager::GetSingletonPtr()->loadDialogs(_dialogDirectory);

		treeWidget->clear();
		_dialogItems.clear();

		std::map<std::string, rpg::dialog::Dialog *> dialogs = rpg::dialog::DialogManager::GetSingletonPtr()->getDialogs();

		for (const auto & p : dialogs) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(p.first) });
			treeWidget->addTopLevelItem(twi);
			_dialogItems.insert(twi);

			for (std::string participant : p.second->participants) {
				twi->addChild(new QTreeWidgetItem(twi, { QString::fromStdString(participant) }));
			}
		}
		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectedDialog(QTreeWidgetItem *)));
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */
