#include "widgets/EmitterPropertyWindow.h"

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

namespace i6engine {
namespace particleEditor {
namespace widgets {

	EmitterPropertyWindow::EmitterPropertyWindow(QWidget * par, QString name) : PropertyWindow(par, name) {
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
		PRNL_NAME = "Name";
		PRNL_POST_RGB = " RGB";
		PRNL_POST_ALPHA = " Alpha";
		PRNL_W = ".w";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_EMITTER_TYPE = "Emitter type";
		PRNL_EMITTER_ENABLED = "Enabled";
		PRNL_EMITTER_POSITION = "Position";
		PRNL_EMITTER_KEEP_LOCAL = "Keep local";
		PRNL_EMITTER_DIRECTION = "Direction";
		PRNL_EMITTER_ORIENTATION = "Orientation";
		PRNL_EMITTER_ORIENTATION_START = "Orientation range start";
		PRNL_EMITTER_ORIENTATION_END = "Orientation range end";
		PRNL_EMITTER_VELOCITY = "Velocity";
		PRNL_EMITTER_DURATION = "Duration";
		PRNL_EMITTER_REPEAT_DELAY = "Repeat - Delay";
		PRNL_EMITTER_ANGLE = "Angle";
		PRNL_EMITTER_EMISSION_RATE = "Emission rate";
		PRNL_EMITTER_TIME_TO_LIVE = "Time to live";
		PRNL_EMITTER_MASS = "Mass";
		PRNL_EMITTER_TEXTURE_COORD = "Texture coordinate";
		PRNL_EMITTER_TEXTURE_COORD_START = "Texture coordinate start";
		PRNL_EMITTER_TEXTURE_COORD_END = "Texture coordinate end";
		PRNL_EMITTER_COLOUR = "Colour";
		PRNL_EMITTER_COLOUR_RANGE_START = "Colour range start";
		PRNL_EMITTER_COLOUR_RANGE_END = "Colour range end";
		PRNL_EMITTER_ALL_PARTICLE_DIM = "All particle dimensions";
		PRNL_EMITTER_PARTICLE_WIDTH = "Particle width";
		PRNL_EMITTER_PARTICLE_HEIGHT = "Particle height";
		PRNL_EMITTER_PARTICLE_DEPTH = "Particle depth";
		PRNL_EMITTER_AUTO_DIRECTION = "Auto direction";
		PRNL_EMITTER_FORCE_EMISSION = "Force emission";

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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
