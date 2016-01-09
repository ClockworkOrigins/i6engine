#include "widgets/EmitterPropertyWindows/BoxEmitterPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleEmitters/ParticleUniverseBoxEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BoxEmitterPropertyWindow::BoxEmitterPropertyWindow(QWidget * par, QString name) : EmitterPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_BOX_EMITTER_WIDTH = "Box width";
		PRNL_BOX_EMITTER_HEIGHT = "Box height";
		PRNL_BOX_EMITTER_DEPTH = "Box depth";

		// Width: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_EMITTER_WIDTH, PRNL_BOX_EMITTER_WIDTH, ParticleUniverse::BoxEmitter::DEFAULT_WIDTH));

		// Height: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_EMITTER_HEIGHT, PRNL_BOX_EMITTER_HEIGHT, ParticleUniverse::BoxEmitter::DEFAULT_HEIGHT));

		// Depth: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_EMITTER_DEPTH, PRNL_BOX_EMITTER_DEPTH, ParticleUniverse::BoxEmitter::DEFAULT_DEPTH));
	}

	BoxEmitterPropertyWindow::~BoxEmitterPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
