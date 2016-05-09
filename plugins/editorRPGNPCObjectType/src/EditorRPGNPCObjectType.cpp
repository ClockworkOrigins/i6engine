#include "EditorRPGNPCObjectType.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/npc/NPCManager.h"
#include "i6engine/rpg/npc/NPCParser.h"

#include "clockUtils/iniParser/iniParser.h"

#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>

namespace i6e {
namespace plugins {

	EditorRPGNPCObjectTypeWidget::EditorRPGNPCObjectTypeWidget(QWidget * parent) : ObjectTypeWidgetInterface(parent), _listView(new QListView(this)) {
		clockUtils::iniParser::IniParser iniParser;
		iniParser.load("RPG.ini");

		std::string NPCDirectory;
		if (iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			ISIXE_THROW_FAILURE("RPGApplication", "'npcDirectory' in section 'SCRIPT' in RPG.ini not found!");
		}
		rpg::npc::NPCParser::GetSingletonPtr()->loadNPCs(NPCDirectory);

		auto npcList = rpg::npc::NPCParser::GetSingletonPtr()->getNPCList();
		QStringList list;
		for (auto & p : npcList) {
			list.append(QString::fromStdString(p.first));
		}
		QStringListModel * model = new QStringListModel(_listView);
		model->setStringList(list);
		_listView->setModel(model);
		_listView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
		QVBoxLayout * layout = new QVBoxLayout(this);
		layout->addWidget(_listView);
		setLayout(layout);
		connect(_listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectIndex(QModelIndex)));
	}
	
	void EditorRPGNPCObjectTypeWidget::selectIndex(QModelIndex index) {
		auto cameras = i6eObjectFacade->getAllObjectsOfType("EditorCam");
		if (cameras.empty()) {
			return;
		}
		auto camera = cameras.front();

		utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = camera->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
		Vec3 newPos = ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, 5.0), ssc->getRotation());

		rpg::npc::NPCManager::GetSingletonPtr()->createNPC(_listView->model()->data(index).toString().toStdString(), newPos, ssc->getRotation(), false);

		_callback();
	}

	editor::plugins::ObjectTypeWidgetInterface * EditorRPGNPCObjectType::createWidget(QWidget * parent) {
		return new EditorRPGNPCObjectTypeWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.objectType.ObjectTypePluginInterface")
