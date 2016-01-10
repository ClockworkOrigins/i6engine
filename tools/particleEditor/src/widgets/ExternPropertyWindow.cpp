#include "widgets/ExternPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseExtern.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ExternPropertyWindow::ExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_EXTERN_BOX_COLLIDER = EXTERN_BOX_COLLIDER;
		CST_EXTERN_SPHERE_COLLIDER = EXTERN_SPHERE_COLLIDER;
		CST_EXTERN_GRAVITY = EXTERN_GRAVITY;
		CST_EXTERN_PHYSX_ACTOR = EXTERN_PHYSX_ACTOR;
		CST_EXTERN_PHYSX_FLUID = EXTERN_PHYSX_FLUID;
		CST_EXTERN_VORTEX = EXTERN_VORTEX;
		PRNL_EXTERN_TYPE = "Extern type";
		PRNL_NAME = "Name";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create an extern)
		QStringList types;
		types.append(CST_EXTERN_BOX_COLLIDER);
		types.append(CST_EXTERN_GRAVITY);
		types.append(CST_EXTERN_SPHERE_COLLIDER);
		types.append(CST_EXTERN_VORTEX);
		append(new properties::EnumProperty(this, PRNL_EXTERN_TYPE, PRNL_EXTERN_TYPE, types));
	}

	ExternPropertyWindow::~ExternPropertyWindow() {
	}

	void ExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern * externObject) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(externObject->getName()));

		// Type: List of types
		setEnumString(PRNL_EXTERN_TYPE, QString::fromStdString(externObject->getExternType()));
	}

	void ExternPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToExtern(prop, name);
		//notifyPropertyChanged();
	}

	void ExternPropertyWindow::copyAttributeToExtern(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::Extern * externObject = static_cast<ParticleUniverse::Extern *>(_owner->getPUElement());
		if (!externObject) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			_owner->setName(prop->getString());
			_owner->setCaption();
			externObject->setName(prop->getString().toStdString());
		} else if (propertyName == PRNL_EXTERN_TYPE) {
			// Type: List of types
			// This requires the extern to be replaced.
			replaceExternType(prop);
		}
	}

	void ExternPropertyWindow::replaceExternType(properties::Property * prop) {
		// Type: List of types
		Ogre::String type = prop->getEnumString().toStdString();
		if (type == Ogre::StringUtil::BLANK) {
			return;
		}

		ParticleUniverse::Extern * oldExtern = static_cast<ParticleUniverse::Extern *>(_owner->getPUElement());
		if (type == oldExtern->getExternType()) {
			return;
		}
		if (oldExtern) {
			ParticleUniverse::ParticleTechnique * technique = oldExtern->getParentTechnique();
			if (technique) {
				ParticleUniverse::Extern * newExtern = technique->createExtern(type);
				oldExtern->copyParentAttributesTo(newExtern);
				bool wasStarted = false;
				ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
					wasStarted = true;
					system->stop();
				}
				technique->destroyExtern(oldExtern);
				_owner->setPUElement(newExtern);
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old extern didn't have a technique, so create a new extern by means of the ParticleSystemManager itself and also delete
				the old extern by means of the ParticleSystemManager
				*/
				ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
				ParticleUniverse::Extern * newExtern = particleSystemManager->createExtern(type);
				oldExtern->copyParentAttributesTo(newExtern);
				particleSystemManager->destroyExtern(oldExtern);
				_owner->setPUElement(newExtern);
			}
		} else {
			// There is no old extern. Create a new extern by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::Extern * newExtern = particleSystemManager->createExtern(type);
			_owner->setPUElement(newExtern);
		}
		emit replacePropertyWindow(QString::fromStdString(type));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
