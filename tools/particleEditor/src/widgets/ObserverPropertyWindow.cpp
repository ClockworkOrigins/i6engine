#include "widgets/ObserverPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseObserver.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

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
		PRNL_OBSERVER_TYPE = QApplication::tr("Observer type");
		PRNL_OBSERVER_ENABLED = QApplication::tr("Enabled");
		PRNL_UNTIL_EVENT = QApplication::tr("Observer until event");
		PRNL_OBSERVE_INTERVAL = QApplication::tr("Observe interval");
		PRNL_NAME = QApplication::tr("Name");
		PRNL_PARTICLE_TYPE = QApplication::tr("Particle type");
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
		append(new properties::DoubleProperty(this, PRNL_OBSERVE_INTERVAL, PRNL_OBSERVE_INTERVAL, 0.0));

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

	void ObserverPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToObserver(prop, name);
		emit notifyChanged();
	}

	void ObserverPropertyWindow::copyAttributeToObserver(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(_owner->getPUElement());
		if (!observer) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			_owner->setName(prop->getString());
			_owner->setCaption();
			observer->setName(prop->getString().toStdString());
		} else if (propertyName == PRNL_OBSERVER_TYPE) {
			// Type: List of types
			// This requires the observer to be replaced.
			replaceObserverType(prop);
		} else if (propertyName == PRNL_PARTICLE_TYPE) {
			// Observe Particle Type: List
			QString observeType = prop->getEnumString();
			if (observeType == PT_VISUAL) {
				observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_VISUAL);
			} else if (observeType == PT_EMITTER) {
				observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_EMITTER);
			} else if (observeType == PT_AFFECTOR) {
				observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_AFFECTOR);
			} else if (observeType == PT_TECHNIQUE) {
				observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_TECHNIQUE);
			} else if (observeType == PT_SYSTEM) {
				observer->setParticleTypeToObserve(ParticleUniverse::Particle::PT_SYSTEM);
			}
		} else if (propertyName == PRNL_OBSERVER_ENABLED) {
			// Enabled: Bool
			observer->_resetEnabled();
			observer->setEnabled(prop->getBool());
		} else if (propertyName == PRNL_OBSERVE_INTERVAL) {
			// Observe Interval: ParticleUniverse::Real
			observer->setObserverInterval(prop->getDouble());
		} else if (propertyName == PRNL_UNTIL_EVENT) {
			// Observe Until Event: bool
			observer->setObserveUntilEvent(prop->getBool());
		}
	}

	void ObserverPropertyWindow::replaceObserverType(properties::Property * prop) {
		// Type: List of types
		Ogre::String type = prop->getEnumString().toStdString();
		if (type == Ogre::StringUtil::BLANK) {
			return;
		}

		ParticleUniverse::ParticleObserver * oldObserver = static_cast<ParticleUniverse::ParticleObserver *>(_owner->getPUElement());
		if (type == oldObserver->getObserverType()) {
			return;
		}
		if (oldObserver) {
			ParticleUniverse::ParticleTechnique * technique = oldObserver->getParentTechnique();
			if (technique) {
				ParticleUniverse::ParticleObserver * newObserver = technique->createObserver(type);
				oldObserver->copyParentAttributesTo(newObserver);
				copyHandlersTo(oldObserver, newObserver);
				bool wasStarted = false;
				ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
					wasStarted = true;
					system->stop();
				}
				technique->destroyObserver(oldObserver);
				_owner->setPUElement(newObserver);
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old observer didn't have a technique, so create a new observer by means of the ParticleSystemManager itself and also delete
				the old observer by means of the ParticleSystemManager
				*/
				ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
				ParticleUniverse::ParticleObserver * newObserver = particleSystemManager->createObserver(type);
				oldObserver->copyParentAttributesTo(newObserver);
				copyHandlersTo(oldObserver, newObserver);
				particleSystemManager->destroyObserver(oldObserver);
				_owner->setPUElement(newObserver);
			}
		} else {
			// There is no old observer. Create a new observer by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleObserver * newObserver = particleSystemManager->createObserver(type);
			_owner->setPUElement(newObserver);
		}
		emit replacePropertyWindow(QString::fromStdString(type));
	}

	void ObserverPropertyWindow::copyHandlersTo(ParticleUniverse::ParticleObserver * oldObserver, ParticleUniverse::ParticleObserver * newObserver) {
		// First delete all handlers that where created during the copy action
		newObserver->destroyAllEventHandlers();

		// Move the handlers from the old observer to the new one
		ParticleUniverse::ParticleEventHandler * handler;
		while (oldObserver->getNumEventHandlers() > 0) {
			handler = oldObserver->getEventHandler(0);
			oldObserver->removeEventHandler(handler);
			newObserver->addEventHandler(handler);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
