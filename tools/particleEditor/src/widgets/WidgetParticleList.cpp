#include "widgets/WidgetParticleList.h"

#include "ParticleUniverseSystemManager.h"

#include <QFileDialog>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetParticleList::WidgetParticleList(QWidget * par) : QWidget(par), _currentParticleTemplate(), _templateMap(), _system(nullptr), _dirty(false), _script() {
		setupUi(this);

		refreshParticleList();

		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectParticle(QTreeWidgetItem *)));
	}

	WidgetParticleList::~WidgetParticleList() {
	}

	void WidgetParticleList::selectParticle(QTreeWidgetItem * item) {
		if (_currentParticleTemplate != item->text(0)) {
			if (_dirty && _script.toStdString() != ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system)) {
				QString file = QFileDialog::getSaveFileName(nullptr, "Save file ...", QString::fromStdString("../media/particles"), "Particle Files (*.pu)");
				if (!file.isEmpty()) {
					ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system, file.toStdString());
				}
			}
			_currentParticleTemplate = item->text(0);
			_dirty = false;
			emit createNewSystem(item->text(0));
		}
	}

	void WidgetParticleList::selectParticle(QString templateName) {
		selectParticle(_templateMap[templateName]);
	}

	void WidgetParticleList::setNewParticleSystem(ParticleUniverse::ParticleSystem * system) {
		_system = system;
		_script = QString::fromStdString(ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system));
	}

	void WidgetParticleList::notifyChanged() {
		_dirty = true;
	}

	void WidgetParticleList::saveParticle() {
		if (_dirty) {
			QString file = QFileDialog::getSaveFileName(nullptr, "Save file ...", QString::fromStdString("../media/particles"), "Particle Files (*.pu)");
			if (!file.isEmpty()) {
				ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system, file.toStdString());
			}
			_dirty = false;
		}
	}

	void WidgetParticleList::refreshParticleList() {
		_currentParticleTemplate = "";
		_templateMap.clear();
		ParticleUniverse::vector<ParticleUniverse::String> names;
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->particleSystemTemplateNames(names);
		treeWidget->clear();
		for (ParticleUniverse::String s : names) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(s) });
			_templateMap.insert(std::make_pair(QString::fromStdString(s), twi));
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
