#include "widgets/AffectorPropertyWindows/ForceFieldAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniverseForceFieldAffector.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ForceFieldAffectorPropertyWindow::ForceFieldAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_AFFECTOR_FORCEFIELD_FF_TYPE = "Forcefield type";
		FORCEFIELD_REALTIME = "Realtime";
		FORCEFIELD_MATRIX = "Matrix";
		PRNL_AFFECTOR_FORCEFIELD_DELTA = "Delta";
		PRNL_AFFECTOR_FORCEFIELD_FORCE = "Force";
		PRNL_AFFECTOR_FORCEFIELD_OCTAVES = "Octaves";
		PRNL_AFFECTOR_FORCEFIELD_FREQUENCY = "Frequency";
		PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE = "Amplitude";
		PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE = "Persistence";
		PRNL_AFFECTOR_FORCEFIELD_FF_SIZE = "Forcefield size";
		PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE = "Worldsize";
		PRNL_AFFECTOR_FORCEFIELD_IGNORE_X = "Ignore -x";
		PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y = "Ignore -y";
		PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z = "Ignore -z";
		PRNL_AFFECTOR_FORCEFIELD_MOVEMENT = "Movement";
		PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY = "Movement frequency";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// forcefield_type: List
		QStringList type;
		type.append(FORCEFIELD_REALTIME);
		type.append(FORCEFIELD_MATRIX);
		append(new properties::EnumProperty(this, PRNL_AFFECTOR_FORCEFIELD_FF_TYPE, PRNL_AFFECTOR_FORCEFIELD_FF_TYPE, type));

		// delta: Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_FORCEFIELD_DELTA, PRNL_AFFECTOR_FORCEFIELD_DELTA, ParticleUniverse::ForceFieldAffector::DEFAULT_DELTA));

		// force: Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_FORCEFIELD_FORCE, PRNL_AFFECTOR_FORCEFIELD_FORCE, ParticleUniverse::ForceFieldAffector::DEFAULT_FORCE));

		// octaves: uint
		append(new properties::UIntProperty(this, PRNL_AFFECTOR_FORCEFIELD_OCTAVES, PRNL_AFFECTOR_FORCEFIELD_OCTAVES, ParticleUniverse::ForceFieldAffector::DEFAULT_OCTAVES));

		// frequency: Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, ParticleUniverse::ForceFieldAffector::DEFAULT_FREQUENCY));

		// amplitude: Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, ParticleUniverse::ForceFieldAffector::DEFAULT_AMPLITUDE));

		// persistence: Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, ParticleUniverse::ForceFieldAffector::DEFAULT_PERSISTENCE));

		// forcefield_size: uint
		append(new properties::UIntProperty(this, PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, ParticleUniverse::ForceFieldAffector::DEFAULT_FORCEFIELDSIZE));

		// worldsize: Vector3
		append(new properties::Vec3Property(this, PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, ParticleUniverse::ForceFieldAffector::DEFAULT_WORLDSIZE));

		// ignore_negative_x: bool
		append(new properties::BoolProperty(this, PRNL_AFFECTOR_FORCEFIELD_IGNORE_X, PRNL_AFFECTOR_FORCEFIELD_IGNORE_X, false));

		// ignore_negative_y: bool
		append(new properties::BoolProperty(this, PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y, PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y, false));

		// ignore_negative_z: bool
		append(new properties::BoolProperty(this, PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z, PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z, false));

		// movement: Vector3
		append(new properties::Vec3Property(this, PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, ParticleUniverse::ForceFieldAffector::DEFAULT_MOVEMENT));

		// movement_frequency: Real
		append(new properties::DoubleProperty(this, PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, ParticleUniverse::ForceFieldAffector::DEFAULT_MOVEMENT_FREQUENCY));
	}

	ForceFieldAffectorPropertyWindow::~ForceFieldAffectorPropertyWindow() {
	}

	void ForceFieldAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ForceFieldAffector * forceFieldAffector = static_cast<ParticleUniverse::ForceFieldAffector *>(affector);

		// forcefield_type: List
		ParticleUniverse::ForceField::ForceFieldType type = forceFieldAffector->getForceFieldType();
		QString typeString = FORCEFIELD_REALTIME;
		if (type == ParticleUniverse::ForceField::FF_MATRIX_CALC) {
			typeString = FORCEFIELD_MATRIX;
		}
		setEnumString(PRNL_AFFECTOR_FORCEFIELD_FF_TYPE, typeString);

		// delta: Real
		setDouble(PRNL_AFFECTOR_FORCEFIELD_DELTA, forceFieldAffector->getDelta());

		// force: Real
		setDouble(PRNL_AFFECTOR_FORCEFIELD_FORCE, forceFieldAffector->getScaleForce());

		// octaves: uint
		setUint16(PRNL_AFFECTOR_FORCEFIELD_OCTAVES, forceFieldAffector->getOctaves());

		// frequency: Real
		setDouble(PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, forceFieldAffector->getFrequency());

		// amplitude: Real
		setDouble(PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, forceFieldAffector->getAmplitude());

		// persistence: Real
		setDouble(PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, forceFieldAffector->getPersistence());

		// forcefield_size: uint
		setUint16(PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, forceFieldAffector->getForceFieldSize());

		// worldsize: Vector3
		setVector3(PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, forceFieldAffector->getWorldSize());

		// ignore_negative_x: bool
		setBool(PRNL_AFFECTOR_FORCEFIELD_IGNORE_X, forceFieldAffector->getIgnoreNegativeX());

		// ignore_negative_y: bool
		setBool(PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y, forceFieldAffector->getIgnoreNegativeY());

		// ignore_negative_z: bool
		setBool(PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z, forceFieldAffector->getIgnoreNegativeZ());

		// movement: Vector3
		setVector3(PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, forceFieldAffector->getMovement());

		// movement_frequency: Real
		setDouble(PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, forceFieldAffector->getMovementFrequency());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
