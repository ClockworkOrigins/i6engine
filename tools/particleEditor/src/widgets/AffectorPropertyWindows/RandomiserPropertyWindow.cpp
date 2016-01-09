#include "widgets/AffectorPropertyWindows/RandomiserPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseRandomiser.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	RandomiserPropertyWindow::RandomiserPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_RANDOM_DIRECTION = "Random direction";
		PRNL_MAX_DEVIATION = "Maximum deviation";
		PRNL_TIME_STEP = "Time step";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Max. deviation: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_MAX_DEVIATION, PRNL_MAX_DEVIATION, ParticleUniverse::Randomiser::DEFAULT_MAX_DEVIATION));

		// Random direction: bool
		append(new properties::BoolProperty(this, PRNL_RANDOM_DIRECTION, PRNL_RANDOM_DIRECTION, ParticleUniverse::Randomiser::DEFAULT_RANDOM_DIRECTION));

		// Timestep: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::Randomiser::DEFAULT_TIME_STEP));
	}

	RandomiserPropertyWindow::~RandomiserPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
