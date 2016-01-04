#include "widgets/WidgetScript.h"

#include "ParticleUniverseSystemManager.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetScript::WidgetScript(QWidget * par) : QWidget(par) {
		setupUi(this);
	}

	WidgetScript::~WidgetScript() {
	}

	void WidgetScript::loadScript(ParticleUniverse::ParticleSystem * system) {
		ParticleUniverse::String script = ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(system);
		textEdit->setText(QString::fromStdString(script));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
