#include "widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseCircleEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	CircleEmitterPropertyWindow::CircleEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
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

	void CircleEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::CircleEmitter * circleEmitter = static_cast<ParticleUniverse::CircleEmitter *>(emitter);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_CIRCLE_EMITTER_RADIUS, circleEmitter->getRadius());

		// Step: ParticleUniverse::Real
		setDouble(PRNL_CIRCLE_EMITTER_STEP, circleEmitter->getStep());

		// Angle: ParticleUniverse::Real
		setDouble(PRNL_CIRCLE_EMITTER_ANGLE, circleEmitter->getCircleAngle());

		// Random: bool
		setBool(PRNL_CIRCLE_EMITTER_RANDOM, circleEmitter->isRandom());

		// Normal: Vector3
		setVector3(PRNL_CIRCLE_EMITTER_NORMAL, circleEmitter->getNormal());
	}

	void CircleEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::CircleEmitter * circleEmitter = static_cast<ParticleUniverse::CircleEmitter *>(_owner->getPUElement());
		if (!circleEmitter) {
			return;
		}

		if (propertyName == PRNL_CIRCLE_EMITTER_ANGLE) {
			// Update emitter with angle
			circleEmitter->setCircleAngle(prop->getDouble());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_NORMAL) {
			// Update emitter with normal
			circleEmitter->setNormal(prop->getVector3());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_RADIUS) {
			// Update emitter with radius
			circleEmitter->setRadius(prop->getDouble());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_RANDOM) {
			// Update emitter with random
			circleEmitter->setRandom(prop->getBool());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_STEP) {
			// Update emitter with step
			circleEmitter->setStep(prop->getDouble());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
