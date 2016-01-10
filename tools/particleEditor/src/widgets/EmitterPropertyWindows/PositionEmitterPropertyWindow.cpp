#include "widgets/EmitterPropertyWindows/PositionEmitterPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/Vec3ListProperty.h"

#include "ParticleEmitters/ParticleUniversePositionEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	PositionEmitterPropertyWindow::PositionEmitterPropertyWindow(QWidget * par, QString name) : EmitterPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_POSITION_EMITTER_POSITION = "Add particle position";
		PRNL_POSITION_EMITTER_RANDOMIZE = "Position randomize";
		PRNL_POSITION = "Position";
		PRNL_POSITION_X = ".x";
		PRNL_POSITION_Y = ".y";
		PRNL_POSITION_Z = ".z";

		// Add Position: Button, with wich new positions can be added
		append(new properties::Vec3ListProperty(this, PRNL_POSITION_EMITTER_POSITION, PRNL_POSITION_EMITTER_POSITION));

		// Randomize: bool
		append(new properties::BoolProperty(this, PRNL_POSITION_EMITTER_RANDOMIZE, PRNL_POSITION_EMITTER_RANDOMIZE, ParticleUniverse::PositionEmitter::DEFAULT_RANDOMIZE));
	}

	PositionEmitterPropertyWindow::~PositionEmitterPropertyWindow() {
	}

	void PositionEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::PositionEmitter * positionEmitter = static_cast<ParticleUniverse::PositionEmitter *>(emitter);

		// Positions: List
		std::vector<Ogre::Vector3> positions(positionEmitter->getPositions().begin(), positionEmitter->getPositions().end());
		setVector3List(PRNL_POSITION_EMITTER_POSITION, positions);

		// Randomize: bool
		setBool(PRNL_POSITION_EMITTER_RANDOMIZE, positionEmitter->isRandomized());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
