#include "widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GravityAffectorPropertyWindow::GravityAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_GRAVITY = "Gravity";

		// Gravity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	}

	GravityAffectorPropertyWindow::~GravityAffectorPropertyWindow() {
	}

	void GravityAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::GravityAffector * gravityAffector = static_cast<ParticleUniverse::GravityAffector *>(affector);

		// Gravity: ParticleUniverse::Real
		setDouble(PRNL_GRAVITY, gravityAffector->getGravity());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
