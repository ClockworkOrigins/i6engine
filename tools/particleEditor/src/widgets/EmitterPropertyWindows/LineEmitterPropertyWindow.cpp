#include "widgets/EmitterPropertyWindows/LineEmitterPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseLineEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	LineEmitterPropertyWindow::LineEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_LINE_END = "Line end";
		PRNL_LINE_MAX_INCREMENT = "Line max increment";
		PRNL_LINE_MIN_INCREMENT = "Line min increment";
		PRNL_LINE_DEVIATION = "Line deviation";

		// End: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_LINE_END, PRNL_LINE_END, ParticleUniverse::LineEmitter::DEFAULT_END));

		// Max increment: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_LINE_MAX_INCREMENT, PRNL_LINE_MAX_INCREMENT, ParticleUniverse::LineEmitter::DEFAULT_MAX_INCREMENT));

		// Min increment: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_LINE_MIN_INCREMENT, PRNL_LINE_MIN_INCREMENT, ParticleUniverse::LineEmitter::DEFAULT_MIN_INCREMENT));

		// Deviation: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_LINE_DEVIATION, PRNL_LINE_DEVIATION, ParticleUniverse::LineEmitter::DEFAULT_MAX_DEVIATION));
	}

	LineEmitterPropertyWindow::~LineEmitterPropertyWindow() {
	}

	void LineEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::LineEmitter * lineEmitter = static_cast<ParticleUniverse::LineEmitter *>(emitter);

		// End: Ogre::Vector3
		setVector3(PRNL_LINE_END, lineEmitter->getEnd());

		// Max increment: ParticleUniverse::Real
		setDouble(PRNL_LINE_MAX_INCREMENT, lineEmitter->getMaxIncrement());

		// Min increment: ParticleUniverse::Real
		setDouble(PRNL_LINE_MIN_INCREMENT, lineEmitter->getMinIncrement());

		// Deviation: ParticleUniverse::Real
		setDouble(PRNL_LINE_DEVIATION, lineEmitter->getMaxDeviation());
	}

	void LineEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::LineEmitter * lineEmitter = static_cast<ParticleUniverse::LineEmitter *>(_owner->getPUElement());
		if (!lineEmitter) {
			return;
		}

		if (propertyName == PRNL_LINE_END) {
			// Update emitter with end
			lineEmitter->setEnd(prop->getVector3());
		} else if (propertyName == PRNL_LINE_MAX_INCREMENT) {
			// Update emitter with Max increment
			lineEmitter->setMaxIncrement(prop->getDouble());
		} else if (propertyName == PRNL_LINE_MIN_INCREMENT) {
			// Update emitter with Min increment
			lineEmitter->setMinIncrement(prop->getDouble());
		} else if (propertyName == PRNL_LINE_DEVIATION) {
			// Update emitter with Deviation
			lineEmitter->setMaxDeviation(prop->getDouble());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */