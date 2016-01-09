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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
