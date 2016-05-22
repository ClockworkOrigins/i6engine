/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "widgets/TechniquePropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/MaterialProperty.h"
#include "properties/StringProperty.h"
#include "properties/UIntProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	TechniquePropertyWindow::TechniquePropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_TECHNIQUE_ENABLED = QApplication::tr("Enabled");
		PRNL_TECHNIQUE_POSITION = QApplication::tr("Position");
		PRNL_TECHNIQUE_KEEP_LOCAL = QApplication::tr("Keep local");
		PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA = QApplication::tr("Visual particle quota");
		PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA = QApplication::tr("Emitted emitter quota");
		PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA = QApplication::tr("Emitted affector quota");
		PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA = QApplication::tr("Emitted technique quota");
		PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA = QApplication::tr("Emitted system quota");
		PRNL_TECHNIQUE_MATERIAL = QApplication::tr("Material");
		PRNL_TECHNIQUE_LOD_INDEX = QApplication::tr("Lod index");
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH = QApplication::tr("Default particle width");
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT = QApplication::tr("Default particle height");
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH = QApplication::tr("Default particle depth");
		PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION = QApplication::tr("Spatial hashing cell dimension");
		PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP = QApplication::tr("Spatial hashing cell overlap");
		PRNL_TECHNIQUE_SPHASHING_SIZE = QApplication::tr("Spatial hashtable size");
		PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL = QApplication::tr("Spatial hashing update interval");
		PRNL_TECHNIQUE_MAX_VELOCITY = QApplication::tr("Maximum velocity");
		PRNL_NAME = QApplication::tr("Name");
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
		append(new properties::MaterialProperty(this, PRNL_TECHNIQUE_MATERIAL, PRNL_TECHNIQUE_MATERIAL, "BaseWhite"));

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
		append(new properties::UIntProperty(this, PRNL_TECHNIQUE_SPHASHING_SIZE, PRNL_TECHNIQUE_SPHASHING_SIZE, static_cast<unsigned int>(ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_TABLE_SIZE)));

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

	void TechniquePropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToTechnique(prop, name);
		ParticleUniverse::ParticleTechnique * technique = static_cast<ParticleUniverse::ParticleTechnique *>(_owner->getPUElement());
		if (technique && technique->_isMarkedForEmission()) {
			ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
			if (system) {
				// Force recreation of new emitted techniques
				// 1. Search emitter that is emitting this technique.
				// 2. Find its parent technique.
				// 3. call the other technique's restartParticleTechnique().
				size_t numTechniques = system->getNumTechniques();
				for (size_t i = 0; i < numTechniques; ++i) {
					ParticleUniverse::ParticleTechnique * t = system->getTechnique(i);
					size_t numEmitters = t->getNumEmittedEmitters();
					for (size_t j = 0; j < numEmitters; ++j) {
						ParticleUniverse::ParticleEmitter * emitter = t->getEmitter(j);
						if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_TECHNIQUE && emitter->getEmitsName() == technique->getName()) {
							// Found it.
							t->_unprepare();
						}
					}
				}
			}
		}
		emit notifyChanged();
	}

	void TechniquePropertyWindow::copyAttributeToTechnique(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleTechnique * technique = static_cast<ParticleUniverse::ParticleTechnique *>(_owner->getPUElement());
		if (!technique) {
			return;
		}

		if (propertyName == PRNL_NAME) {
			// Name: String
			technique->setName(prop->getString().toStdString());
			_owner->setName(prop->getString());
			_owner->setCaption();
		} else if (propertyName == PRNL_TECHNIQUE_ENABLED) {
			// Enabled: Bool
			technique->_setOriginalEnabled(prop->getBool());
			technique->setEnabled(prop->getBool());
		} else if (propertyName == PRNL_TECHNIQUE_POSITION + PRNL_X || propertyName == PRNL_TECHNIQUE_POSITION + PRNL_Y || propertyName == PRNL_TECHNIQUE_POSITION + PRNL_Z) {
			// Position: Vector3
			technique->position = prop->getVector3();
			technique->originalPosition = prop->getVector3();
		} else if (propertyName == PRNL_TECHNIQUE_KEEP_LOCAL) {
			// Keep local: Bool
			technique->setKeepLocal(prop->getBool());
		} else if (propertyName == PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA) {
			// Visual particle quota: ParticleUniverse::Real
			restartParticle(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_VISUAL);
			technique->setVisualParticleQuota(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA) {
			// Emitted emitter quota: ParticleUniverse::Real
			restartParticle(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_EMITTER);
			technique->setEmittedEmitterQuota(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA) {
			// Emitted affector quota: ParticleUniverse::Real
			restartParticle(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_AFFECTOR);
			technique->setEmittedAffectorQuota(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA) {
			// Emitted technique quota: ParticleUniverse::Real
			restartParticle(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_TECHNIQUE);
			technique->setEmittedTechniqueQuota(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA) {
			// Emitted system quota: ParticleUniverse::Real
			restartParticle(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_SYSTEM);
			technique->setEmittedSystemQuota(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_MATERIAL) {
			Ogre::String materialName = prop->getString().toStdString();
			technique->setMaterialName(materialName);
		} else if (propertyName == PRNL_TECHNIQUE_LOD_INDEX) {
			// Lod index: unsigned short
			technique->setLodIndex(prop->getUInt());
		} else if (propertyName == PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH) {
			// Default particle width: ParticleUniverse::Real
			technique->setDefaultWidth(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT) {
			// Default particle height: ParticleUniverse::Real
			technique->setDefaultHeight(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH) {
			// Default particle depth: ParticleUniverse::Real
			technique->setDefaultDepth(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION) {
			// Spatial hashing cell dimension: ParticleUniverse::ushort
			technique->setSpatialHashingCellDimension(prop->getUInt());
		} else if (propertyName == PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP) {
			// Spatial hashing cell overlap: ParticleUniverse::ushort
			technique->setSpatialHashingCellOverlap(prop->getUInt());
		} else if (propertyName == PRNL_TECHNIQUE_SPHASHING_SIZE) {
			// Spatial hashtable size: unsigned int
			technique->setSpatialHashTableSize(prop->getUInt());
		} else if (propertyName == PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL) {
			// Spatial hashing update interval: ParticleUniverse::Real
			technique->setSpatialHashingInterval(prop->getDouble());
		} else if (propertyName == PRNL_TECHNIQUE_MAX_VELOCITY) {
			// Maximum velocity: ParticleUniverse::Real
			technique->setMaxVelocity(prop->getDouble());
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
