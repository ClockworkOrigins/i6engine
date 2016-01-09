#include "widgets/AffectorPropertyWindows/GeometryRotatorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseGeometryRotator.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	GeometryRotatorPropertyWindow::GeometryRotatorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
