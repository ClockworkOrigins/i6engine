#include "widgets/AffectorPropertyWindows/ScaleAffectorPropertyWindow.h"

#include <cfloat>

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseScaleAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ScaleAffectorPropertyWindow::ScaleAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_XYZ_SCALE = QApplication::tr("Scale x, y and z");
		PRNL_X_SCALE = QApplication::tr("Scale x");
		PRNL_Y_SCALE = QApplication::tr("Scale y");
		PRNL_Z_SCALE = QApplication::tr("Scale z");
		PRNL_SINCE_START_SYSTEM = QApplication::tr("Since start system");

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

	void ScaleAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ScaleAffector * affector = static_cast<ParticleUniverse::ScaleAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_XYZ_SCALE) {
			// ScaleXYZ: DynamicAttribute
			affector->resetDynScaleXYZ(false); // Value has changed, so the flag is set internally in PU.
			affector->setDynScaleXYZ(prop->getDynamicAttribute());
			ParticleUniverse::DynamicAttribute * dynAttr = affector->getDynScaleXYZ();
			if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && std::abs(dynAttr->getValue() - ParticleUniverse::ScaleAffector::DEFAULT_XYZ_SCALE) < DBL_EPSILON) {
				// Force default state; internal flag in PU must be reset, which isn't done if the value is changed.
				affector->resetDynScaleXYZ(true);
			}

			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else if (propertyName == PRNL_X_SCALE) {
			// ScaleX: DynamicAttribute
			affector->resetDynScaleX(false);
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				affector->setDynScaleX(dynAttr);
			}
			dynAttr = affector->getDynScaleX();
			if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && std::abs(dynAttr->getValue() - ParticleUniverse::ScaleAffector::DEFAULT_X_SCALE) < DBL_EPSILON) {
				// Force default state
				affector->resetDynScaleX(true);
			}

			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else if (propertyName == PRNL_Y_SCALE) {
			// ScaleY: DynamicAttribute
			affector->resetDynScaleY(false);
			ParticleUniverse::DynamicAttribute* dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				affector->setDynScaleY(dynAttr);
			}
			dynAttr = affector->getDynScaleY();
			if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && std::abs(dynAttr->getValue() - ParticleUniverse::ScaleAffector::DEFAULT_Y_SCALE) < DBL_EPSILON) {
				// Force default state
				affector->resetDynScaleY(true);
			}

			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else if (propertyName == PRNL_Z_SCALE) {
			// ScaleZ: DynamicAttribute
			affector->resetDynScaleZ(false);
			ParticleUniverse::DynamicAttribute* dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				affector->setDynScaleZ(dynAttr);
			}
			dynAttr = affector->getDynScaleZ();
			if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && std::abs(dynAttr->getValue() - ParticleUniverse::ScaleAffector::DEFAULT_Z_SCALE) < DBL_EPSILON) {
				// Force default state
				affector->resetDynScaleZ(true);
			}

			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else if (propertyName == PRNL_SINCE_START_SYSTEM) {
			// Since Start System: bool
			affector->setSinceStartSystem(prop->getBool());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
