#include "widgets/AffectorPropertyWindows/ScaleVelocityAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "ParticleAffectors/ParticleUniverseScaleVelocityAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ScaleVelocityAffectorPropertyWindow::ScaleVelocityAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_VELOCITY_SCALE = "Scale velocity";
		PRNL_STOP_AT_FLIP = "Stop at flip";
		PRNL_SINCE_START_SYSTEM = "Since start system";

		// ScaleVelocity: DynamicAttribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ScaleVelocityAffector::DEFAULT_VELOCITY_SCALE);
		append(new properties::DynamicAttributeProperty(this, PRNL_VELOCITY_SCALE, PRNL_VELOCITY_SCALE, dynAttr));

		// Since Start System: bool
		append(new properties::BoolProperty(this, PRNL_SINCE_START_SYSTEM, PRNL_SINCE_START_SYSTEM, false));

		// Stop at flip: bool
		append(new properties::BoolProperty(this, PRNL_STOP_AT_FLIP, PRNL_STOP_AT_FLIP, false));
	}

	ScaleVelocityAffectorPropertyWindow::~ScaleVelocityAffectorPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
