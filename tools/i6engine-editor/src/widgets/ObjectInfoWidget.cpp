#include "i6engine/editor/widgets/ObjectInfoWidget.h"

#include <thread>

#include "i6engine/utils/i6eString.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/WaynetManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/editor/plugins/TypePluginInterface.h"

#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPluginLoader>
#include <QPushButton>

Q_IMPORT_PLUGIN(EditorTypeAngle)
Q_IMPORT_PLUGIN(EditorTypeBool)
Q_IMPORT_PLUGIN(EditorTypeDouble)
Q_IMPORT_PLUGIN(EditorTypeInteger)
Q_IMPORT_PLUGIN(EditorTypeLightType)
Q_IMPORT_PLUGIN(EditorTypeMoverInterpolateMode)
Q_IMPORT_PLUGIN(EditorTypeMoverInterpolateWay)
Q_IMPORT_PLUGIN(EditorTypeMoverPositioning)
Q_IMPORT_PLUGIN(EditorTypeQuaternion)
Q_IMPORT_PLUGIN(EditorTypeShapeType)
Q_IMPORT_PLUGIN(EditorTypeShatterInterest)
Q_IMPORT_PLUGIN(EditorTypeString)
Q_IMPORT_PLUGIN(EditorTypeVec3)
Q_IMPORT_PLUGIN(EditorTypeVec4)

namespace i6engine {
namespace editor {
namespace widgets {

	ObjectInfoWidget::ObjectInfoWidget(QWidget * par) : QWidget(par), _selectedObjectID(-1), _infos(), _entries(), _typePlugins() {
		setupUi(this);

		connect(this, SIGNAL(selectObject(int64_t)), this, SLOT(doSelectObject(int64_t)));
		connect(this, SIGNAL(removeObject()), this, SLOT(doRemoveObject()));

		hide();

		loadTypePlugins();
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
		emit selectedObject(id);

		if (_selectedObjectID != -1) {
			api::GOPtr go = api::EngineController::GetSingleton().getObjectFacade()->getObject(id);
			api::WeakGOPtr wgo = go;

			QLabel * l = new QLabel(QString::fromStdString(go->getType()), this);
			verticalLayout->addWidget(l);
			_infos.push_back(l);

			QWidget * w = new QWidget(this);
			QHBoxLayout * layout = new QHBoxLayout(w);
			l = new QLabel(QString("Flags"), w);
			plugins::TypeWidgetInterface * twi = _typePlugins["String"]->createWidget(w);
			std::vector<std::string> flags = go->getFlags();
			std::string flagString;
			for (size_t i = 0; i < flags.size(); i++) {
				flagString += flags[i];

				if (i < flags.size() - 1) {
					flagString += "|";
				}
			}
			twi->setValues(flagString);
			layout->addWidget(l);
			layout->addWidget(twi);
			verticalLayout->addWidget(w);
			_infos.push_back(w);
			_entries.insert(std::make_pair(twi, [wgo](std::string s) {
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
					if (_typePlugins.find(std::get<api::ComponentOptionsParameter::WIDGETTYPE>(option)) == _typePlugins.end()) {
						QMessageBox box;
						box.setWindowTitle(QApplication::tr("Attribute can't be displayed!"));
						box.setInformativeText(QApplication::tr("Attribute '") + QString::fromStdString(std::get<api::ComponentOptionsParameter::NAME>(option)) + QApplication::tr("' has unregistered type '") + QString::fromStdString(std::get<api::ComponentOptionsParameter::WIDGETTYPE>(option)) + QApplication::tr("'. It will be skipped and is not accessible. Check your plugins folder!"));
						box.setStandardButtons(QMessageBox::StandardButton::Ok);
						box.exec();
						continue;
					}
					w = new QWidget(gb);
					layout = new QHBoxLayout(w);
					w->setLayout(layout);
					vLayout->addWidget(w);
					l = new QLabel(QString::fromStdString(std::get<api::ComponentOptionsParameter::NAME>(option)), w);
					layout->addWidget(l);
					twi = _typePlugins[std::get<api::ComponentOptionsParameter::WIDGETTYPE>(option)]->createWidget(w);
					twi->setValues(std::get<api::ComponentOptionsParameter::READFUNC>(option)());
					layout->addWidget(twi);
					_entries.insert(std::make_pair(twi, std::get<api::ComponentOptionsParameter::WRITEFUNC>(option)));
					twi->setReadOnly(std::get<api::ComponentOptionsParameter::ACCESSSTATE>(option) == api::AccessState::READONLY);
				}
			}

			QPushButton * button = new QPushButton(QApplication::tr("Apply"), this);
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
			box.setText(QApplication::tr("Really delete object?"));
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
				emit changedLevel();
			}
		}
	}

	void ObjectInfoWidget::applyChanges() {
		for (auto & p : _entries) {
			p.second(p.first->getValues());
		}
	}

	void ObjectInfoWidget::loadTypePlugins() {
		foreach(QObject * plugin, QPluginLoader::staticInstances()) {
			if (qobject_cast<plugins::TypePluginInterface *>(plugin)) {
				_typePlugins.insert(std::make_pair(qobject_cast<plugins::TypePluginInterface *>(plugin)->getIdentifier(), qobject_cast<plugins::TypePluginInterface *>(plugin)));
			}
		}


		QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/editor/type");
		foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject * plugin = loader.instance();
			if (plugin) {
				_typePlugins.insert(std::make_pair(qobject_cast<plugins::TypePluginInterface *>(plugin)->getIdentifier(), qobject_cast<plugins::TypePluginInterface *>(plugin)));
			} else {
				QMessageBox box;
				box.setWindowTitle(QApplication::tr("Error loading plugin!"));
				box.setInformativeText(loader.errorString());
				box.setStandardButtons(QMessageBox::StandardButton::Ok);
				box.exec();
			}
		}
		emit changedLevel();
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
