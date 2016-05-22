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

#include "widgets/AffectorPropertyWindows/CollisionAvoidanceAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseCollisionAvoidanceAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	CollisionAvoidanceAffectorPropertyWindow::CollisionAvoidanceAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_COLLISON_AVOIDANCE_RADIUS = QApplication::tr("Avoidance radius");

		// Avoidance Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLISON_AVOIDANCE_RADIUS, PRNL_COLLISON_AVOIDANCE_RADIUS, ParticleUniverse::CollisionAvoidanceAffector::DEFAULT_RADIUS));
	}

	CollisionAvoidanceAffectorPropertyWindow::~CollisionAvoidanceAffectorPropertyWindow() {
	}

	void CollisionAvoidanceAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::CollisionAvoidanceAffector * collisionAvoidanceAffector = static_cast<ParticleUniverse::CollisionAvoidanceAffector *>(affector);

		// Avoidance Radius: ParticleUniverse::Real
		setDouble(PRNL_COLLISON_AVOIDANCE_RADIUS, collisionAvoidanceAffector->getRadius());
	}

	void CollisionAvoidanceAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::CollisionAvoidanceAffector * affector = static_cast<ParticleUniverse::CollisionAvoidanceAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_COLLISON_AVOIDANCE_RADIUS) {
			// Avoidance Radius: ParticleUniverse::Real
			affector->setRadius(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
