#include "widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GravityAffectorPropertyWindow::GravityAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_GRAVITY = "Gravity";

		// Gravity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	}

	GravityAffectorPropertyWindow::~GravityAffectorPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
