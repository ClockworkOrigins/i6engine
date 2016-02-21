#include "widgets/BehaviourPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseBehaviour.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BehaviourPropertyWindow::BehaviourPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_BEHAVIOUR_SLAVE = BEHAVIOUR_SLAVE;
		PRNL_BEHAVIOUR_TYPE = QApplication::tr("Behaviour type");
		PRNL_NAME = QApplication::tr("Name");

		// Type
		QStringList types;
		types.append(CST_BEHAVIOUR_SLAVE);
		append(new properties::EnumProperty(this, PRNL_BEHAVIOUR_TYPE, PRNL_BEHAVIOUR_TYPE, types));
	}

	BehaviourPropertyWindow::~BehaviourPropertyWindow() {
	}

	void BehaviourPropertyWindow::copyAttributesFromBehaviour(ParticleUniverse::ParticleBehaviour * behaviour) {
		// Type: List of types
		setEnumString(PRNL_BEHAVIOUR_TYPE, QString::fromStdString(behaviour->getBehaviourType()));
	}

	void BehaviourPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToBehaviour(prop, name);
		emit notifyChanged();
	}

	void BehaviourPropertyWindow::copyAttributeToBehaviour(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleBehaviour * behaviour = static_cast<ParticleUniverse::ParticleBehaviour *>(_owner->getPUElement());
		if (!behaviour) {
			return;
		}

		if (propertyName == PRNL_BEHAVIOUR_TYPE) {
			// Type: List of types
			// This requires the behaviour to be replaced.
			replaceBehaviourType(prop);
		}
	}

	void BehaviourPropertyWindow::replaceBehaviourType(properties::Property * prop) {
		// Type: List of types
		Ogre::String type = prop->getEnumString().toStdString();
		if (type == Ogre::StringUtil::BLANK) {
			return;
		}

		ParticleUniverse::ParticleBehaviour * oldBehaviour = static_cast<ParticleUniverse::ParticleBehaviour *>(_owner->getPUElement());
		if (type == oldBehaviour->getBehaviourType()) {
			return;
		}
		if (oldBehaviour) {
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleBehaviour * newBehaviour = particleSystemManager->createBehaviour(type);
			oldBehaviour->copyParentAttributesTo(newBehaviour);
			ParticleUniverse::ParticleTechnique * technique = oldBehaviour->getParentTechnique();
			if (technique) {
				bool wasStarted = false;
				ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
					wasStarted = true;
					system->stop();
				}
				technique->_destroyBehaviourTemplate(oldBehaviour);
				technique->_addBehaviourTemplate(newBehaviour);
				_owner->setPUElement(newBehaviour);
				technique->_unprepareBehaviours();
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old behaviour didn't have a technique.
				*/
				particleSystemManager->destroyBehaviour(oldBehaviour);
				_owner->setPUElement(newBehaviour);
			}
		} else {
			// There is no old behaviour. Create a new behaviour by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleBehaviour * newBehaviour = particleSystemManager->createBehaviour(type);
			_owner->setPUElement(newBehaviour);
		}
		emit replacePropertyWindow(QString::fromStdString(type));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
