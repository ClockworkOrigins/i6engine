#include "i6engine/editor/widgets/ObjectListWidget.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/editor/widgets/MainWindow.h"

#include <QStringListModel>

namespace i6e {
namespace editor {
namespace widgets {

	ObjectListWidget::ObjectListWidget(QWidget * par, QWidget * mainWindow) : QWidget(par), _mainWindow(dynamic_cast<MainWindow *>(mainWindow)) {
		setupUi(this);

		connect(this, SIGNAL(updateObjectList()), this, SLOT(doUpdateObjectList()));
	}

	ObjectListWidget::~ObjectListWidget() {
	}

	void ObjectListWidget::doUpdateObjectList() {
		QStringList list;
		for (auto & p : api::EngineController::GetSingleton().getObjectFacade()->getGOMap()) {
			if (p.second->getType() != "EditorCam") {
				list.append(QString::fromStdString(p.second->getType()));
			}
		}
		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

	void ObjectListWidget::selectIndex(QModelIndex index) {
		auto goMap = api::EngineController::GetSingleton().getObjectFacade()->getGOMap();
		int idx = index.row();
		for (auto it = goMap.begin(); it != goMap.end(); it++) {
			if (idx == 0) {
				emit selectObject(it->second->getID());
				break;
			}
			idx--;
		}

		_mainWindow->setFocus();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */
