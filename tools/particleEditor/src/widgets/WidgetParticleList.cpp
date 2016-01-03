#include "widgets/WidgetParticleList.h"

#include "ParticleUniverseSystemManager.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetParticleList::WidgetParticleList(QWidget * par) : QWidget(par) {
		setupUi(this);

		refreshParticleList();
	}

	WidgetParticleList::~WidgetParticleList() {
	}

	void WidgetParticleList::refreshParticleList() {
		ParticleUniverse::vector<ParticleUniverse::String> names;
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->particleSystemTemplateNames(names);
		treeView->clear();
		for (ParticleUniverse::String s : names) {
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
