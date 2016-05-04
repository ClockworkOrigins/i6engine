#include "widgets/EmitterPropertyWindow.h"

#include <cfloat>

#include "i6engine/math/i6eQuaternion.h"

#include "properties/BoolProperty.h"
#include "properties/ColourWithAlphaProperty.h"
#include "properties/DynamicAttributeProperty.h"
#include "properties/EnumProperty.h"
#include "properties/QuaternionProperty.h"
#include "properties/UIntProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	EmitterPropertyWindow::EmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_EMITTER_BOX = EMITTER_BOX;
		CST_EMITTER_CIRCLE = EMITTER_CIRCLE;
		CST_EMITTER_LINE = EMITTER_LINE;
		CST_EMITTER_MESH_SURFACE = EMITTER_MESH_SURFACE;
		CST_EMITTER_POINT = EMITTER_POINT;
		CST_EMITTER_POSITION = EMITTER_POSITION;
		CST_EMITTER_SLAVE = EMITTER_SLAVE;
		CST_EMITTER_SPHERE = EMITTER_SPHERE;
		CST_EMITTER_VERTEX = EMITTER_VERTEX;
		PRNL_NAME = QApplication::tr("Name");
		PRNL_POST_RGB = " RGB";
		PRNL_POST_ALPHA = " Alpha";
		PRNL_W = ".w";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_EMITTER_TYPE = QApplication::tr("Emitter type");
		PRNL_EMITTER_ENABLED = QApplication::tr("Enabled");
		PRNL_EMITTER_POSITION = QApplication::tr("Position");
		PRNL_EMITTER_KEEP_LOCAL = QApplication::tr("Keep local");
		PRNL_EMITTER_DIRECTION = QApplication::tr("Direction");
		PRNL_EMITTER_ORIENTATION = QApplication::tr("Orientation");
		PRNL_EMITTER_ORIENTATION_START = QApplication::tr("Orientation range start");
		PRNL_EMITTER_ORIENTATION_END = QApplication::tr("Orientation range end");
		PRNL_EMITTER_VELOCITY = QApplication::tr("Velocity");
		PRNL_EMITTER_DURATION = QApplication::tr("Duration");
		PRNL_EMITTER_REPEAT_DELAY = QApplication::tr("Repeat - Delay");
		PRNL_EMITTER_ANGLE = QApplication::tr("Angle");
		PRNL_EMITTER_EMISSION_RATE = QApplication::tr("Emission rate");
		PRNL_EMITTER_TIME_TO_LIVE = QApplication::tr("Time to live");
		PRNL_EMITTER_MASS = QApplication::tr("Mass");
		PRNL_EMITTER_TEXTURE_COORD = QApplication::tr("Texture coordinate");
		PRNL_EMITTER_TEXTURE_COORD_START = QApplication::tr("Texture coordinate start");
		PRNL_EMITTER_TEXTURE_COORD_END = QApplication::tr("Texture coordinate end");
		PRNL_EMITTER_COLOUR = QApplication::tr("Colour");
		PRNL_EMITTER_COLOUR_RANGE_START = QApplication::tr("Colour range start");
		PRNL_EMITTER_COLOUR_RANGE_END = QApplication::tr("Colour range end");
		PRNL_EMITTER_ALL_PARTICLE_DIM = QApplication::tr("All particle dimensions");
		PRNL_EMITTER_PARTICLE_WIDTH = QApplication::tr("Particle width");
		PRNL_EMITTER_PARTICLE_HEIGHT = QApplication::tr("Particle height");
		PRNL_EMITTER_PARTICLE_DEPTH = QApplication::tr("Particle depth");
		PRNL_EMITTER_AUTO_DIRECTION = QApplication::tr("Auto direction");
		PRNL_EMITTER_FORCE_EMISSION = QApplication::tr("Force emission");

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create an emitter)
		QStringList types;
		types.append(CST_EMITTER_BOX);
		types.append(CST_EMITTER_CIRCLE);
		types.append(CST_EMITTER_LINE);
		types.append(CST_EMITTER_MESH_SURFACE);
		types.append(CST_EMITTER_POINT);
		types.append(CST_EMITTER_POSITION);
		types.append(CST_EMITTER_SLAVE);
		types.append(CST_EMITTER_SPHERE);
		types.append(CST_EMITTER_VERTEX);
		append(new properties::EnumProperty(this, PRNL_EMITTER_TYPE, PRNL_EMITTER_TYPE, types));

		// Enabled: Bool
		append(new properties::BoolProperty(this, PRNL_EMITTER_ENABLED, PRNL_EMITTER_ENABLED, ParticleUniverse::ParticleEmitter::DEFAULT_ENABLED));

		// Position: Vector3
		append(new properties::Vec3Property(this, PRNL_EMITTER_POSITION, PRNL_EMITTER_POSITION, ParticleUniverse::ParticleEmitter::DEFAULT_POSITION));

		// Keep local: Bool
		append(new properties::BoolProperty(this, PRNL_EMITTER_KEEP_LOCAL, PRNL_EMITTER_KEEP_LOCAL, ParticleUniverse::ParticleEmitter::DEFAULT_KEEP_LOCAL));

		// Direction: Vector3
		append(new properties::Vec3Property(this, PRNL_EMITTER_DIRECTION, PRNL_EMITTER_DIRECTION, ParticleUniverse::ParticleEmitter::DEFAULT_DIRECTION));

		// Orientation: Quaternion
		append(new properties::QuaternionProperty(this, PRNL_EMITTER_ORIENTATION, PRNL_EMITTER_ORIENTATION, Quaternion(ParticleUniverse::ParticleEmitter::DEFAULT_ORIENTATION)));

		// Orientation range start: Quaternion
		append(new properties::QuaternionProperty(this, PRNL_EMITTER_ORIENTATION_START, PRNL_EMITTER_ORIENTATION_START, Quaternion(ParticleUniverse::ParticleEmitter::DEFAULT_ORIENTATION_RANGE_START)));

		// Orientation range end: Quaternion
		append(new properties::QuaternionProperty(this, PRNL_EMITTER_ORIENTATION_END, PRNL_EMITTER_ORIENTATION_END, Quaternion(ParticleUniverse::ParticleEmitter::DEFAULT_ORIENTATION_RANGE_END)));

		// Velocity: Dynamic attribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_VELOCITY);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_VELOCITY, PRNL_EMITTER_VELOCITY, dynAttr));

		// Duration: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_DURATION);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_DURATION, PRNL_EMITTER_DURATION, dynAttr));

		// Repeat - Delay: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_REPEAT_DELAY);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_REPEAT_DELAY, PRNL_EMITTER_REPEAT_DELAY, dynAttr));

		// Emits: Is no property, because this is done by means of connections

		// Angle: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_ANGLE);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_ANGLE, PRNL_EMITTER_ANGLE, dynAttr));

		// Emission rate: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_EMISSION_RATE);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_EMISSION_RATE, PRNL_EMITTER_EMISSION_RATE, dynAttr));

		// Time to live: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_TIME_TO_LIVE);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_TIME_TO_LIVE, PRNL_EMITTER_TIME_TO_LIVE, dynAttr));

		// Mass: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_MASS);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_MASS, PRNL_EMITTER_MASS, dynAttr));

		// Texture coordinate: Uint16
		append(new properties::UIntProperty(this, PRNL_EMITTER_TEXTURE_COORD, PRNL_EMITTER_TEXTURE_COORD, ParticleUniverse::ParticleEmitter::DEFAULT_TEXTURE_COORDS));

		// Texture coordinate Start: Uint16
		append(new properties::UIntProperty(this, PRNL_EMITTER_TEXTURE_COORD_START, PRNL_EMITTER_TEXTURE_COORD_START, ParticleUniverse::ParticleEmitter::DEFAULT_TEXTURE_COORDS));

		// Texture coordinate End: Uint16
		append(new properties::UIntProperty(this, PRNL_EMITTER_TEXTURE_COORD_END, PRNL_EMITTER_TEXTURE_COORD_END, ParticleUniverse::ParticleEmitter::DEFAULT_TEXTURE_COORDS));

		// Colour: Colour attibute with alpha
		append(new properties::ColourWithAlphaProperty(this, PRNL_EMITTER_COLOUR, PRNL_EMITTER_COLOUR, Vec4(255, 255, 255, 255)));

		// Colour range start: Colour attibute with alpha
		append(new properties::ColourWithAlphaProperty(this, PRNL_EMITTER_COLOUR_RANGE_START, PRNL_EMITTER_COLOUR_RANGE_START, Vec4(0, 0, 0, 0)));

		// Colour range end: Colour attibute with alpha
		append(new properties::ColourWithAlphaProperty(this, PRNL_EMITTER_COLOUR_RANGE_END, PRNL_EMITTER_COLOUR_RANGE_END, Vec4(255, 255, 255, 255)));

		// All particle dimensions: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_DIMENSIONS);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_ALL_PARTICLE_DIM, PRNL_EMITTER_ALL_PARTICLE_DIM, dynAttr));

		// Particle width: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_WIDTH);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_PARTICLE_WIDTH, PRNL_EMITTER_PARTICLE_WIDTH, dynAttr));

		// Particle height: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_HEIGHT);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_PARTICLE_HEIGHT, PRNL_EMITTER_PARTICLE_HEIGHT, dynAttr));

		// Particle depth: Dynamic attibute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::ParticleEmitter::DEFAULT_DEPTH);
		append(new properties::DynamicAttributeProperty(this, PRNL_EMITTER_PARTICLE_DEPTH, PRNL_EMITTER_PARTICLE_DEPTH, dynAttr));

		// Auto direction: Bool
		append(new properties::BoolProperty(this, PRNL_EMITTER_AUTO_DIRECTION, PRNL_EMITTER_AUTO_DIRECTION, ParticleUniverse::ParticleEmitter::DEFAULT_AUTO_DIRECTION));

		// Force emission: Bool
		append(new properties::BoolProperty(this, PRNL_EMITTER_FORCE_EMISSION, PRNL_EMITTER_FORCE_EMISSION, ParticleUniverse::ParticleEmitter::DEFAULT_FORCE_EMISSION));
	}

	EmitterPropertyWindow::~EmitterPropertyWindow() {
	}

	void EmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(emitter->getName()));

		// Type: List of types
		setEnumString(PRNL_EMITTER_TYPE, QString::fromStdString(emitter->getEmitterType()));

		// Enabled: Bool
		setBool(PRNL_EMITTER_ENABLED, emitter->isEnabled());

		// Position: Vector3
		setVector3(PRNL_EMITTER_POSITION, emitter->position);

		// Keep local: Bool
		setBool(PRNL_EMITTER_KEEP_LOCAL, emitter->isKeepLocal());

		// Direction: Vector3
		setVector3(PRNL_EMITTER_DIRECTION, emitter->getOriginalParticleDirection());

		// Orientation: Quaternion
		setQuaternion(PRNL_EMITTER_ORIENTATION, Quaternion(emitter->getParticleOrientation()));

		// Orientation range start: Quaternion
		setQuaternion(PRNL_EMITTER_ORIENTATION_START, Quaternion(emitter->getParticleOrientationRangeStart()));

		// Orientation range end: Quaternion
		setQuaternion(PRNL_EMITTER_ORIENTATION_END, Quaternion(emitter->getParticleOrientationRangeEnd()));

		// Velocity: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_VELOCITY, emitter->getDynVelocity());

		// Duration: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_DURATION, emitter->getDynDuration());

		// Repeat - Delay: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_REPEAT_DELAY, emitter->getDynRepeatDelay());

		// Emits: Is no property, because this is done by means of connections

		// Angle: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_ANGLE, emitter->getDynAngle());

		// Emission rate: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_EMISSION_RATE, emitter->getDynEmissionRate());

		// Time to live: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_TIME_TO_LIVE, emitter->getDynTotalTimeToLive());

		// Mass: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_MASS, emitter->getDynParticleMass());

		// Texture coordinate: Uint16
		setUint16(PRNL_EMITTER_TEXTURE_COORD, emitter->getParticleTextureCoords());

		// Texture coordinate Start: Uint16
		setUint16(PRNL_EMITTER_TEXTURE_COORD_START, emitter->getParticleTextureCoordsRangeStart());

		// Texture coordinate End: Uint16
		setUint16(PRNL_EMITTER_TEXTURE_COORD_END, emitter->getParticleTextureCoordsRangeEnd());

		// Colour: Colour attibute with alpha
		setColourWithAlpha(PRNL_EMITTER_COLOUR, Vec4(255 * emitter->getParticleColour().a, 255 * emitter->getParticleColour().r, 255 * emitter->getParticleColour().g, 255 * emitter->getParticleColour().b));

		// Colour range start: Colour attibute with alpha
		setColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_START, Vec4(255 * emitter->getParticleColourRangeStart().a, 255 * emitter->getParticleColourRangeStart().r, 255 * emitter->getParticleColourRangeStart().g, 255 * emitter->getParticleColourRangeStart().b));

		// Colour range end: Colour attibute with alpha
		setColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_END, Vec4(255 * emitter->getParticleColourRangeEnd().a, 255 * emitter->getParticleColourRangeEnd().r, 255 * emitter->getParticleColourRangeEnd().g, 255 * emitter->getParticleColourRangeEnd().b));

		// All particle dimensions: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_ALL_PARTICLE_DIM, emitter->getDynParticleAllDimensions());

		// Particle width: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_PARTICLE_WIDTH, emitter->getDynParticleWidth());

		// Particle height: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_PARTICLE_HEIGHT, emitter->getDynParticleHeight());

		// Particle depth: Dynamic attibute
		setDynamicAttribute(PRNL_EMITTER_PARTICLE_DEPTH, emitter->getDynParticleDepth());

		// Auto direction: Bool
		setBool(PRNL_EMITTER_AUTO_DIRECTION, emitter->isAutoDirection());

		// Force emission: Bool
		setBool(PRNL_EMITTER_FORCE_EMISSION, emitter->isForceEmission());
	}

	void EmitterPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToEmitter(prop, name);
		ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(_owner->getPUElement());
		if (emitter && emitter->_isMarkedForEmission() && emitter->getParentTechnique()) {
			// Unprepare, to change a property of an emitted emitter
			//restartParticleEmitters(emitter);
			restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
		emit notifyChanged();
	}

	void EmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(_owner->getPUElement());
		if (!emitter) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			_owner->setName(prop->getString());
			_owner->setCaption();
			emitter->setName(prop->getString().toStdString());
		} else if (propertyName == PRNL_EMITTER_TYPE) {
			// Type: List of types
			// This requires the emitter to be replaced.
			replaceEmitterType(prop);
		} else if (propertyName == PRNL_EMITTER_ENABLED) {
			// Enabled: Bool
			emitter->_setOriginalEnabled(prop->getBool());
			emitter->setEnabled(prop->getBool());
		} else if (propertyName == PRNL_EMITTER_POSITION + PRNL_X || propertyName == PRNL_EMITTER_POSITION + PRNL_Y || propertyName == PRNL_EMITTER_POSITION + PRNL_Z) {
			// Position: Vector3
			emitter->position = prop->getVector3();
			emitter->originalPosition = prop->getVector3();
		} else if (propertyName == PRNL_EMITTER_KEEP_LOCAL) {
			// Keep local: Bool
			emitter->setKeepLocal(prop->getBool());
		} else if (propertyName == PRNL_EMITTER_DIRECTION) {
			// Direction: Vector3
			emitter->setParticleDirection(prop->getVector3());
		} else if (propertyName == PRNL_EMITTER_ORIENTATION) {
			// Orientation: Quaternion
			emitter->setParticleOrientation(prop->getQuaternion().toOgre());
		} else if (propertyName == PRNL_EMITTER_ORIENTATION_START) {
			// Orientation range start: Quaternion
			emitter->setParticleOrientationRangeStart(prop->getQuaternion().toOgre());
		} else if (propertyName == PRNL_EMITTER_ORIENTATION_END) {
			// Orientation range end: Quaternion
			emitter->setParticleOrientationRangeEnd(prop->getQuaternion().toOgre());
		} else if (propertyName == PRNL_EMITTER_VELOCITY) {
			// Velocity type: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynVelocity(dynAttr);
			}
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_DURATION) {
			// Duration: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynDuration(dynAttr);
			}
			if (std::abs(emitter->getDynDuration()->getValue() - ParticleUniverse::ParticleEmitter::DEFAULT_DURATION) < DBL_EPSILON) {
				emitter->setDynDurationSet(false); // Supressing writing this property the script and setting the emitter to the right state.
			} else {
				emitter->setDynDurationSet(true); // Force writing this property to the script and setting the emitter to the right state.
			}
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_REPEAT_DELAY) {
			// Repeat - Delay: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynRepeatDelay(dynAttr);
			}
			emitter->setDynRepeatDelaySet(true); // Force writing to the script
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_ANGLE) {
			// Angle: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynAngle(dynAttr);
			}
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_EMISSION_RATE) {
			// Emission rate: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynEmissionRate(dynAttr);
			}
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_TIME_TO_LIVE) {
			// Time to live: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynTotalTimeToLive(dynAttr);
			}
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_MASS) {
			// Mass: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynParticleMass(dynAttr);
			}
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_TEXTURE_COORD) {
			// Texture coordinate: Uint16
			emitter->setParticleTextureCoords(prop->getUInt());
		} else if (propertyName == PRNL_EMITTER_TEXTURE_COORD_START) {
			// Texture coordinate Start: Uint16
			emitter->setParticleTextureCoordsRangeStart(prop->getUInt());
		} else if (propertyName == PRNL_EMITTER_TEXTURE_COORD_END) {
			// Texture coordinate End: Uint16
			emitter->setParticleTextureCoordsRangeEnd(prop->getUInt());
		} else if (propertyName == PRNL_EMITTER_COLOUR) {
			// Colour: Colour attibute with alpha
			Vec4 c = prop->getColourWithAlpha();
			Ogre::ColourValue colour(ParticleUniverse::Real(c.getX()) / 255.0f, ParticleUniverse::Real(c.getY()) / 255.0f, ParticleUniverse::Real(c.getZ()) / 255.0f, ParticleUniverse::Real(c.getW()) / 255.0f);
			emitter->setParticleColour(colour);
		} else if (propertyName == PRNL_EMITTER_COLOUR_RANGE_START) {
			// Colour range start: Colour attibute with alpha
			Vec4 c = prop->getColourWithAlpha();
			Ogre::ColourValue colour(ParticleUniverse::Real(c.getX()) / 255.0f, ParticleUniverse::Real(c.getY()) / 255.0f, ParticleUniverse::Real(c.getZ()) / 255.0f, ParticleUniverse::Real(c.getW()) / 255.0f);
			emitter->setParticleColourRangeStart(colour);
		} else if (propertyName == PRNL_EMITTER_COLOUR_RANGE_END) {
			// Colour range end: Colour attibute with alpha
			Vec4 c = prop->getColourWithAlpha();
			Ogre::ColourValue colour(ParticleUniverse::Real(c.getX()) / 255.0f, ParticleUniverse::Real(c.getY()) / 255.0f, ParticleUniverse::Real(c.getZ()) / 255.0f, ParticleUniverse::Real(c.getW()) / 255.0f);
			emitter->setParticleColourRangeEnd(colour);
		} else if (propertyName == PRNL_EMITTER_ALL_PARTICLE_DIM) {
			// All particle dimensions: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynParticleAllDimensions(dynAttr);
			}
			emitter->setDynParticleAllDimensionsSet(true); // Force writing to the script
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_PARTICLE_WIDTH) {
			// Particle width: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynParticleWidth(dynAttr);
			}
			emitter->setDynParticleWidthSet(true); // Force writing to the script
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_PARTICLE_HEIGHT) {
			// Particle height: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynParticleHeight(dynAttr);
			}
			emitter->setDynParticleHeightSet(true); // Force writing to the script
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_PARTICLE_DEPTH) {
			// Particle depth: Dynamic attibute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				emitter->setDynParticleDepth(dynAttr);
			}
			emitter->setDynParticleDepthSet(true); // Force writing to the script
			if (emitter->_isMarkedForEmission()) {
				//restartParticleEmitters(emitter);
				restartParticle(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
			}
		} else if (propertyName == PRNL_EMITTER_AUTO_DIRECTION) {
			// Auto direction: Bool
			emitter->setAutoDirection(prop->getBool());
		} else if (propertyName == PRNL_EMITTER_FORCE_EMISSION) {
			// Force emission: Bool
			emitter->setForceEmission(prop->getBool());
		}
	}

	void EmitterPropertyWindow::replaceEmitterType(properties::Property * prop) {
		// Type: List of types
		Ogre::String type = prop->getEnumString().toStdString();
		if (type == Ogre::StringUtil::BLANK) {
			return;
		}

		ParticleUniverse::ParticleEmitter * oldEmitter = static_cast<ParticleUniverse::ParticleEmitter *>(_owner->getPUElement());
		if (type == oldEmitter->getEmitterType()) {
			return;
		}
		if (oldEmitter) {
			ParticleUniverse::ParticleTechnique * technique = oldEmitter->getParentTechnique();
			if (technique) {
				ParticleUniverse::ParticleEmitter * newEmitter = technique->createEmitter(type);
				oldEmitter->copyParentAttributesTo(newEmitter);
				bool wasStarted = false;
				ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
					wasStarted = true;
					system->stop();
				}
				technique->destroyEmitter(oldEmitter);
				_owner->setPUElement(newEmitter);
				technique->_unprepareEmitters();
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old emitter didn't have a technique, so create a new emitter by means of the ParticleSystemManager itself and also delete
				the old emitter by means of the ParticleSystemManager
				*/
				ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
				ParticleUniverse::ParticleEmitter * newEmitter = particleSystemManager->createEmitter(type);
				oldEmitter->copyParentAttributesTo(newEmitter);
				particleSystemManager->destroyEmitter(oldEmitter);
				_owner->setPUElement(newEmitter);
			}
		} else {
			// There is no old emitter. Create a new emitter by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleEmitter * newEmitter = particleSystemManager->createEmitter(type);
			_owner->setPUElement(newEmitter);
		}

		emit replacePropertyWindow(QString::fromStdString(type));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
