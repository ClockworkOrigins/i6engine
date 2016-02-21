#include "widgets/AffectorPropertyWindows/ScaleVelocityAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseScaleVelocityAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ScaleVelocityAffectorPropertyWindow::ScaleVelocityAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_VELOCITY_SCALE = QApplication::tr("Scale velocity");
		PRNL_STOP_AT_FLIP = QApplication::tr("Stop at flip");
		PRNL_SINCE_START_SYSTEM = QApplication::tr("Since start system");

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

	void ScaleVelocityAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ScaleVelocityAffector * scaleVelocityAffector = static_cast<ParticleUniverse::ScaleVelocityAffector *>(affector);

		// ScaleVelocity: DynamicAttribute
		setDynamicAttribute(PRNL_VELOCITY_SCALE, scaleVelocityAffector->getDynScaleVelocity());

		// Since Start System: bool
		setBool(PRNL_SINCE_START_SYSTEM, scaleVelocityAffector->isSinceStartSystem());

		// Stop at flip: bool
		setBool(PRNL_STOP_AT_FLIP, scaleVelocityAffector->isStopAtFlip());
	}

	void ScaleVelocityAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ScaleVelocityAffector * affector = static_cast<ParticleUniverse::ScaleVelocityAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_VELOCITY_SCALE) {
			// ScaleVelocity: DynamicAttribute
			affector->resetDynScaleVelocity(false);
			ParticleUniverse::DynamicAttribute* dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				affector->setDynScaleVelocity(dynAttr);
			}
			dynAttr = affector->getDynScaleVelocity();
			if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && std::abs(dynAttr->getValue() - ParticleUniverse::ScaleVelocityAffector::DEFAULT_VELOCITY_SCALE) < DBL_EPSILON) {
				// Force default state
				affector->resetDynScaleVelocity(true);
			}

			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else if (propertyName == PRNL_SINCE_START_SYSTEM) {
			// Since Start System: bool
			affector->setSinceStartSystem(prop->getBool());
		} else if (propertyName == PRNL_STOP_AT_FLIP) {
			// Stop at flip: bool
			affector->setStopAtFlip(prop->getBool());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
