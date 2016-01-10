#include "widgets/AffectorPropertyWindows/SineForceAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseSineForceAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	SineForceAffectorPropertyWindow::SineForceAffectorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_FREQ_MIN = "Minimum frequency";
		PRNL_FREQ_MAX = "Maximum frequency";
		PRNL_FORCE_VECTOR = "Force vector";
		PRNL_LINEAR_FORCE_APPLICATION = "Application";
		APP_ADD = "Add";
		APP_AVG = "Average";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Force vector: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_FORCE_VECTOR, PRNL_FORCE_VECTOR, ParticleUniverse::SineForceAffector::DEFAULT_FORCE_VECTOR));

		// Force Application: 
		QStringList application;
		application.append(APP_ADD);
		application.append(APP_AVG);
		append(new properties::EnumProperty(this, PRNL_LINEAR_FORCE_APPLICATION, PRNL_LINEAR_FORCE_APPLICATION, application));

		// Minimum Frequency: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_FREQ_MIN, PRNL_FREQ_MIN, ParticleUniverse::SineForceAffector::DEFAULT_FREQ_MIN));

		// Maximum Frequency: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_FREQ_MAX, PRNL_FREQ_MAX, ParticleUniverse::SineForceAffector::DEFAULT_FREQ_MAX));
	}

	SineForceAffectorPropertyWindow::~SineForceAffectorPropertyWindow() {
	}

	void SineForceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::SineForceAffector * sineForceAffector = static_cast<ParticleUniverse::SineForceAffector *>(affector);

		// Force vector: Ogre::Vector3
		setVector3(PRNL_FORCE_VECTOR, sineForceAffector->getForceVector());

		// Force Application: List
		ParticleUniverse::BaseForceAffector::ForceApplication application = sineForceAffector->getForceApplication();
		QString applicationString = APP_ADD;
		if (application == ParticleUniverse::BaseForceAffector::FA_AVERAGE) {
			applicationString = APP_AVG;
		}
		setEnumString(PRNL_LINEAR_FORCE_APPLICATION, applicationString);

		// Minimum Frequency: ParticleUniverse::Real
		setDouble(PRNL_FREQ_MIN, sineForceAffector->getFrequencyMin());

		// Maximum Frequency: ParticleUniverse::Real
		setDouble(PRNL_FREQ_MAX, sineForceAffector->getFrequencyMax());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
