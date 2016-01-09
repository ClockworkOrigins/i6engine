#include "widgets/AffectorPropertyWindows/JetAffectorPropertyWindow.h"

#include "properties/DynamicAttributeProperty.h"

#include "ParticleAffectors/ParticleUniverseJetAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	JetAffectorPropertyWindow::JetAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ACCELERATION = "Acceleration";

		// Acceleration: DynamicAttribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::JetAffector::DEFAULT_ACCELERATION);
		append(new properties::DynamicAttributeProperty(this, PRNL_ACCELERATION, PRNL_ACCELERATION, dynAttr));
	}

	JetAffectorPropertyWindow::~JetAffectorPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
