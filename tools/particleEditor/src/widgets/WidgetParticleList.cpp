#include "widgets/WidgetParticleList.h"

#include "ParticleUniverseSystemManager.h"

#include <QFileDialog>
#include <QMessageBox>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetParticleList::WidgetParticleList(QWidget * par) : QWidget(par), _currentParticleTemplate(), _templateMap(), _system(nullptr), _dirty(false), _script(), _systemFileMapping() {
		setupUi(this);

		refreshParticleList();

		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectParticle(QTreeWidgetItem *)));
	}

	WidgetParticleList::~WidgetParticleList() {
	}

	void WidgetParticleList::selectParticle(QTreeWidgetItem * item) {
		if (_currentParticleTemplate != item->text(0)) {
			if (_dirty) {
				if (QMessageBox::question(this, QApplication::tr("Unsaved changes"), QApplication::tr("There are unsaved changes for particle ") + _currentParticleTemplate + ".\n " + QApplication::tr("Do you want to save?"), QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No) == QMessageBox::StandardButton::Yes) {
					saveParticle();
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
			auto it = _systemFileMapping.find(_currentParticleTemplate);
			QString file;
			if (it != _systemFileMapping.end()) {
				file = it->second;
			} else {
				file = QFileDialog::getSaveFileName(nullptr, QApplication::tr("Save file ..."), QString::fromStdString("../media/particles"), QApplication::tr("Particle Files (*.pu)"));
			}
			if (!file.isEmpty()) {
				ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system, file.toStdString());
				_systemFileMapping[_currentParticleTemplate] = file;
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
