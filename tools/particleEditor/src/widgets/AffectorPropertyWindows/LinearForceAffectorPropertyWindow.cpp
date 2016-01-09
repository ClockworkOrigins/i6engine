#include "widgets/AffectorPropertyWindows/LinearForceAffectorPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseLinearForceAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	LinearForceAffectorPropertyWindow::LinearForceAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_FORCE_VECTOR = "Force vector";
		PRNL_LINEAR_FORCE_APPLICATION = "Application";
		APP_ADD = "Add";
		APP_AVG = "Average";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Force vector: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_FORCE_VECTOR, PRNL_FORCE_VECTOR, ParticleUniverse::LinearForceAffector::DEFAULT_FORCE_VECTOR));

		// Force Application: List
		QStringList application;
		application.append(APP_ADD);
		application.append(APP_AVG);
		append(new properties::EnumProperty(this, PRNL_LINEAR_FORCE_APPLICATION, PRNL_LINEAR_FORCE_APPLICATION, application));
	}

	LinearForceAffectorPropertyWindow::~LinearForceAffectorPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
