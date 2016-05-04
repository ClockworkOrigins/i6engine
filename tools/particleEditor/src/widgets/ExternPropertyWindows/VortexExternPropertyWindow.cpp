#include "widgets/ExternPropertyWindows/VortexExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/DynamicAttributeProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "Externs/ParticleUniverseVortexExtern.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	VortexExternPropertyWindow::VortexExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ExternPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		// TODO
		PRNL_ROTATION_SPEED = QApplication::tr("Rotation speed");
		PRNL_ROTATION_AXIS = QApplication::tr("Rotation axis");
		PRNL_EXTERN_THRESHOLD = QApplication::tr("Distance threshold");

		// Distance Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<double>::max()));

		// Rotation Vector: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_ROTATION_AXIS, PRNL_ROTATION_AXIS, ParticleUniverse::VortexAffector::DEFAULT_ROTATION_VECTOR));

		// Rotation Speed: Dynamic Attribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::VortexAffector::DEFAULT_ROTATION_SPEED);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr));
	}

	VortexExternPropertyWindow::~VortexExternPropertyWindow() {
	}

	void VortexExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern * externObject) {
		ExternPropertyWindow::copyAttributesFromExtern(externObject);

		// Copy properties from Extern to property window
		ParticleUniverse::VortexExtern * vortexExtern = static_cast<ParticleUniverse::VortexExtern *>(externObject);

		// Distance Threshold: ParticleUniverse::Real
		setDouble(PRNL_EXTERN_THRESHOLD, vortexExtern->getDistanceThreshold());

		// Rotation Vector: Ogre::Vector3
		setVector3(PRNL_ROTATION_AXIS, vortexExtern->getRotationVector());

		// Rotation Speed: Dynamic Attribute
		setDynamicAttribute(PRNL_ROTATION_SPEED, vortexExtern->getRotationSpeed());
	}

	void VortexExternPropertyWindow::copyAttributeToExtern(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::Extern * ext = static_cast<ParticleUniverse::Extern *>(_owner->getPUElement());
		ParticleUniverse::VortexExtern * externObject = static_cast<ParticleUniverse::VortexExtern *>(ext);

		if (propertyName == PRNL_EXTERN_THRESHOLD) {
			// Distance Threshold: ParticleUniverse::Real
			ParticleUniverse::Attachable * attachable = static_cast<ParticleUniverse::Attachable *>(_owner->getPUElement());
			attachable->setDistanceThreshold(prop->getDouble());
		} else if (propertyName == PRNL_ROTATION_AXIS) {
			// Rotation Vector: Ogre::Vector3
			externObject->setRotationVector(prop->getVector3());
		} else if (propertyName == PRNL_ROTATION_SPEED) {
			// Rotation speed: DynamicAttribute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				externObject->setRotationSpeed(dynAttr);
			}
		} else {
			// Update extern with another attribute
			ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
