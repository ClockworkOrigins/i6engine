#include "i6engine/editor/widgets/ObjectInfoWidget.h"

#include <thread>

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/WaynetManager.h"
#include "i6engine/api/objects/GameObject.h"

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

namespace i6engine {
namespace editor {
namespace widgets {

	ObjectInfoWidget::ObjectInfoWidget(QWidget * par) : QWidget(par), _selectedObjectID(-1), _infos(), _entries() {
		setupUi(this);

		connect(this, SIGNAL(selectObject(int64_t)), this, SLOT(doSelectObject(int64_t)));
		connect(this, SIGNAL(removeObject()), this, SLOT(doRemoveObject()));

		hide();
	}

	ObjectInfoWidget::~ObjectInfoWidget() {
	}

	void ObjectInfoWidget::doSelectObject(int64_t id) {
		if (_selectedObjectID != -1) {
			for (QWidget * w : _infos) {
				verticalLayout->removeWidget(w);
				w->deleteLater();
			}
			_infos.clear();
			_entries.clear();
			auto go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
			if (go != nullptr) {
				auto mc = go->getGOC<api::MeshAppearanceComponent>(api::components::ComponentTypes::MeshAppearanceComponent);
				if (mc != nullptr) {
					mc->removeBoundingBox();
				}
			}
		}
		_selectedObjectID = id;

		if (_selectedObjectID != -1) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(id);
			api::WeakGOPtr wgo = go;

			QLabel * l = new QLabel(QString::fromStdString(go->getType()), this);
			verticalLayout->addWidget(l);
			_infos.push_back(l);

			QWidget * w = new QWidget(this);
			QHBoxLayout * layout = new QHBoxLayout(w);
			l = new QLabel(QString("Flags"), w);
			QLineEdit * edit = new QLineEdit(w);
			std::vector<std::string> flags = go->getFlags();
			std::string flagString;
			for (size_t i = 0; i < flags.size(); i++) {
				flagString += flags[i];

				if (i < flags.size() - 1) {
					flagString += "|";
				}
			}
			edit->setText(QString::fromStdString(flagString));
			layout->addWidget(l);
			layout->addWidget(edit);
			verticalLayout->addWidget(w);
			_infos.push_back(w);
			_entries.insert(std::make_pair(edit, [wgo](std::string s) {
				auto gobj = wgo.get();
				gobj->setFlags(utils::split(s, "|"));
			}));

			for (auto c : go->getGOCList()) {
				if (go->getType() == "Waypoint" && (c->getTemplateName() == "MeshAppearance" || c->getTemplateName() == "MovableText")) {
					continue;
				}
				QGroupBox * gb = new QGroupBox(QString::fromStdString(c->getTemplateName()), this);
				verticalLayout->addWidget(gb);
				QVBoxLayout * vLayout = new QVBoxLayout(gb);
				gb->setLayout(vLayout);
				l = new QLabel(QString::fromStdString(c->getIdentifier()), gb);
				vLayout->addWidget(l);
				_infos.push_back(gb);
				for (auto option : c->getComponentOptions()) {
					w = new QWidget(gb);
					layout = new QHBoxLayout(w);
					w->setLayout(layout);
					vLayout->addWidget(w);
					l = new QLabel(QString::fromStdString(std::get<api::ComponentOptionsParameter::NAME>(option)), w);
					layout->addWidget(l);
					edit = new QLineEdit(QString::fromStdString(std::get<api::ComponentOptionsParameter::READFUNC>(option)()), w);
					layout->addWidget(edit);
					_entries.insert(std::make_pair(edit, std::get<api::ComponentOptionsParameter::WRITEFUNC>(option)));
					edit->setReadOnly(std::get<api::ComponentOptionsParameter::ACCESSSTATE>(option) == api::AccessState::READONLY);
				}
			}

			QPushButton * button = new QPushButton(QString("Apply"), this);
			verticalLayout->addWidget(button);
			_infos.push_back(button);
			connect(button, SIGNAL(clicked()), this, SLOT(applyChanges()));

			auto mc = go->getGOC<api::MeshAppearanceComponent>(api::components::ComponentTypes::MeshAppearanceComponent);
			if (mc != nullptr) {
				mc->drawBoundingBox(Vec3(1.0, 0.0, 0.0));
			}
		}
		if (_selectedObjectID == -1) {
			hide();
		} else {
			show();
		}
	}

	void ObjectInfoWidget::doRemoveObject() {
		if (_selectedObjectID != -1) {
			QMessageBox box;
			box.setText(QString("Really delete object?"));
			box.setStandardButtons(QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
			if (box.exec() == QMessageBox::StandardButton::Yes) {
				bool waypoint = false;
				{
					// Daniel: scope to remove reference to sharedPtr before waiting!
					auto go = api::EngineController::GetSingleton().getObjectFacade()->getObject(_selectedObjectID);
					waypoint = go->getType() == "Waypoint";
					emit selectObject(-1);
					go->setDie();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				if (waypoint) {
					api::EngineController::GetSingleton().getWaynetManager()->createWaynet();
				}
				emit updateObjectList();
			}
		}
	}

	void ObjectInfoWidget::applyChanges() {
		for (auto & p : _entries) {
			p.second(p.first->text().toStdString());
		}
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
