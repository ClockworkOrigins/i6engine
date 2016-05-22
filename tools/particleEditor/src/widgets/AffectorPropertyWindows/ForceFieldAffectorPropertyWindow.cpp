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

#include "widgets/AffectorPropertyWindows/ForceFieldAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseForceFieldAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	ForceFieldAffectorPropertyWindow::ForceFieldAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_AFFECTOR_FORCEFIELD_FF_TYPE = QApplication::tr("Forcefield type");
		FORCEFIELD_REALTIME = "Realtime";
		FORCEFIELD_MATRIX = "Matrix";
		PRNL_AFFECTOR_FORCEFIELD_DELTA = QApplication::tr("Delta");
		PRNL_AFFECTOR_FORCEFIELD_FORCE = QApplication::tr("Force");
		PRNL_AFFECTOR_FORCEFIELD_OCTAVES = QApplication::tr("Octaves");
		PRNL_AFFECTOR_FORCEFIELD_FREQUENCY = QApplication::tr("Frequency");
		PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE = QApplication::tr("Amplitude");
		PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE = QApplication::tr("Persistence");
		PRNL_AFFECTOR_FORCEFIELD_FF_SIZE = QApplication::tr("Forcefield size");
		PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE = QApplication::tr("Worldsize");
		PRNL_AFFECTOR_FORCEFIELD_IGNORE_X = QApplication::tr("Ignore -x");
		PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y = QApplication::tr("Ignore -y");
		PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z = QApplication::tr("Ignore -z");
		PRNL_AFFECTOR_FORCEFIELD_MOVEMENT = QApplication::tr("Movement");
		PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY = QApplication::tr("Movement frequency");
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

	void ForceFieldAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ForceFieldAffector * affector = static_cast<ParticleUniverse::ForceFieldAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FF_TYPE) {
			// forcefield_type: List
			QString type = prop->getEnumString();
			if (type == FORCEFIELD_REALTIME) {
				affector->setForceFieldType(ParticleUniverse::ForceField::FF_REALTIME_CALC);
			} else if (type == FORCEFIELD_MATRIX) {
				affector->setForceFieldType(ParticleUniverse::ForceField::FF_MATRIX_CALC);
			}
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_DELTA) {
			// delta: Real
			affector->setDelta(prop->getDouble());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FORCE) {
			// force: Real
			affector->setScaleForce(prop->getDouble());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_OCTAVES) {
			// octaves: uint
			affector->setOctaves(prop->getUInt());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FREQUENCY) {
			// frequency: Real
			affector->setFrequency(prop->getDouble());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE) {
			// amplitude: Real
			affector->setAmplitude(prop->getDouble());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE) {
			// persistence: Real
			affector->setPersistence(prop->getDouble());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FF_SIZE) {
			// forcefield_size: uint
			affector->setForceFieldSize(prop->getUInt());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE) {
			// worldsize: Vector3
			affector->setWorldSize(prop->getVector3());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_IGNORE_X) {
			// ignore_negative_x: bool
			affector->setIgnoreNegativeX(prop->getBool());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y) {
			// ignore_negative_y: bool
			affector->setIgnoreNegativeY(prop->getBool());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z) {
			// ignore_negative_z: bool
			affector->setIgnoreNegativeZ(prop->getBool());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_MOVEMENT) {
			// movement: Vector3
			affector->setMovement(prop->getVector3());
		} else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY) {
			// movement_frequency: Real
			affector->setMovementFrequency(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
