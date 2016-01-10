#include "widgets/ObserverPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ObserverPropertyWindow::ObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_OBSERVER_ON_CLEAR = OBSERVER_ON_CLEAR;
		CST_OBSERVER_ON_COLLISION = OBSERVER_ON_COLLISION;
		CST_OBSERVER_ON_COUNT = OBSERVER_ON_COUNT;
		CST_OBSERVER_ON_EMISSION = OBSERVER_ON_EMISSION;
		CST_OBSERVER_ON_EVENTFLAG = OBSERVER_ON_EVENTFLAG;
		CST_OBSERVER_ON_EXPIRE = OBSERVER_ON_EXPIRE;
		CST_OBSERVER_ON_POSITION = OBSERVER_ON_POSITION;
		CST_OBSERVER_ON_QUOTA = OBSERVER_ON_QUOTA;
		CST_OBSERVER_ON_RANDOM = OBSERVER_ON_RANDOM;
		CST_OBSERVER_ON_TIME = OBSERVER_ON_TIME;
		CST_OBSERVER_ON_VELOCITY = OBSERVER_ON_VELOCITY;
		PRNL_OBSERVER_TYPE = "Observer type";
		PRNL_OBSERVER_ENABLED = "Enabled";
		PRNL_UNTIL_EVENT = "Observer until event";
		PRNL_OBSERVE_INTERVAL = "Observe interval";
		PRNL_NAME = "Name";
		PRNL_PARTICLE_TYPE = "Particle type";
		PT_ALL = "All types";
		PT_VISUAL = "Visual";
		PT_EMITTER = "Emitter";
		PT_AFFECTOR = "Affector";
		PT_SYSTEM = "System";
		PT_TECHNIQUE = "Technique";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create an observer)
		QStringList types;
		types.append(CST_OBSERVER_ON_CLEAR);
		types.append(CST_OBSERVER_ON_COLLISION);
		types.append(CST_OBSERVER_ON_COUNT);
		types.append(CST_OBSERVER_ON_EMISSION);
		types.append(CST_OBSERVER_ON_EVENTFLAG);
		types.append(CST_OBSERVER_ON_EXPIRE);
		types.append(CST_OBSERVER_ON_POSITION);
		types.append(CST_OBSERVER_ON_QUOTA);
		types.append(CST_OBSERVER_ON_RANDOM);
		types.append(CST_OBSERVER_ON_TIME);
		types.append(CST_OBSERVER_ON_VELOCITY);
		append(new properties::EnumProperty(this, PRNL_OBSERVER_TYPE, PRNL_OBSERVER_TYPE, types));

		// Enabled: bool
		append(new properties::BoolProperty(this, PRNL_OBSERVER_ENABLED, PRNL_OBSERVER_ENABLED, ParticleUniverse::ParticleObserver::DEFAULT_ENABLED));

		// Observe Particle Type: List
		QStringList particleTypes;
		particleTypes.append(PT_ALL);
		particleTypes.append(PT_VISUAL);
		particleTypes.append(PT_EMITTER);
		particleTypes.append(PT_AFFECTOR);
		particleTypes.append(PT_SYSTEM);
		particleTypes.append(PT_TECHNIQUE);
		append(new properties::EnumProperty(this, PRNL_PARTICLE_TYPE, PRNL_PARTICLE_TYPE, particleTypes));

		// Observe Interval: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_OBSERVE_INTERVAL, PRNL_OBSERVE_INTERVAL, 0.0f));

		// Observe Until Event: bool
		append(new properties::BoolProperty(this, PRNL_UNTIL_EVENT, PRNL_UNTIL_EVENT, ParticleUniverse::ParticleObserver::DEFAULT_UNTIL_EVENT));
	}

	ObserverPropertyWindow::~ObserverPropertyWindow() {
	}

	void ObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(observer->getName()));

		// Type: List of types
		setEnumString(PRNL_OBSERVER_TYPE, QString::fromStdString(observer->getObserverType()));

		// Enabled: bool
		setBool(PRNL_OBSERVER_ENABLED, observer->isEnabled());

		// Observe Particle Type: List
		QString pType = "";
		if (observer->isParticleTypeToObserveSet()) {
			ParticleUniverse::Particle::ParticleType particleType = observer->getParticleTypeToObserve();
			switch (particleType) {
			case ParticleUniverse::Particle::PT_VISUAL: {
				pType = PT_VISUAL;
				break;
			}
			case ParticleUniverse::Particle::PT_EMITTER: {
				pType = PT_EMITTER;
				break;
			}
			case ParticleUniverse::Particle::PT_AFFECTOR: {
				pType = PT_AFFECTOR;
				break;
			}
			case ParticleUniverse::Particle::PT_TECHNIQUE: {
				pType = PT_TECHNIQUE;
				break;
			}
			case ParticleUniverse::Particle::PT_SYSTEM: {
				pType = PT_SYSTEM;
				break;
			}
			default: {
				break;
			}
			}
		} else {
			pType = PT_ALL;
		}
		setEnumString(PRNL_PARTICLE_TYPE, pType);

		// Observe Interval: ParticleUniverse::Real
		setDouble(PRNL_OBSERVE_INTERVAL, observer->getObserverInterval());

		// Observe Until Event: bool
		setBool(PRNL_UNTIL_EVENT, observer->getObserveUntilEvent());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
