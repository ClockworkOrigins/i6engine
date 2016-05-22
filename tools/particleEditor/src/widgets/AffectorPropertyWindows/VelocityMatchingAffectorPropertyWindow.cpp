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

#include "widgets/AffectorPropertyWindows/VelocityMatchingAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseVelocityMatchingAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	VelocityMatchingAffectorPropertyWindow::VelocityMatchingAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_VELOCITY_MATCHING_RADIUS = QApplication::tr("Velocity matching radius");

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_VELOCITY_MATCHING_RADIUS, PRNL_VELOCITY_MATCHING_RADIUS, ParticleUniverse::VelocityMatchingAffector::DEFAULT_RADIUS));
	}

	VelocityMatchingAffectorPropertyWindow::~VelocityMatchingAffectorPropertyWindow() {
	}

	void VelocityMatchingAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::VelocityMatchingAffector * velocityMatchingAffector = static_cast<ParticleUniverse::VelocityMatchingAffector *>(affector);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_VELOCITY_MATCHING_RADIUS, velocityMatchingAffector->getRadius());
	}

	void VelocityMatchingAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::VelocityMatchingAffector * affector = static_cast<ParticleUniverse::VelocityMatchingAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_VELOCITY_MATCHING_RADIUS) {
			// Radius: ParticleUniverse::Real
			affector->setRadius(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
