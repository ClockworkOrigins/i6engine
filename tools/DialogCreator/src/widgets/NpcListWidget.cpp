#include "widgets/NpcListWidget.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include <QMessageBox>

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	NpcListWidget::NpcListWidget(QWidget * par) : QWidget(par) {
		setupUi(this);

		refreshNpcList();
	}

	NpcListWidget::~NpcListWidget() {
	}

	void NpcListWidget::refreshNpcList() {
		treeWidget->clear();

		std::map<std::string, std::vector<rpg::dialog::Dialog *>> npcDialogs = rpg::dialog::DialogManager::GetSingletonPtr()->getNpcDialogs();

		for (const auto & p : npcDialogs) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(p.first) });
			treeWidget->addTopLevelItem(twi);
			std::vector<rpg::dialog::Dialog *> dialogs = p.second;
			std::sort(dialogs.begin(), dialogs.end(), [](rpg::dialog::Dialog * a, rpg::dialog::Dialog * b) {
				return a->nr < b->nr;
			});
			for (rpg::dialog::Dialog * d : dialogs) {
				twi->addChild(new QTreeWidgetItem(twi, { QString::fromStdString(d->identifier) }));
			}
		}
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */
