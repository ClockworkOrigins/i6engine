#include "widgets/AffectorPropertyWindows/JetAffectorPropertyWindow.h"

#include "properties/DynamicAttributeProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseJetAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	JetAffectorPropertyWindow::JetAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ACCELERATION = QApplication::tr("Acceleration");

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

	void JetAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop)
			return;

		ParticleUniverse::JetAffector * affector = static_cast<ParticleUniverse::JetAffector *>(_owner->getPUElement());
		if (!affector)
			return;

		if (propertyName == PRNL_ACCELERATION) {
			// Acceleration: DynamicAttribute
			affector->setDynAcceleration(prop->getDynamicAttribute());
			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
