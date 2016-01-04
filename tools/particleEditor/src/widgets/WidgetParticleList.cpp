#include "widgets/WidgetParticleList.h"

#include "ParticleUniverseSystemManager.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetParticleList::WidgetParticleList(QWidget * par) : QWidget(par), _currentParticleTemplate() {
		setupUi(this);

		refreshParticleList();

		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectParticle(QTreeWidgetItem *)));
	}

	WidgetParticleList::~WidgetParticleList() {
	}

	void WidgetParticleList::selectParticle(QTreeWidgetItem * item) {
		if (_currentParticleTemplate != item->text(0)) {
			_currentParticleTemplate = item->text(0);
			emit createNewSystem(item->text(0));
		}
	}

	void WidgetParticleList::refreshParticleList() {
		ParticleUniverse::vector<ParticleUniverse::String> names;
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->particleSystemTemplateNames(names);
		treeWidget->clear();
		for (ParticleUniverse::String s : names) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(s) });
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
