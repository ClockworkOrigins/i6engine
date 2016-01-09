#include "widgets/ExternPropertyWindows/VortexExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/DynamicAttributeProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseVortexAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	VortexExternPropertyWindow::VortexExternPropertyWindow(QWidget * par, QString name) : ExternPropertyWindow(par, name) {
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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
