#include "widgets/AffectorPropertyWindows/CollisionAvoidanceAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseCollisionAvoidanceAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	CollisionAvoidanceAffectorPropertyWindow::CollisionAvoidanceAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_COLLISON_AVOIDANCE_RADIUS = "Avoidance radius";

		// Avoidance Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLISON_AVOIDANCE_RADIUS, PRNL_COLLISON_AVOIDANCE_RADIUS, ParticleUniverse::CollisionAvoidanceAffector::DEFAULT_RADIUS));
	}

	CollisionAvoidanceAffectorPropertyWindow::~CollisionAvoidanceAffectorPropertyWindow() {
	}

	void CollisionAvoidanceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::CollisionAvoidanceAffector * collisionAvoidanceAffector = static_cast<ParticleUniverse::CollisionAvoidanceAffector *>(affector);

		// Avoidance Radius: ParticleUniverse::Real
		setDouble(PRNL_COLLISON_AVOIDANCE_RADIUS, collisionAvoidanceAffector->getRadius());
	}

	void CollisionAvoidanceAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::CollisionAvoidanceAffector * affector = static_cast<ParticleUniverse::CollisionAvoidanceAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_COLLISON_AVOIDANCE_RADIUS) {
			// Avoidance Radius: ParticleUniverse::Real
			affector->setRadius(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
