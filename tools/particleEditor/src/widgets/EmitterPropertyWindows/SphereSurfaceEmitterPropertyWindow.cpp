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

#include "widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	SphereSurfaceEmitterPropertyWindow::SphereSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_SPHERE_SURFACE_EMITTER_RADIUS = QApplication::tr("Sphere Radius");

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SPHERE_SURFACE_EMITTER_RADIUS, PRNL_SPHERE_SURFACE_EMITTER_RADIUS, ParticleUniverse::SphereSurfaceEmitter::DEFAULT_RADIUS));
	}

	SphereSurfaceEmitterPropertyWindow::~SphereSurfaceEmitterPropertyWindow() {
	}

	void SphereSurfaceEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::SphereSurfaceEmitter * sphereSurfaceEmitter = static_cast<ParticleUniverse::SphereSurfaceEmitter *>(emitter);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_SPHERE_SURFACE_EMITTER_RADIUS, sphereSurfaceEmitter->getRadius());
	}

	void SphereSurfaceEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::SphereSurfaceEmitter * sphereSurfaceEmitter = static_cast<ParticleUniverse::SphereSurfaceEmitter *>(_owner->getPUElement());
		if (!sphereSurfaceEmitter) {
			return;
		}

		if (propertyName == PRNL_SPHERE_SURFACE_EMITTER_RADIUS) {
			// Update emitter with Radius
			sphereSurfaceEmitter->setRadius(prop->getDouble());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
