#include "widgets/AffectorPropertyWindows/ParticleFollowerPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleAffectors/ParticleUniverseParticleFollower.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ParticleFollowerPropertyWindow::ParticleFollowerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_MIN_DISTANCE = "Minimum distance";
		PRNL_MAX_DISTANCE = "Maximum distance";

		// Min distance: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_MIN_DISTANCE, PRNL_MIN_DISTANCE, ParticleUniverse::ParticleFollower::DEFAULT_MIN_DISTANCE));

		// Max distance: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_MAX_DISTANCE, PRNL_MAX_DISTANCE, ParticleUniverse::ParticleFollower::DEFAULT_MAX_DISTANCE));
	}

	ParticleFollowerPropertyWindow::~ParticleFollowerPropertyWindow() {
	}

	void ParticleFollowerPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ParticleFollower * particleFollower = static_cast<ParticleUniverse::ParticleFollower *>(affector);

		// Min distance: ParticleUniverse::Real
		setDouble(PRNL_MIN_DISTANCE, particleFollower->getMinDistance());

		// Max distance: ParticleUniverse::Real
		setDouble(PRNL_MAX_DISTANCE, particleFollower->getMaxDistance());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
