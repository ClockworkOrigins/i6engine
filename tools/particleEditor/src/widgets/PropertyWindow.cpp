#include "widgets/PropertyWindow.h"

#include "properties/StringProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	PropertyWindow::PropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : QWidget(par), _owner(owner), _name(name), _types(), _properties() {
		setupUi(this);

		PRNL_NAME = "Name";
		PRNL_POST_RGB = " RGB";
		PRNL_POST_ALPHA = " Alpha";
		PRNL_W = ".w";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_TYPE = "Attribute type";
		PRNL_VALUE = "Value";
		PRNL_MIN_VALUE = "Minimum";
		PRNL_MAX_VALUE = "Maximum";
		PRNL_OSC_TYPE = "Oscillation type";
		PRNL_OSC_SINE = "Sine";
		PRNL_OSC_SQUARE = "Square";
		PRNL_OSC_FREQUENCY = "Frequency";
		PRNL_OSC_PHASE = "Phase";
		PRNL_OSC_BASE = "Base";
		PRNL_OSC_AMPLITUDE = "Amplitude";
		PRNL_CURVED_INTERPOLATION = "Interpolation";
		PRNL_CONTROL_POINT = "Control Point";
		PRNL_POSITION = "Position";
		PRNL_CONTROL_POINT_X = ".x";
		PRNL_CONTROL_POINT_Y = ".y";
		PRNL_POSITION_X = ".x";
		PRNL_POSITION_Y = ".y";
		PRNL_POSITION_Z = ".z";
		PRNL_FLOAT = "Value";
		DYN_FIXED = "Fixed";
		DYN_RANDOM = "Random";
		DYN_CURVED = "Curved";
		DYN_OSCILLATE = "Oscillate";
		DYN_CURVED_LINEAR = "Linear";
		DYN_CURVED_SPLINE = "Spline";
		PT_ALL = "All types";
		PT_VISUAL = "Visual";
		PT_EMITTER = "Emitter";
		PT_AFFECTOR = "Affector";
		PT_SYSTEM = "System";
		PT_TECHNIQUE = "Technique";

		_types.append(DYN_FIXED);
		_types.append(DYN_RANDOM);
		_types.append(DYN_CURVED);
		_types.append(DYN_OSCILLATE);

		if (!_name.isEmpty()) {
			append(new properties::StringProperty(this, PRNL_NAME, PRNL_NAME, _name));
		}

		hide();
	}

	PropertyWindow::~PropertyWindow() {
	}

	void PropertyWindow::append(properties::Property * prop) {
		verticalLayout->addWidget(prop);
		_properties.insert(std::make_pair(prop->getName(), prop));
		connect(prop, SIGNAL(changed(QString)), this, SLOT(changedProperty(QString)));
	}

	void PropertyWindow::setBool(QString name, bool value) {
		_properties[name]->setBool(value);
	}

	void PropertyWindow::setColourWithAlpha(QString name, Vec4 value) {
		_properties[name]->setColourWithAlpha(value);
	}

	void PropertyWindow::setDouble(QString name, double value) {
		_properties[name]->setDouble(value);
	}

	void PropertyWindow::setDynamicAttribute(QString name, ParticleUniverse::DynamicAttribute * value) {
		_properties[name]->setDynamicAttribute(value);
	}

	void PropertyWindow::setEnumString(QString name, QString value) {
		_properties[name]->setEnumString(value);
	}

	void PropertyWindow::setQuaternion(QString name, Quaternion value) {
		_properties[name]->setQuaternion(value);
	}

	void PropertyWindow::setString(QString name, QString value) {
		_properties[name]->setString(value);
	}

	void PropertyWindow::setUint16(QString name, uint16_t value) {
		_properties[name]->setUInt(value);
	}

	void PropertyWindow::setVector3(QString name, ParticleUniverse::Vector3 value) {
		_properties[name]->setVector3(value);
	}

	void PropertyWindow::setVector3List(QString name, std::vector<ParticleUniverse::Vector3> value) {
		_properties[name]->setVector3List(value);
	}

	void PropertyWindow::changedProperty(properties::Property * prop, QString name) {
		if (name == PRNL_NAME) {
			_owner->setName(prop->getString());
			_owner->setCaption();
		}
	}

	void PropertyWindow::restartParticle(ParticleUniverse::IElement * element, ParticleUniverse::Particle::ParticleType elementType, ParticleUniverse::Particle::ParticleType unprepareType) {
		if (!element) {
			return;
		}

		ParticleUniverse::ParticleTechnique * technique = nullptr;
		switch (elementType) {
		case ParticleUniverse::Particle::PT_TECHNIQUE: {
			technique = static_cast<ParticleUniverse::ParticleTechnique *>(element);
			break;
		}
		case ParticleUniverse::Particle::PT_EMITTER: {
			ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element);
			technique = emitter->getParentTechnique();
			break;
		}
		case ParticleUniverse::Particle::PT_AFFECTOR: {
			ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element);
			technique = affector->getParentTechnique();
			break;
		}
		default: {
			break;
		}
		}

		if (!technique) {
			return;
		}

		ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
		bool wasStarted = mustStopParticleSystem(system);
		switch (unprepareType) {
		case  ParticleUniverse::Particle::PT_SYSTEM: {
			technique->_unprepareSystem();
			// TODO: (Daniel) no break here, is this correct?
		}
		case  ParticleUniverse::Particle::PT_TECHNIQUE: {
			technique->_unprepareTechnique();
			break;
		}
		case  ParticleUniverse::Particle::PT_EMITTER: {
			technique->_unprepareEmitters();
			break;
		}
		case  ParticleUniverse::Particle::PT_AFFECTOR: {
			technique->_unprepareAffectors();
			break;
		}
		case  ParticleUniverse::Particle::PT_VISUAL: {
			technique->_unprepareVisualParticles();
			break;
		}
		default: {
			break;
		}
		}
		mustRestartParticleSystem(system, wasStarted);
	}

	void PropertyWindow::changedProperty(QString name) {
		changedProperty(_properties[name], name);
	}

	bool PropertyWindow::mustStopParticleSystem(ParticleUniverse::ParticleSystem * system) {
		if (!system) {
			return false;
		}

		bool wasStarted = false;
		if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
			wasStarted = true;
			system->stop();
		}
		return wasStarted;
	}

	void PropertyWindow::mustRestartParticleSystem(ParticleUniverse::ParticleSystem * system, bool wasStarted) {
		if (!system) {
			return;
		}

		// Start the system if needed
		if (wasStarted) {
			if (system) {
				system->start();
			}
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
