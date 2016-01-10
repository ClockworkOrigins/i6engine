#include "widgets/BehaviourPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseBehaviour.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BehaviourPropertyWindow::BehaviourPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_BEHAVIOUR_SLAVE = BEHAVIOUR_SLAVE;
		PRNL_BEHAVIOUR_TYPE = "Behaviour type";
		PRNL_NAME = "Name";

		// Type
		QStringList types;
		types.append(CST_BEHAVIOUR_SLAVE);
		append(new properties::EnumProperty(this, PRNL_BEHAVIOUR_TYPE, PRNL_BEHAVIOUR_TYPE, types));
	}

	BehaviourPropertyWindow::~BehaviourPropertyWindow() {
	}

	void BehaviourPropertyWindow::copyAttributesFromBehaviour(ParticleUniverse::ParticleBehaviour * behaviour) {
		// Type: List of types
		setEnumString(PRNL_BEHAVIOUR_TYPE, QString::fromStdString(behaviour->getBehaviourType()));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
