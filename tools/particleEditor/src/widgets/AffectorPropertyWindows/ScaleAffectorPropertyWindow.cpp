#include "widgets/AffectorPropertyWindows/ScaleAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "ParticleAffectors/ParticleUniverseScaleAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ScaleAffectorPropertyWindow::ScaleAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_XYZ_SCALE = "Scale x, y and z";
		PRNL_X_SCALE = "Scale x";
		PRNL_Y_SCALE = "Scale y";
		PRNL_Z_SCALE = "Scale z";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_SINCE_START_SYSTEM = "Since start system";

		// ScaleXYZ: DynamicAttribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ScaleAffector::DEFAULT_XYZ_SCALE);
		append(new properties::DynamicAttributeProperty(this, PRNL_XYZ_SCALE, PRNL_XYZ_SCALE, dynAttr));

		// ScaleX: DynamicAttribute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ScaleAffector::DEFAULT_X_SCALE);
		append(new properties::DynamicAttributeProperty(this, PRNL_X_SCALE, PRNL_X_SCALE, dynAttr));

		// ScaleY: DynamicAttribute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ScaleAffector::DEFAULT_Y_SCALE);
		append(new properties::DynamicAttributeProperty(this, PRNL_Y_SCALE, PRNL_Y_SCALE, dynAttr));

		// ScaleZ: DynamicAttribute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ScaleAffector::DEFAULT_Z_SCALE);
		append(new properties::DynamicAttributeProperty(this, PRNL_Z_SCALE, PRNL_Z_SCALE, dynAttr));

		// Since Start System: bool
		append(new properties::BoolProperty(this, PRNL_SINCE_START_SYSTEM, PRNL_SINCE_START_SYSTEM, false));
	}

	ScaleAffectorPropertyWindow::~ScaleAffectorPropertyWindow() {
	}

	void ScaleAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ScaleAffector * scaleAffector = static_cast<ParticleUniverse::ScaleAffector *>(affector);

		// ScaleXYZ: DynamicAttribute
		setDynamicAttribute(PRNL_XYZ_SCALE, scaleAffector->getDynScaleXYZ());

		// ScaleX: DynamicAttribute
		setDynamicAttribute(PRNL_X_SCALE, scaleAffector->getDynScaleX());

		// ScaleY: DynamicAttribute
		setDynamicAttribute(PRNL_Y_SCALE, scaleAffector->getDynScaleY());

		// ScaleZ: DynamicAttribute
		setDynamicAttribute(PRNL_Z_SCALE, scaleAffector->getDynScaleZ());

		// Since Start System: bool
		setBool(PRNL_SINCE_START_SYSTEM, scaleAffector->isSinceStartSystem());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
