#include "widgets/AffectorPropertyWindows/AlignAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "ParticleAffectors/ParticleUniverseAlignAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	AlignAffectorPropertyWindow::AlignAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		PRNL_AFFECTOR_ALIGN_RESIZE = "Resize";

		// Resize: bool
		append(new properties::BoolProperty(this, PRNL_AFFECTOR_ALIGN_RESIZE, PRNL_AFFECTOR_ALIGN_RESIZE, ParticleUniverse::AlignAffector::DEFAULT_RESIZE));
	}

	AlignAffectorPropertyWindow::~AlignAffectorPropertyWindow() {
	}

	void AlignAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::AlignAffector * alignAffector = static_cast<ParticleUniverse::AlignAffector *>(affector);

		// Resize: bool
		setBool(PRNL_AFFECTOR_ALIGN_RESIZE, alignAffector->isResize());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
