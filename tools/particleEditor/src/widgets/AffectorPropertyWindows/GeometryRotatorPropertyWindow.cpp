#include "widgets/AffectorPropertyWindows/GeometryRotatorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseGeometryRotator.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GeometryRotatorPropertyWindow::GeometryRotatorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ROTATION_SPEED = "Rotation speed";
		PRNL_ROTATION_AXIS = "Rotation axis";
		PRNL_USE_OWN_ROTATION = "Use own rotation";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Use own rotation speed: bool
		append(new properties::BoolProperty(this, PRNL_USE_OWN_ROTATION, PRNL_USE_OWN_ROTATION, ParticleUniverse::GeometryRotator::DEFAULT_USE_OWN));

		// Rotation speed: DynamicAttribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::GeometryRotator::DEFAULT_ROTATION_SPEED);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr));

		// Rotation axis: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_ROTATION_AXIS, PRNL_ROTATION_AXIS, ParticleUniverse::GeometryRotator::DEFAULT_ROTATION_AXIS));
	}

	GeometryRotatorPropertyWindow::~GeometryRotatorPropertyWindow() {
	}

	void GeometryRotatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::GeometryRotator * geometryRotator = static_cast<ParticleUniverse::GeometryRotator *>(affector);

		// Use own rotation speed: bool
		setBool(PRNL_USE_OWN_ROTATION, geometryRotator->useOwnRotationSpeed());

		// Rotation speed: DynamicAttribute
		setDynamicAttribute(PRNL_ROTATION_SPEED, geometryRotator->getRotationSpeed());

		// Rotation axis: Ogre::Vector3
		setVector3(PRNL_ROTATION_AXIS, geometryRotator->getRotationAxis());
	}

	void GeometryRotatorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::GeometryRotator * affector = static_cast<ParticleUniverse::GeometryRotator *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_USE_OWN_ROTATION) {
			// Use own rotation speed: bool
			affector->setUseOwnRotationSpeed(prop->getBool());
		} else if (propertyName == PRNL_ROTATION_SPEED) {
			// Rotation speed: DynamicAttribute
			affector->setRotationSpeed(prop->getDynamicAttribute());
			restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		} else if (propertyName == PRNL_ROTATION_AXIS) {
			// Rotation axis: Ogre::Vector3
			affector->setRotationAxis(prop->getVector3());
			if (prop->getVector3() == Ogre::Vector3::ZERO) {
				// Force to default state
				affector->resetRotationAxis();
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
