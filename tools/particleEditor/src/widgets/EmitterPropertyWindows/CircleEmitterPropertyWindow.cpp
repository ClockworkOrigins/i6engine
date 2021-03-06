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

#include "widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseCircleEmitter.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	CircleEmitterPropertyWindow::CircleEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_CIRCLE_EMITTER_RADIUS = QApplication::tr("Circle Radius");
		PRNL_CIRCLE_EMITTER_STEP = QApplication::tr("Circle Step");
		PRNL_CIRCLE_EMITTER_ANGLE = QApplication::tr("Circle angle");
		PRNL_CIRCLE_EMITTER_RANDOM = QApplication::tr("Circle Random");
		PRNL_CIRCLE_EMITTER_NORMAL = QApplication::tr("Circle normal");

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_CIRCLE_EMITTER_RADIUS, PRNL_CIRCLE_EMITTER_RADIUS, ParticleUniverse::CircleEmitter::DEFAULT_RADIUS));

		// Step: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_CIRCLE_EMITTER_STEP, PRNL_CIRCLE_EMITTER_STEP, ParticleUniverse::CircleEmitter::DEFAULT_STEP));

		// Angle: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_CIRCLE_EMITTER_ANGLE, PRNL_CIRCLE_EMITTER_ANGLE, ParticleUniverse::CircleEmitter::DEFAULT_ANGLE));

		// Random: bool
		append(new properties::BoolProperty(this, PRNL_CIRCLE_EMITTER_RANDOM, PRNL_CIRCLE_EMITTER_RANDOM, ParticleUniverse::CircleEmitter::DEFAULT_RANDOM));

		// Normal: Vector3
		append(new properties::Vec3Property(this, PRNL_CIRCLE_EMITTER_NORMAL, PRNL_CIRCLE_EMITTER_NORMAL, ParticleUniverse::CircleEmitter::DEFAULT_NORMAL));
	}

	CircleEmitterPropertyWindow::~CircleEmitterPropertyWindow() {
	}

	void CircleEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::CircleEmitter * circleEmitter = static_cast<ParticleUniverse::CircleEmitter *>(emitter);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_CIRCLE_EMITTER_RADIUS, circleEmitter->getRadius());

		// Step: ParticleUniverse::Real
		setDouble(PRNL_CIRCLE_EMITTER_STEP, circleEmitter->getStep());

		// Angle: ParticleUniverse::Real
		setDouble(PRNL_CIRCLE_EMITTER_ANGLE, circleEmitter->getCircleAngle());

		// Random: bool
		setBool(PRNL_CIRCLE_EMITTER_RANDOM, circleEmitter->isRandom());

		// Normal: Vector3
		setVector3(PRNL_CIRCLE_EMITTER_NORMAL, circleEmitter->getNormal());
	}

	void CircleEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::CircleEmitter * circleEmitter = static_cast<ParticleUniverse::CircleEmitter *>(_owner->getPUElement());
		if (!circleEmitter) {
			return;
		}

		if (propertyName == PRNL_CIRCLE_EMITTER_ANGLE) {
			// Update emitter with angle
			circleEmitter->setCircleAngle(prop->getDouble());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_NORMAL) {
			// Update emitter with normal
			circleEmitter->setNormal(prop->getVector3());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_RADIUS) {
			// Update emitter with radius
			circleEmitter->setRadius(prop->getDouble());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_RANDOM) {
			// Update emitter with random
			circleEmitter->setRandom(prop->getBool());
		} else if (propertyName == PRNL_CIRCLE_EMITTER_STEP) {
			// Update emitter with step
			circleEmitter->setStep(prop->getDouble());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
