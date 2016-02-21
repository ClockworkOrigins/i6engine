#include "widgets/EmitterPropertyWindows/BoxEmitterPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseBoxEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BoxEmitterPropertyWindow::BoxEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_BOX_EMITTER_WIDTH = QApplication::tr("Box width");
		PRNL_BOX_EMITTER_HEIGHT = QApplication::tr("Box height");
		PRNL_BOX_EMITTER_DEPTH = QApplication::tr("Box depth");

		// Width: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_EMITTER_WIDTH, PRNL_BOX_EMITTER_WIDTH, ParticleUniverse::BoxEmitter::DEFAULT_WIDTH));

		// Height: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_EMITTER_HEIGHT, PRNL_BOX_EMITTER_HEIGHT, ParticleUniverse::BoxEmitter::DEFAULT_HEIGHT));

		// Depth: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_EMITTER_DEPTH, PRNL_BOX_EMITTER_DEPTH, ParticleUniverse::BoxEmitter::DEFAULT_DEPTH));
	}

	BoxEmitterPropertyWindow::~BoxEmitterPropertyWindow() {
	}

	void BoxEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::BoxEmitter * boxEmitter = static_cast<ParticleUniverse::BoxEmitter *>(emitter);

		// Width: ParticleUniverse::Real
		setDouble(PRNL_BOX_EMITTER_WIDTH, boxEmitter->getWidth());

		// Height: ParticleUniverse::Real
		setDouble(PRNL_BOX_EMITTER_HEIGHT, boxEmitter->getHeight());

		// Depth: ParticleUniverse::Real
		setDouble(PRNL_BOX_EMITTER_DEPTH, boxEmitter->getDepth());
	}

	void BoxEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::BoxEmitter * boxEmitter = static_cast<ParticleUniverse::BoxEmitter *>(_owner->getPUElement());
		if (!boxEmitter) {
			return;
		}

		if (propertyName == PRNL_BOX_EMITTER_WIDTH) {
			// Update emitter with width
			boxEmitter->setWidth(prop->getDouble());
		} else if (propertyName == PRNL_BOX_EMITTER_HEIGHT) {
			// Update emitter with height
			boxEmitter->setHeight(prop->getDouble());
		} else if (propertyName == PRNL_BOX_EMITTER_DEPTH) {
			// Update emitter with depth
			boxEmitter->setDepth(prop->getDouble());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
