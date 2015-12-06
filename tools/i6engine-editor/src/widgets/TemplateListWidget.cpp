#include "i6engine/editor/widgets/TemplateListWidget.h"

#include <thread>

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/components/StaticStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/editor/widgets/MainWindow.h"

#include <QStringListModel>

namespace i6engine {
namespace editor {
namespace widgets {

	TemplateListWidget::TemplateListWidget(QWidget * par) : QWidget(par), _editor(dynamic_cast<MainWindow *>(par)) {
		setupUi(this);

		connect(this, SIGNAL(loadTemplates()), this, SLOT(doLoadTemplates()));
	}

	TemplateListWidget::~TemplateListWidget() {
	}

	void TemplateListWidget::doLoadTemplates() {
		QStringList list;
		for (auto & p : api::EngineController::GetSingleton().getObjectFacade()->getGOTemplates()) {
			list.append(QString::fromStdString(p.first));
		}
		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

	void TemplateListWidget::selectIndex(QModelIndex index) {
		if (_editor->getCamera() == nullptr) {
			return;
		}
		auto tmpl = api::EngineController::GetSingleton().getObjectFacade()->getGOTemplates()[listView->model()->data(index).toString().toStdString()];

		for (auto & c : tmpl._components) {
			if (c._template == "PhysicalState" || c._template == "StaticState") {
				utils::sharedPtr<api::StaticStateComponent, api::Component> ssc = _editor->getCamera()->getGOC<api::StaticStateComponent>(api::components::StaticStateComponent);
				Vec3 newPos = ssc->getPosition() + math::rotateVector(Vec3(0.0, 0.0, 5.0), ssc->getRotation());
				newPos.insertInMap("pos", c._params);
			}
		}

		api::EngineController::GetSingleton().getObjectFacade()->createObject(listView->model()->data(index).toString().toStdString(), tmpl, api::EngineController::GetSingleton().getUUID(), false);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		dynamic_cast<MainWindow *>(_editor)->updateObjectList();

		dynamic_cast<MainWindow *>(_editor)->setFocus();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
