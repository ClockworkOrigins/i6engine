#include "widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleEmitters/ParticleUniverseCircleEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	CircleEmitterPropertyWindow::CircleEmitterPropertyWindow(QWidget * par, QString name) : EmitterPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_CIRCLE_EMITTER_RADIUS = "Circle Radius";
		PRNL_CIRCLE_EMITTER_STEP = "Circle Step";
		PRNL_CIRCLE_EMITTER_ANGLE = "Circle angle";
		PRNL_CIRCLE_EMITTER_RANDOM = "Circle Random";
		PRNL_CIRCLE_EMITTER_NORMAL = "Circle normal";

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_CIRCLE_EMITTER_RADIUS, PRNL_CIRCLE_EMITTER_RADIUS, ParticleUniverse::CircleEmitter::DEFAULT_RADIUS));

		// Step: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_CIRCLE_EMITTER_STEP, PRNL_CIRCLE_EMITTER_STEP, ParticleUniverse::CircleEmitter::DEFAULT_STEP));

		// Angle: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_CIRCLE_EMITTER_ANGLE, PRNL_CIRCLE_EMITTER_ANGLE, ParticleUniverse::CircleEmitter::DEFAULT_ANGLE));

		// Random: bool
		append(new properties::BoolProperty(this, PRNL_CIRCLE_EMITTER_RANDOM, PRNL_CIRCLE_EMITTER_RANDOM, ParticleUniverse::CircleEmitter::DEFAULT_RANDOM));

		// Normal: Vector3
		append(new properties::Vec3Property(this, PRNL_CIRCLE_EMITTER_NORMAL, PRNL_CIRCLE_EMITTER_NORMAL, ParticleUniverse::CircleEmitter::DEFAULT_NORMAL));
	}

	CircleEmitterPropertyWindow::~CircleEmitterPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
