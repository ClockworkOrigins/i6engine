#include "widgets/AffectorPropertyWindows/VortexAffectorPropertyWindow.h"

#include "properties/DynamicAttributeProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseVortexAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	VortexAffectorPropertyWindow::VortexAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_ROTATION = "Rotation";
		PRNL_ROTATION_AXIS = "Rotation axis";
		PRNL_ROTATION_SPEED = "Rotation speed";

		// Rotation Vector: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_ROTATION_AXIS, PRNL_ROTATION_AXIS, ParticleUniverse::VortexAffector::DEFAULT_ROTATION_VECTOR));

		// Rotation Speed: Dynamic Attribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::VortexAffector::DEFAULT_ROTATION_SPEED);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr));
	}

	VortexAffectorPropertyWindow::~VortexAffectorPropertyWindow() {
	}

	void VortexAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::VortexAffector * vortexAffector = static_cast<ParticleUniverse::VortexAffector *>(affector);

		// Rotation Vector: Ogre::Vector3
		setVector3(PRNL_ROTATION_AXIS, vortexAffector->getRotationVector());

		// Rotation Speed: Dynamic Attribute
		setDynamicAttribute(PRNL_ROTATION_SPEED, vortexAffector->getRotationSpeed());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
