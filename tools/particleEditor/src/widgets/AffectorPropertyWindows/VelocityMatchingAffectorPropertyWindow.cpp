#include "widgets/AffectorPropertyWindows/VelocityMatchingAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseVelocityMatchingAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	VelocityMatchingAffectorPropertyWindow::VelocityMatchingAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_VELOCITY_MATCHING_RADIUS = "Velocity matching radius";

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_VELOCITY_MATCHING_RADIUS, PRNL_VELOCITY_MATCHING_RADIUS, ParticleUniverse::VelocityMatchingAffector::DEFAULT_RADIUS));
	}

	VelocityMatchingAffectorPropertyWindow::~VelocityMatchingAffectorPropertyWindow() {
	}

	void VelocityMatchingAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::VelocityMatchingAffector * velocityMatchingAffector = static_cast<ParticleUniverse::VelocityMatchingAffector *>(affector);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_VELOCITY_MATCHING_RADIUS, velocityMatchingAffector->getRadius());
	}

	void VelocityMatchingAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::VelocityMatchingAffector * affector = static_cast<ParticleUniverse::VelocityMatchingAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_VELOCITY_MATCHING_RADIUS) {
			// Radius: ParticleUniverse::Real
			affector->setRadius(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
