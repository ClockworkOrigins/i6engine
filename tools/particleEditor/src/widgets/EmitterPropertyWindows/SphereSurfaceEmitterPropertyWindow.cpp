#include "widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	SphereSurfaceEmitterPropertyWindow::SphereSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_SPHERE_SURFACE_EMITTER_RADIUS = "Sphere Radius";

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SPHERE_SURFACE_EMITTER_RADIUS, PRNL_SPHERE_SURFACE_EMITTER_RADIUS, ParticleUniverse::SphereSurfaceEmitter::DEFAULT_RADIUS));
	}

	SphereSurfaceEmitterPropertyWindow::~SphereSurfaceEmitterPropertyWindow() {
	}

	void SphereSurfaceEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::SphereSurfaceEmitter * sphereSurfaceEmitter = static_cast<ParticleUniverse::SphereSurfaceEmitter *>(emitter);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_SPHERE_SURFACE_EMITTER_RADIUS, sphereSurfaceEmitter->getRadius());
	}

	void SphereSurfaceEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::SphereSurfaceEmitter * sphereSurfaceEmitter = static_cast<ParticleUniverse::SphereSurfaceEmitter *>(_owner->getPUElement());
		if (!sphereSurfaceEmitter) {
			return;
		}

		if (propertyName == PRNL_SPHERE_SURFACE_EMITTER_RADIUS) {
			// Update emitter with Radius
			sphereSurfaceEmitter->setRadius(prop->getDouble());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
