#include "widgets/AffectorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	AffectorPropertyWindow::AffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_AFFECTOR_ALIGN = AFFECTOR_ALIGN;
		CST_AFFECTOR_BOX_COLLIDER = AFFECTOR_BOX_COLLIDER;
		CST_AFFECTOR_COLLISION_AVOIDANCE = AFFECTOR_COLLISION_AVOIDANCE;
		CST_AFFECTOR_COLOUR = AFFECTOR_COLOUR;
		CST_AFFECTOR_FLOCK_CENTERING = AFFECTOR_FLOCK_CENTERING;
		CST_AFFECTOR_FORCEFIELD = AFFECTOR_FORCEFIELD;
		CST_AFFECTOR_GEOMETRY_ROTATOR = AFFECTOR_GEOMETRY_ROTATOR;
		CST_AFFECTOR_GRAVITY = AFFECTOR_GRAVITY;
		CST_AFFECTOR_INTER_PARTICLE_COLLIDER = AFFECTOR_INTER_PARTICLE_COLLIDER;
		CST_AFFECTOR_JET = AFFECTOR_JET;
		CST_AFFECTOR_LINE = AFFECTOR_LINE;
		CST_AFFECTOR_LINEAR_FORCE = AFFECTOR_LINEAR_FORCE;
		CST_AFFECTOR_PARTICLE_FOLLOWER = AFFECTOR_PARTICLE_FOLLOWER;
		CST_AFFECTOR_PATH_FOLLOWER = AFFECTOR_PATH_FOLLOWER;
		CST_AFFECTOR_PLANE_COLLIDER = AFFECTOR_PLANE_COLLIDER;
		CST_AFFECTOR_RANDOMISER = AFFECTOR_RANDOMISER;
		CST_AFFECTOR_SCALE = AFFECTOR_SCALE;
		CST_AFFECTOR_SCALE_VELOCITY = AFFECTOR_SCALE_VELOCITY;
		CST_AFFECTOR_SINE_FORCE = AFFECTOR_SINE_FORCE;
		CST_AFFECTOR_SPHERE_COLLIDER = AFFECTOR_SPHERE_COLLIDER;
		CST_AFFECTOR_TEXTURE_ANIMATOR = AFFECTOR_TEXTURE_ANIMATOR;
		CST_AFFECTOR_TEXTURE_ROTATOR = AFFECTOR_TEXTURE_ROTATOR;
		CST_AFFECTOR_VELOCITY_MATCHING = AFFECTOR_VELOCITY_MATCHING;
		CST_AFFECTOR_VORTEX = AFFECTOR_VORTEX;
		PRNL_NAME = "Name";
		PRNL_AFFECTOR_TYPE = "Affector type";
		PRNL_AFFECTOR_ENABLED = "Enabled";
		PRNL_AFFECTOR_POSITION = "Position";
		PRNL_AFFECTOR_MASS = "Mass";
		PRNL_AFFECTOR_SPECIALISATION = "Specialisation";
		PRNL_AFFECTOR_SPEC_DEFAULT = "Default";
		PRNL_AFFECTOR_SPEC_TT_INCREASE = "Increment by time to live";
		PRNL_AFFECTOR_SPEC_TT_DECREASE = "Decrement by time to live";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create an affector)
		QStringList affectorTypes;
		affectorTypes.append(CST_AFFECTOR_ALIGN);
		affectorTypes.append(CST_AFFECTOR_BOX_COLLIDER);
		affectorTypes.append(CST_AFFECTOR_COLLISION_AVOIDANCE);
		affectorTypes.append(CST_AFFECTOR_COLOUR);
		affectorTypes.append(CST_AFFECTOR_FLOCK_CENTERING);
		affectorTypes.append(CST_AFFECTOR_FORCEFIELD);
		affectorTypes.append(CST_AFFECTOR_GEOMETRY_ROTATOR);
		affectorTypes.append(CST_AFFECTOR_GRAVITY);
		affectorTypes.append(CST_AFFECTOR_INTER_PARTICLE_COLLIDER);
		affectorTypes.append(CST_AFFECTOR_JET);
		affectorTypes.append(CST_AFFECTOR_LINE);
		affectorTypes.append(CST_AFFECTOR_LINEAR_FORCE);
		affectorTypes.append(CST_AFFECTOR_PARTICLE_FOLLOWER);
		affectorTypes.append(CST_AFFECTOR_PATH_FOLLOWER);
		affectorTypes.append(CST_AFFECTOR_PLANE_COLLIDER);
		affectorTypes.append(CST_AFFECTOR_RANDOMISER);
		affectorTypes.append(CST_AFFECTOR_SCALE);
		affectorTypes.append(CST_AFFECTOR_SCALE_VELOCITY);
		affectorTypes.append(CST_AFFECTOR_SINE_FORCE);
		affectorTypes.append(CST_AFFECTOR_SPHERE_COLLIDER);
		affectorTypes.append(CST_AFFECTOR_TEXTURE_ANIMATOR);
		affectorTypes.append(CST_AFFECTOR_TEXTURE_ROTATOR);
		affectorTypes.append(CST_AFFECTOR_VELOCITY_MATCHING);
		affectorTypes.append(CST_AFFECTOR_VORTEX);
		append(new properties::EnumProperty(this, PRNL_AFFECTOR_TYPE, PRNL_AFFECTOR_TYPE, affectorTypes));

		// Enabled: Bool
		append(new properties::BoolProperty(this, PRNL_AFFECTOR_ENABLED, PRNL_AFFECTOR_ENABLED, ParticleUniverse::ParticleAffector::DEFAULT_ENABLED));

		// Position: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_AFFECTOR_POSITION, PRNL_AFFECTOR_POSITION, ParticleUniverse::ParticleAffector::DEFAULT_POSITION));

		// Mass: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_MASS, PRNL_AFFECTOR_MASS, ParticleUniverse::ParticleAffector::DEFAULT_MASS));
	}

	AffectorPropertyWindow::~AffectorPropertyWindow() {
	}

	void AffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(affector->getName()));

		// Type: List of types
		setEnumString(PRNL_AFFECTOR_TYPE, QString::fromStdString(affector->getAffectorType()));

		// Enabled: Bool
		setBool(PRNL_AFFECTOR_ENABLED, affector->isEnabled());

		// Position: Ogre::Vector3
		setVector3(PRNL_AFFECTOR_POSITION, affector->position);

		// Mass: ParticleUniverse::Real
		setDouble(PRNL_AFFECTOR_MASS, affector->mass);
	}

	void AffectorPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToAffector(prop, name);
		ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(_owner->getPUElement());
		if (affector && affector->_isMarkedForEmission() && affector->getParentTechnique()) {
			// Unprepare, to change a property of an emitted affector
			restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
		emit notifyChanged();
	}

	void AffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			_owner->setName(prop->getString());
			_owner->setCaption();
			affector->setName(prop->getString().toStdString());
		} else if (propertyName == PRNL_AFFECTOR_TYPE) {
			// Type: List of types
			// This requires the affector to be replaced.
			replaceAffectorType(prop);
		} else if (propertyName == PRNL_AFFECTOR_ENABLED) {
			// Enabled: Bool
			affector->_setOriginalEnabled(prop->getBool());
			affector->setEnabled(prop->getBool());
		} else if (propertyName == PRNL_AFFECTOR_POSITION) {
			// Position: Ogre::Vector3
			affector->position = prop->getVector3();
			affector->originalPosition = prop->getVector3();
		} else if (propertyName == PRNL_AFFECTOR_MASS) {
			// Mass: ParticleUniverse::Real
			affector->mass = prop->getDouble();
		}
	}

	void AffectorPropertyWindow::replaceAffectorType(properties::Property * prop) {
		// Type: List of types
		QString type = prop->getEnumString();
		if (type == "") {
			return;
		}

		ParticleUniverse::ParticleAffector * oldAffector = static_cast<ParticleUniverse::ParticleAffector *>(_owner->getPUElement());
		if (type.toStdString() == oldAffector->getAffectorType()) {
			return;
		}
		if (oldAffector) {
			ParticleUniverse::ParticleTechnique * technique = oldAffector->getParentTechnique();
			if (technique) {
				ParticleUniverse::ParticleAffector * newAffector = technique->createAffector(type.toStdString());
				oldAffector->copyParentAttributesTo(newAffector);
				bool wasStarted = false;
				ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
					wasStarted = true;
					system->stop();
				}
				technique->destroyAffector(oldAffector);
				_owner->setPUElement(newAffector);
				technique->_unprepareAffectors();
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old affector didn't have a technique, so create a new affector by means of the ParticleSystemManager itself and also delete
				the old affector by means of the ParticleSystemManager
				*/
				ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
				ParticleUniverse::ParticleAffector * newAffector = particleSystemManager->createAffector(type.toStdString());
				oldAffector->copyParentAttributesTo(newAffector);
				particleSystemManager->destroyAffector(oldAffector);
				_owner->setPUElement(newAffector);
			}
		} else {
			// There is no old affector. Create a new affector by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleAffector * newAffector = particleSystemManager->createAffector(type.toStdString());
			_owner->setPUElement(newAffector);
		}
		emit replacePropertyWindow(type);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */