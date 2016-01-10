#include "widgets/EventHandlerPropertyWindows/DoPlacementParticleEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"

#include "ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandler.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	DoPlacementParticleEventHandlerPropertyWindow::DoPlacementParticleEventHandlerPropertyWindow(QWidget * par, QString name) : EventHandlerPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_NUMBER_OF_PARTICLES = "Number of particles";
		PRNL_INHERIT_POSITION = "Inherit position";
		PRNL_INHERIT_DIRECTION = "Inherit direction";
		PRNL_INHERIT_ORIENTATION = "Inherit orientation";
		PRNL_INHERIT_TIME_TO_LIVE = "Inherit time to live";
		PRNL_INHERIT_MASS = "Inherit mass";
		PRNL_INHERIT_TEXTURE_COORDINATE = "Inherit texture coord";
		PRNL_INHERIT_COLOUR = "Inherit colour";
		PRNL_INHERIT_PARTICLE_WIDTH = "Inherit particle width";
		PRNL_INHERIT_PARTICLE_HEIGHT = "Inherit particle height";
		PRNL_INHERIT_PARTICLE_DEPTH = "Inherit particle depth";

		// Force Emitter: Is established by means of a connection

		// Number of Particles: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_NUMBER_OF_PARTICLES, PRNL_NUMBER_OF_PARTICLES, ParticleUniverse::DoPlacementParticleEventHandler::DEFAULT_NUMBER_OF_PARTICLES));

		// Inherit Position: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_POSITION, PRNL_INHERIT_POSITION, true));

		// Inherit Direction: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_DIRECTION, PRNL_INHERIT_DIRECTION, false));

		// Inherit Orientation: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_ORIENTATION, PRNL_INHERIT_ORIENTATION, false));

		// Inherit Time to live: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_TIME_TO_LIVE, PRNL_INHERIT_TIME_TO_LIVE, false));

		// Inherit Mass: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_MASS, PRNL_INHERIT_MASS, false));

		// Inherit Texture Coord: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_TEXTURE_COORDINATE, PRNL_INHERIT_TEXTURE_COORDINATE, false));

		// Inherit Colour: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_COLOUR, PRNL_INHERIT_COLOUR, false));

		// Inherit Particle width: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_PARTICLE_WIDTH, PRNL_INHERIT_PARTICLE_WIDTH, false));

		// Inherit Particle height: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_PARTICLE_HEIGHT, PRNL_INHERIT_PARTICLE_HEIGHT, false));

		// Inherit Particle Depth: bool
		append(new properties::BoolProperty(this, PRNL_INHERIT_PARTICLE_DEPTH, PRNL_INHERIT_PARTICLE_DEPTH, false));
	}

	DoPlacementParticleEventHandlerPropertyWindow::~DoPlacementParticleEventHandlerPropertyWindow() {
	}

	void DoPlacementParticleEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

		// Copy properties from eventHandler to property window
		ParticleUniverse::DoPlacementParticleEventHandler * doPlacementParticleEventHandler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler *>(eventHandler);

		// Number of Particles: ParticleUniverse::Real
		setDouble(PRNL_NUMBER_OF_PARTICLES, doPlacementParticleEventHandler->getNumberOfParticles());

		// Inherit Position: bool
		setBool(PRNL_INHERIT_POSITION, doPlacementParticleEventHandler->isInheritPosition());

		// Inherit Direction: bool
		setBool(PRNL_INHERIT_DIRECTION, doPlacementParticleEventHandler->isInheritDirection());

		// Inherit Orientation: bool
		setBool(PRNL_INHERIT_ORIENTATION, doPlacementParticleEventHandler->isInheritOrientation());

		// Inherit Time to live: bool
		setBool(PRNL_INHERIT_TIME_TO_LIVE, doPlacementParticleEventHandler->isInheritTimeToLive());

		// Inherit Mass: bool
		setBool(PRNL_INHERIT_MASS, doPlacementParticleEventHandler->isInheritMass());

		// Inherit Texture Coord: bool
		setBool(PRNL_INHERIT_TEXTURE_COORDINATE, doPlacementParticleEventHandler->isInheritTextureCoordinate());

		// Inherit Colour: bool
		setBool(PRNL_INHERIT_COLOUR, doPlacementParticleEventHandler->isInheritColour());

		// Inherit Particle width: bool
		setBool(PRNL_INHERIT_PARTICLE_WIDTH, doPlacementParticleEventHandler->isInheritParticleWidth());

		// Inherit Particle height: bool
		setBool(PRNL_INHERIT_PARTICLE_HEIGHT, doPlacementParticleEventHandler->isInheritParticleHeight());

		// Inherit Particle Depth: bool
		setBool(PRNL_INHERIT_PARTICLE_DEPTH, doPlacementParticleEventHandler->isInheritParticleDepth());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
