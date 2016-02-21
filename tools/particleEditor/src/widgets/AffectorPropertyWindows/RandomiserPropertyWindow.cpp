#include "widgets/AffectorPropertyWindows/RandomiserPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseRandomiser.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	RandomiserPropertyWindow::RandomiserPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_RANDOM_DIRECTION = QApplication::tr("Random direction");
		PRNL_MAX_DEVIATION = QApplication::tr("Maximum deviation");
		PRNL_TIME_STEP = QApplication::tr("Time step");

		// Max. deviation: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_MAX_DEVIATION, PRNL_MAX_DEVIATION, ParticleUniverse::Randomiser::DEFAULT_MAX_DEVIATION));

		// Random direction: bool
		append(new properties::BoolProperty(this, PRNL_RANDOM_DIRECTION, PRNL_RANDOM_DIRECTION, ParticleUniverse::Randomiser::DEFAULT_RANDOM_DIRECTION));

		// Timestep: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::Randomiser::DEFAULT_TIME_STEP));
	}

	RandomiserPropertyWindow::~RandomiserPropertyWindow() {
	}

	void RandomiserPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::Randomiser * randomiser = static_cast<ParticleUniverse::Randomiser *>(affector);

		// Max. deviation: Ogre::Vector3
		setVector3(PRNL_MAX_DEVIATION, Ogre::Vector3(randomiser->getMaxDeviationX(), randomiser->getMaxDeviationY(), randomiser->getMaxDeviationZ()));

		// Random direction: bool
		setBool(PRNL_RANDOM_DIRECTION, randomiser->isRandomDirection());

		// Timestep: ParticleUniverse::Real
		setDouble(PRNL_TIME_STEP, randomiser->getTimeStep());
	}

	void RandomiserPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::Randomiser* affector = static_cast<ParticleUniverse::Randomiser*>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_MAX_DEVIATION) {
			// Max. deviation: Ogre::Vector3
			Ogre::Vector3 v3 = prop->getVector3();
			affector->setMaxDeviationX(v3.x);
			affector->setMaxDeviationY(v3.y);
			affector->setMaxDeviationZ(v3.z);
		} else if (propertyName == PRNL_RANDOM_DIRECTION) {
			// Random direction: bool
			affector->setRandomDirection(prop->getBool());
		} else if (propertyName == PRNL_TIME_STEP) {
			// Timestep: ParticleUniverse::Real
			affector->setTimeStep(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
