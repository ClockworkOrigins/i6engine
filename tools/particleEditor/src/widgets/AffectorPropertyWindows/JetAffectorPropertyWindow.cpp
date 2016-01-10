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

	void JetAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::JetAffector * jetAffector = static_cast<ParticleUniverse::JetAffector *>(affector);

		// Acceleration: DynamicAttribute
		setDynamicAttribute(PRNL_ACCELERATION, jetAffector->getDynAcceleration());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
