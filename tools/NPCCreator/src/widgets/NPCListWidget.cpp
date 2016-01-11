#include "widgets/NPCListWidget.h"

#include "i6engine/rpg/npc/NPCParser.h"

#include "widgets/NPCEditWidget.h"

#include <QMessageBox>
#include <QStringListModel>

namespace i6engine {
namespace tools {
namespace npcCreator {
namespace widgets {

	NPCListWidget::NPCListWidget(QWidget * par) : QWidget(par), _iniParser(), _npcFileList(), _npcEditWidget(nullptr) {
		setupUi(this);

		if (clockUtils::ClockError::SUCCESS != _iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setText(QString("RPG.ini not found!"));
			box.exec();
		}

		refreshNPCList();
	}

	NPCListWidget::~NPCListWidget() {
	}

	void NPCListWidget::selectIndex(QModelIndex index) {
		int idx = index.row();
		_npcEditWidget->parseNPC(_npcFileList, idx);
	}

	void NPCListWidget::refreshNPCList() {
		std::string NPCDirectory;
		if (_iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setText(QString("No entry for npcDirectory in RPG.ini!"));
			box.exec();
		}
		rpg::npc::NPCParser::GetSingleton().loadNPCs(NPCDirectory);
		_npcFileList = rpg::npc::NPCParser::GetSingleton().getNPCList();

		std::sort(_npcFileList.begin(), _npcFileList.end(), [](const std::pair<std::string, std::string> & a, const std::pair<std::string, std::string> & b) {
			return a.first < b.first;
		});

		QStringList list;
		for (auto & p : _npcFileList) {
			list.append(QString::fromStdString(p.first));
		}
		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6engine */
