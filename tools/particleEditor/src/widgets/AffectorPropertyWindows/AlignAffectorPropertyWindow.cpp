#include "widgets/AffectorPropertyWindows/AlignAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseAlignAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	AlignAffectorPropertyWindow::AlignAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		PRNL_AFFECTOR_ALIGN_RESIZE = QApplication::tr("Resize");

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

	void AlignAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::AlignAffector * affector = static_cast<ParticleUniverse::AlignAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_AFFECTOR_ALIGN_RESIZE) {
			affector->setResize(prop->getBool());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
