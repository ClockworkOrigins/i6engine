#include "widgets/ExternPropertyWindows/VortexExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/DynamicAttributeProperty.h"
#include "properties/Vec3Property.h"

#include "Externs/ParticleUniverseVortexExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	VortexExternPropertyWindow::VortexExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ExternPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		// TODO
		PRNL_ROTATION_SPEED = "Rotation speed";
		PRNL_ROTATION_AXIS = "Rotation axis";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_EXTERN_THRESHOLD = "Distance threshold";

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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
