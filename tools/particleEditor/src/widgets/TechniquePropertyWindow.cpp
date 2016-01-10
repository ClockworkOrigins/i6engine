#include "widgets/TechniquePropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/StringProperty.h"
#include "properties/UIntProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleUniverseTechnique.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	TechniquePropertyWindow::TechniquePropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_TECHNIQUE_ENABLED = "Enabled";
		PRNL_TECHNIQUE_POSITION = "Position";
		PRNL_TECHNIQUE_KEEP_LOCAL = "Keep local";
		PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA = "Visual particle quota";
		PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA = "Emitted emitter quota";
		PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA = "Emitted affector quota";
		PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA = "Emitted technique quota";
		PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA = "Emitted system quota";
		PRNL_TECHNIQUE_MATERIAL = "Material";
		PRNL_TECHNIQUE_LOD_INDEX = "Lod index";
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH = "Default particle width";
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT = "Default particle height";
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH = "Default particle depth";
		PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION = "Spatial hashing cell dimension";
		PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP = "Spatial hashing cell overlap";
		PRNL_TECHNIQUE_SPHASHING_SIZE = "Spatial hashtable size";
		PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL = "Spatial hashing update interval";
		PRNL_TECHNIQUE_MAX_VELOCITY = "Maximum velocity";
		PRNL_NAME = "Name";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Enabled: bool
		append(new properties::BoolProperty(this, PRNL_TECHNIQUE_ENABLED, PRNL_TECHNIQUE_ENABLED, ParticleUniverse::ParticleTechnique::DEFAULT_ENABLED));

		// Position: Vector3
		append(new properties::Vec3Property(this, PRNL_TECHNIQUE_POSITION, PRNL_TECHNIQUE_POSITION, ParticleUniverse::ParticleTechnique::DEFAULT_POSITION));

		// Keep local: bool
		append(new properties::BoolProperty(this, PRNL_TECHNIQUE_KEEP_LOCAL, PRNL_TECHNIQUE_KEEP_LOCAL, ParticleUniverse::ParticleTechnique::DEFAULT_KEEP_LOCAL));

		// Visual particle quota: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, ParticleUniverse::ParticleTechnique::DEFAULT_VISUAL_PARTICLE_QUOTA));

		// Emitted emitter quota: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_EMITTER_QUOTA));

		// Emitted affector quota: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_AFFECTOR_QUOTA));

		// Emitted technique quota: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_TECHNIQUE_QUOTA));

		// Emitted system quota: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_SYSTEM_QUOTA));

		// Material: Ogre::String
		append(new properties::StringProperty(this, PRNL_TECHNIQUE_MATERIAL, PRNL_TECHNIQUE_MATERIAL, ""));

		// Lod index: unsigned short
		append(new properties::UIntProperty(this, PRNL_TECHNIQUE_LOD_INDEX, PRNL_TECHNIQUE_LOD_INDEX, ParticleUniverse::ParticleTechnique::DEFAULT_LOD_INDEX));

		// Default particle width: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, ParticleUniverse::ParticleTechnique::DEFAULT_WIDTH));

		// Default particle height: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT, PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT, ParticleUniverse::ParticleTechnique::DEFAULT_HEIGHT));

		// Default particle depth: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, ParticleUniverse::ParticleTechnique::DEFAULT_DEPTH));

		// Spatial hashing cell dimension: ParticleUniverse::ushort
		append(new properties::UIntProperty(this, PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_CELL_DIM));

		// Spatial hashing cell overlap: ParticleUniverse::ushort
		append(new properties::UIntProperty(this, PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_CELL_OVERLAP));

		// Spatial hashtable size: unsigned int
		append(new properties::UIntProperty(this, PRNL_TECHNIQUE_SPHASHING_SIZE, PRNL_TECHNIQUE_SPHASHING_SIZE, ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_TABLE_SIZE));

		// Spatial hashing update interval: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_INTERVAL));

		// Maximum velocity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TECHNIQUE_MAX_VELOCITY, PRNL_TECHNIQUE_MAX_VELOCITY, ParticleUniverse::ParticleTechnique::DEFAULT_MAX_VELOCITY));
	}

	TechniquePropertyWindow::~TechniquePropertyWindow() {
	}

	void TechniquePropertyWindow::copyAttributesFromTechnique(ParticleUniverse::ParticleTechnique * technique) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(technique->getName()));

		// Enabled: bool
		setBool(PRNL_TECHNIQUE_ENABLED, technique->isEnabled());

		// Position: Vector3
		setVector3(PRNL_TECHNIQUE_POSITION, technique->position);

		// Keep local: bool
		setBool(PRNL_TECHNIQUE_KEEP_LOCAL, technique->isKeepLocal());

		// Visual particle quota: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, technique->getVisualParticleQuota());

		// Emitted emitter quota: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, technique->getEmittedEmitterQuota());

		// Emitted affector quota: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, technique->getEmittedAffectorQuota());

		// Emitted technique quota: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, technique->getEmittedTechniqueQuota());

		// Emitted system quota: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, technique->getEmittedSystemQuota());

		// Material: Ogre::String
		setString(PRNL_TECHNIQUE_MATERIAL, QString::fromStdString(technique->getMaterialName()));

		// Lod index: unsigned short
		setUint16(PRNL_TECHNIQUE_LOD_INDEX, technique->getLodIndex());

		// Default particle width: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, technique->getDefaultWidth());

		// Default particle height: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT, technique->getDefaultHeight());

		// Default particle depth: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, technique->getDefaultDepth());

		// Spatial hashing cell dimension: ParticleUniverse::ushort
		setUint16(PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, technique->getSpatialHashingCellDimension());

		// Spatial hashing cell overlap: ParticleUniverse::ushort
		setUint16(PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, technique->getSpatialHashingCellOverlap());

		// Spatial hashtable size: unsigned int
		setUint16(PRNL_TECHNIQUE_SPHASHING_SIZE, technique->getSpatialHashTableSize());

		// Spatial hashing update interval: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, technique->getSpatialHashingInterval());

		// Maximum velocity: ParticleUniverse::Real
		setDouble(PRNL_TECHNIQUE_MAX_VELOCITY, technique->getMaxVelocity());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
