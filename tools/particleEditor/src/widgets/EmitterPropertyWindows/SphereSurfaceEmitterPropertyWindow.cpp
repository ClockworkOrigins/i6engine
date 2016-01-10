#include "widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	SphereSurfaceEmitterPropertyWindow::SphereSurfaceEmitterPropertyWindow(QWidget * par, QString name) : EmitterPropertyWindow(par, name) {
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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
