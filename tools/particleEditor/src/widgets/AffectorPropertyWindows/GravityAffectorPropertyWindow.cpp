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

#include "widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	GravityAffectorPropertyWindow::GravityAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_GRAVITY = QApplication::tr("Gravity");

		// Gravity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_GRAVITY, PRNL_GRAVITY, ParticleUniverse::GravityAffector::DEFAULT_GRAVITY));
	}

	GravityAffectorPropertyWindow::~GravityAffectorPropertyWindow() {
	}

	void GravityAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::GravityAffector * gravityAffector = static_cast<ParticleUniverse::GravityAffector *>(affector);

		// Gravity: ParticleUniverse::Real
		setDouble(PRNL_GRAVITY, gravityAffector->getGravity());
	}

	void GravityAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::GravityAffector * affector = static_cast<ParticleUniverse::GravityAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_GRAVITY) {
			// Gravity: ParticleUniverse::Real
			affector->setGravity(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
