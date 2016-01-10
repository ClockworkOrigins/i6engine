#include "widgets/EventHandlerPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseObserver.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	EventHandlerPropertyWindow::EventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_HANDLER_DO_AFFECTOR = HANDLER_DO_AFFECTOR;
		CST_HANDLER_DO_ENABLE_COMPONENT = HANDLER_DO_ENABLE_COMPONENT;
		CST_HANDLER_DO_EXPIRE = HANDLER_DO_EXPIRE;
		CST_HANDLER_DO_FREEZE = HANDLER_DO_FREEZE;
		CST_HANDLER_DO_PLACEMENT_PARTICLE = HANDLER_DO_PLACEMENT_PARTICLE;
		CST_HANDLER_DO_SCALE = HANDLER_DO_SCALE;
		CST_HANDLER_DO_STOP_SYSTEM = HANDLER_DO_STOP_SYSTEM;
		PRNL_HANDLER_TYPE = "Event handler type";
		PRNL_NAME = "Name";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create a handler)
		QStringList types;
		types.append(CST_HANDLER_DO_AFFECTOR);
		types.append(CST_HANDLER_DO_ENABLE_COMPONENT);
		types.append(CST_HANDLER_DO_EXPIRE);
		types.append(CST_HANDLER_DO_FREEZE);
		types.append(CST_HANDLER_DO_PLACEMENT_PARTICLE);
		types.append(CST_HANDLER_DO_SCALE);
		types.append(CST_HANDLER_DO_STOP_SYSTEM);
		append(new properties::EnumProperty(this, PRNL_HANDLER_TYPE, PRNL_HANDLER_TYPE, types));
	}

	EventHandlerPropertyWindow::~EventHandlerPropertyWindow() {
	}

	void EventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(eventHandler->getName()));

		// Type: List of types
		setEnumString(PRNL_HANDLER_TYPE, QString::fromStdString(eventHandler->getEventHandlerType()));
	}

	void EventHandlerPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToEventHandler(prop, name);
		emit notifyChanged();
	}

	void EventHandlerPropertyWindow::copyAttributeToEventHandler(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleEventHandler * handler = static_cast<ParticleUniverse::ParticleEventHandler *>(_owner->getPUElement());
		if (!handler) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			_owner->setName(prop->getString());
			_owner->setCaption();
			handler->setName(prop->getString().toStdString());
		} else if (propertyName == PRNL_HANDLER_TYPE) {
			// Type: List of types
			// This requires the handler to be replaced.
			replaceHandlerType(prop);
		}
	}

	void EventHandlerPropertyWindow::replaceHandlerType(properties::Property * prop) {
		// Type: List of types
		Ogre::String type = prop->getEnumString().toStdString();
		if (type == Ogre::StringUtil::BLANK) {
			return;
		}

		ParticleUniverse::ParticleEventHandler * oldHandler = static_cast<ParticleUniverse::ParticleEventHandler *>(_owner->getPUElement());
		if (type == oldHandler->getEventHandlerType()) {
			return;
		}
		if (oldHandler) {
			ParticleUniverse::ParticleObserver * observer = oldHandler->getParentObserver();
			if (observer) {
				ParticleUniverse::ParticleEventHandler * newHandler = observer->createEventHandler(type);
				oldHandler->copyParentAttributesTo(newHandler);
				bool wasStarted = false;
				ParticleUniverse::ParticleTechnique * technique = observer->getParentTechnique();
				ParticleUniverse::ParticleSystem * system = nullptr;
				if (technique) {
					system = technique->getParentSystem();
					if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
						wasStarted = true;
						system->stop();
					}
				}
				observer->destroyEventHandler(oldHandler);
				_owner->setPUElement(newHandler);
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old handler didn't have an observer, so create a new handler by means of the ParticleSystemManager itself and also
				delete the old handler by means of the ParticleSystemManager
				*/
				ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
				ParticleUniverse::ParticleEventHandler * newHandler = particleSystemManager->createEventHandler(type);
				oldHandler->copyParentAttributesTo(newHandler);
				particleSystemManager->destroyEventHandler(oldHandler);
				_owner->setPUElement(newHandler);
			}
		} else {
			// There is no old handler. Create a new handler by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleEventHandler * newHandler = particleSystemManager->createEventHandler(type);
			_owner->setPUElement(newHandler);
		}
		emit replacePropertyWindow(QString::fromStdString(type));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
