#include "widgets/AffectorPropertyWindows/LinearForceAffectorPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseLinearForceAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	LinearForceAffectorPropertyWindow::LinearForceAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_FORCE_VECTOR = QApplication::tr("Force vector");
		PRNL_LINEAR_FORCE_APPLICATION = QApplication::tr("Application");
		APP_ADD = "Add";
		APP_AVG = "Average";

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

	void LinearForceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::LinearForceAffector * linearForceAffector = static_cast<ParticleUniverse::LinearForceAffector *>(affector);

		// Force vector: Ogre::Vector3
		setVector3(PRNL_FORCE_VECTOR, linearForceAffector->getForceVector());

		// Force Application: List
		ParticleUniverse::BaseForceAffector::ForceApplication application = linearForceAffector->getForceApplication();
		QString applicationString = APP_ADD;
		if (application == ParticleUniverse::BaseForceAffector::FA_AVERAGE) {
			applicationString = APP_AVG;
		}
		setEnumString(PRNL_LINEAR_FORCE_APPLICATION, applicationString);
	}

	void LinearForceAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::LinearForceAffector * affector = static_cast<ParticleUniverse::LinearForceAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_FORCE_VECTOR) {
			// Force vector: Ogre::Vector3
			affector->setForceVector(prop->getVector3());
		} else if (propertyName == PRNL_LINEAR_FORCE_APPLICATION) {
			// Force Application: List
			QString application = prop->getEnumString();
			if (application == APP_ADD) {
				affector->setForceApplication(ParticleUniverse::BaseForceAffector::FA_ADD);
			} else if (application == APP_AVG) {
				affector->setForceApplication(ParticleUniverse::BaseForceAffector::FA_AVERAGE);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
