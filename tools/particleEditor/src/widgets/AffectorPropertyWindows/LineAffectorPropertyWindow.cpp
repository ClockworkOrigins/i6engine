#include "widgets/AffectorPropertyWindows/LineAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseLineAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	LineAffectorPropertyWindow::LineAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_END = "End position";
		PRNL_DRIFT = "Drift";
		PRNL_MAX_DEVIATION = "Maximum deviation";
		PRNL_TIME_STEP = "Time step";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Line end: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_END, PRNL_END, ParticleUniverse::LineAffector::DEFAULT_END));

		// Maximum Deviation: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_MAX_DEVIATION, PRNL_MAX_DEVIATION, ParticleUniverse::LineAffector::DEFAULT_MAX_DEVIATION));

		// Time Step: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::LineAffector::DEFAULT_TIME_STEP));

		// Drift: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_DRIFT, PRNL_DRIFT, ParticleUniverse::LineAffector::DEFAULT_DRIFT));
	}

	LineAffectorPropertyWindow::~LineAffectorPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
