#include "widgets/SystemPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/StringProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	SystemPropertyWindow::SystemPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_NAME = QApplication::tr("Name");
		PRNL_SYSTEM_CATEGORY = QApplication::tr("Category");
		PRNL_SYSTEM_KEEP_LOCAL = QApplication::tr("Keep local");
		PRNL_SYSTEM_ITERATION_INTERVAL = QApplication::tr("Iteration interval");
		PRNL_SYSTEM_FIXED_TIMEOUT = QApplication::tr("Fixed timeout");
		PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT = QApplication::tr("Nonvisible update timeout");
		PRNL_SYSTEM_LOD_DISTANCES = QApplication::tr("Lod distances");
		PRNL_SYSTEM_SMOOTH_LOD = QApplication::tr("Smooth lod");
		PRNL_SYSTEM_FAST_FORWARD = QApplication::tr("Fast forward");
		PRNL_SYSTEM_FAST_FORWARD_TIME = QApplication::tr("Time");
		PRNL_SYSTEM_FAST_FORWARD_INTERVAL = QApplication::tr("Interval");
		PRNL_SYSTEM_MAIN_CAMERA_NAME = QApplication::tr("Main camera name");
		PRNL_SYSTEM_SCALE_VELOCITY = QApplication::tr("Scale velocity");
		PRNL_SYSTEM_SCALE_TIME = QApplication::tr("Scale time");
		PRNL_SYSTEM_SCALE = QApplication::tr("Scale dimensions");
		PRNL_SYSTEM_TIGHT_BOUNDING_BOX = QApplication::tr("Use tight bounding box");
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_FLOAT = QApplication::tr("Value");

		// Category: String
		append(new properties::StringProperty(this, PRNL_SYSTEM_CATEGORY, PRNL_SYSTEM_CATEGORY, ""));

		// Keep local: Bool
		append(new properties::BoolProperty(this, PRNL_SYSTEM_KEEP_LOCAL, PRNL_SYSTEM_KEEP_LOCAL, ParticleUniverse::ParticleSystem::DEFAULT_KEEP_LOCAL));

		// Iteration interval: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_ITERATION_INTERVAL, PRNL_SYSTEM_ITERATION_INTERVAL, ParticleUniverse::ParticleSystem::DEFAULT_ITERATION_INTERVAL));

		// Fixed timeout: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_FIXED_TIMEOUT, PRNL_SYSTEM_FIXED_TIMEOUT, ParticleUniverse::ParticleSystem::DEFAULT_FIXED_TIMEOUT));

		// Non-visible update timeout: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, ParticleUniverse::ParticleSystem::DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT));

		// Lod distances: List of ParticleUniverse::Real
		//append(new ParentPropertyWithButtonAndFloats(PRNL_SYSTEM_LOD_DISTANCES, PRNL_SYSTEM_LOD_DISTANCES));

		// Smooth lod: bool
		append(new properties::BoolProperty(this, PRNL_SYSTEM_SMOOTH_LOD, PRNL_SYSTEM_SMOOTH_LOD, ParticleUniverse::ParticleSystem::DEFAULT_SMOOTH_LOD));

		// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_FAST_FORWARD_TIME, PRNL_SYSTEM_FAST_FORWARD_TIME, ParticleUniverse::ParticleSystem::DEFAULT_FAST_FORWARD_TIME));
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_FAST_FORWARD_INTERVAL, PRNL_SYSTEM_FAST_FORWARD_INTERVAL, 0.0));

		// Main camera name: Ogre::String
		append(new properties::StringProperty(this, PRNL_SYSTEM_MAIN_CAMERA_NAME, PRNL_SYSTEM_MAIN_CAMERA_NAME, ""));

		// Scale velocity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_SCALE_VELOCITY, PRNL_SYSTEM_SCALE_VELOCITY, ParticleUniverse::ParticleSystem::DEFAULT_SCALE_VELOCITY));

		// Scale time: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_SCALE_TIME, PRNL_SYSTEM_SCALE_TIME, ParticleUniverse::ParticleSystem::DEFAULT_SCALE_TIME));

		// Scale dimensions: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_SYSTEM_SCALE, PRNL_SYSTEM_SCALE, Ogre::Vector3::UNIT_SCALE));

		// Use tight bounding box: bool
		append(new properties::BoolProperty(this, PRNL_SYSTEM_TIGHT_BOUNDING_BOX, PRNL_SYSTEM_TIGHT_BOUNDING_BOX, ParticleUniverse::ParticleSystem::DEFAULT_TIGHT_BOUNDINGBOX));
	}

	SystemPropertyWindow::~SystemPropertyWindow() {
	}

	void SystemPropertyWindow::copyAttributesFromSystem(ParticleUniverse::ParticleSystem * system) {
		// Name: Ogre::String
		QString templateName = QString::fromStdString(system->getTemplateName());
		setString(PRNL_NAME, templateName);

		// Keep local: Bool
		setBool(PRNL_SYSTEM_KEEP_LOCAL, system->isKeepLocal());

		// Iteration interval: ParticleUniverse::Real
		setDouble(PRNL_SYSTEM_ITERATION_INTERVAL, system->getIterationInterval());

		// Fixed timeout: ParticleUniverse::Real
		setDouble(PRNL_SYSTEM_FIXED_TIMEOUT, system->getFixedTimeout());

		// Non-visible update timeout: ParticleUniverse::Real
		setDouble(PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, system->getNonVisibleUpdateTimeout());

		// Lod distances: List of ParticleUniverse::Real
		/*wxPGProperty * prop = GetPropertyByName(PRNL_SYSTEM_LOD_DISTANCES);
		if (prop) {
			ParentPropertyWithButtonAndFloats * propLodDistances = static_cast<ParentPropertyWithButtonAndFloats *>(prop);
			propLodDistances->reset();
			ParticleUniverse::ParticleSystem::LodDistanceList list = system->getLodDistances();
			ParticleUniverse::ParticleSystem::LodDistanceIterator it;
			ParticleUniverse::ParticleSystem::LodDistanceIterator itEnd = list.end();
			for (it = list.begin(); it != itEnd; ++it) {
				propLodDistances->addFloat(this, Ogre::Math::Sqrt(*it));
			}
		}*/

		// Smooth lod: bool
		setBool(PRNL_SYSTEM_SMOOTH_LOD, system->isSmoothLod());

		// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
		setDouble(PRNL_SYSTEM_FAST_FORWARD_TIME, system->getFastForwardTime());
		setDouble(PRNL_SYSTEM_FAST_FORWARD_INTERVAL, system->getFastForwardInterval());

		// Main camera name: Ogre::String
		QString cameraName = QString::fromStdString(system->getMainCameraName());
		setString(PRNL_SYSTEM_MAIN_CAMERA_NAME, cameraName);

		// Scale velocity: ParticleUniverse::Real
		setDouble(PRNL_SYSTEM_SCALE_VELOCITY, system->getScaleVelocity());

		// Scale time: ParticleUniverse::Real
		setDouble(PRNL_SYSTEM_SCALE_TIME, system->getScaleTime());

		// Scale dimensions: Ogre::Vector3
		setVector3(PRNL_SYSTEM_SCALE, system->getScale());

		// Use tight bounding box: bool
		setBool(PRNL_SYSTEM_TIGHT_BOUNDING_BOX, system->hasTightBoundingBox());
	}

	void SystemPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToSystem(prop, name);
		ParticleUniverse::ParticleSystem * system = static_cast<ParticleUniverse::ParticleSystem *>(_owner->getPUElement());
		if (system && system->_isMarkedForEmission()) {
			// Force recreation of new emitted systems
			// Todo: Implement later, because this version of the editor doesn't support emitted particle systems.
		}
		emit notifyChanged();
	}

	void SystemPropertyWindow::copyAttributeToSystem(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleSystem * system = static_cast<ParticleUniverse::ParticleSystem *>(_owner->getPUElement());
		if (!system) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			QString oldName = QString::fromStdString(system->getTemplateName());
			if (prop->getString() == oldName) {
				return;
			}
			_owner->setName(prop->getString());
			_owner->setCaption();
			system->setTemplateName(prop->getString().toStdString());

			ParticleUniverse::String script = ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(system);

			Ogre::ScriptCompilerManager * scriptCompilerManager = Ogre::ScriptCompilerManager::getSingletonPtr();
			char * buffer = new char[script.length() + 1];
			strcpy(buffer, script.c_str());
			Ogre::DataStreamPtr * datastream = new Ogre::DataStreamPtr(new Ogre::MemoryDataStream(buffer, strlen(buffer)));
			scriptCompilerManager->parseScript(*datastream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			delete datastream;
			delete[] buffer;

			// Update everything
			emit renameParticleSystem(oldName, prop->getString()); // change name only when saving and reload all scripts then to be able to use!
		} else if (propertyName == PRNL_SYSTEM_CATEGORY) {
		} else if (propertyName == PRNL_SYSTEM_KEEP_LOCAL) {
			// Keep local: Bool
			system->setKeepLocal(prop->getBool());
		} else if (propertyName == PRNL_SYSTEM_ITERATION_INTERVAL) {
			// Iteration interval: ParticleUniverse::Real
			system->setIterationInterval(prop->getDouble());
		} else if (propertyName == PRNL_SYSTEM_FIXED_TIMEOUT) {
			// Fixed timeout: ParticleUniverse::Real
			system->setFixedTimeout(prop->getDouble());
		} else if (propertyName == PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT) {
			// Non-visible update timeout: ParticleUniverse::Real
			system->setNonVisibleUpdateTimeout(prop->getDouble());
		} /*else if (propertyName == PRNL_SYSTEM_LOD_DISTANCES || propertyName.StartsWith(PRNL_FLOAT)) {
			// Lod distances: List of ParticleUniverse::Real
			// Todo: Checking on PRNL_FLOAT only is not sufficient if more lists of floats are added to this propertygrid.
			ParentPropertyWithButtonAndFloats * parentPropertyWithButtonAndFloats = nullptr;
			if (propertyName == PRNL_SYSTEM_LOD_DISTANCES) {
				parentPropertyWithButtonAndFloats = static_cast<ParentPropertyWithButtonAndFloats*>(prop);
			} else {
				wxPGProperty * p = prop->GetParent();
				parentPropertyWithButtonAndFloats = static_cast<ParentPropertyWithButtonAndFloats*>(p);
			}
			unsigned int size = parentPropertyWithButtonAndFloats->getNumberOfFloats();
			system->clearLodDistances();
			for (unsigned int i = 0; i < size; ++i) {
				ParticleUniverse::Real f = parentPropertyWithButtonAndFloats->getFloat(this, i);
				system->addLodDistance(f);
			}
		}*/ else if (propertyName == PRNL_SYSTEM_SMOOTH_LOD) {
			// Smooth lod: bool
			system->setSmoothLod(prop->getBool());
		} else if (propertyName == PRNL_SYSTEM_FAST_FORWARD_TIME || propertyName == PRNL_SYSTEM_FAST_FORWARD_INTERVAL) {
			// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
			properties::Property * p = _properties[PRNL_SYSTEM_FAST_FORWARD_TIME];
			if (!p) {
				return;
			}
			ParticleUniverse::Real time = p->getDouble();
			p = _properties[PRNL_SYSTEM_FAST_FORWARD_INTERVAL];
			if (!p) {
				return;
			}
			ParticleUniverse::Real interval = p->getDouble();
			system->setFastForward(time, interval);
		} else if (propertyName == PRNL_SYSTEM_MAIN_CAMERA_NAME) {
			// Main camera name: Ogre::String
			Ogre::String cameraName = prop->getString().toStdString();
			try {
				system->setMainCameraName(cameraName);
			} catch (Ogre::Exception e) {
				// Ignore the exception
			}
		} else if (propertyName == PRNL_SYSTEM_SCALE_VELOCITY) {
			// Scale velocity: ParticleUniverse::Real
			system->setScaleVelocity(prop->getDouble());
		} else if (propertyName == PRNL_SYSTEM_SCALE_TIME) {
			// Scale time: ParticleUniverse::Real
			system->setScaleTime(prop->getDouble());
		} else if (propertyName == PRNL_SYSTEM_SCALE + PRNL_X || propertyName == PRNL_SYSTEM_SCALE + PRNL_Y || propertyName == PRNL_SYSTEM_SCALE + PRNL_Z) {
			// Scale dimensions: Ogre::Vector3
			system->setScale(prop->getVector3());
		} else if (propertyName == PRNL_SYSTEM_TIGHT_BOUNDING_BOX) {
			// Use tight bounding box: bool
			system->setTightBoundingBox(prop->getBool());
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
