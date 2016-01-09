#include "widgets/ExternPropertyWindows/GravityExternPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h"

#include "Externs/ParticleUniverseGravityExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GravityExternPropertyWindow::GravityExternPropertyWindow(QWidget * par, QString name) : ExternPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_EXTERN_THRESHOLD = "Distance threshold";

		// Distance Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<double>::max()));

		// Gravity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	}

	GravityExternPropertyWindow::~GravityExternPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
