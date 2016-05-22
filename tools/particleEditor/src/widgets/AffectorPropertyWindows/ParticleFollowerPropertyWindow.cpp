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

#include "widgets/AffectorPropertyWindows/ParticleFollowerPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseParticleFollower.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	ParticleFollowerPropertyWindow::ParticleFollowerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_MIN_DISTANCE = QApplication::tr("Minimum distance");
		PRNL_MAX_DISTANCE = QApplication::tr("Maximum distance");

		// Min distance: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_MIN_DISTANCE, PRNL_MIN_DISTANCE, ParticleUniverse::ParticleFollower::DEFAULT_MIN_DISTANCE));

		// Max distance: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_MAX_DISTANCE, PRNL_MAX_DISTANCE, ParticleUniverse::ParticleFollower::DEFAULT_MAX_DISTANCE));
	}

	ParticleFollowerPropertyWindow::~ParticleFollowerPropertyWindow() {
	}

	void ParticleFollowerPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ParticleFollower * particleFollower = static_cast<ParticleUniverse::ParticleFollower *>(affector);

		// Min distance: ParticleUniverse::Real
		setDouble(PRNL_MIN_DISTANCE, particleFollower->getMinDistance());

		// Max distance: ParticleUniverse::Real
		setDouble(PRNL_MAX_DISTANCE, particleFollower->getMaxDistance());
	}

	void ParticleFollowerPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleFollower * affector = static_cast<ParticleUniverse::ParticleFollower *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_MIN_DISTANCE) {
			// Min distance: ParticleUniverse::Real
			affector->setMinDistance(prop->getDouble());
		} else if (propertyName == PRNL_MAX_DISTANCE) {
			// Max distance: ParticleUniverse::Real
			affector->setMaxDistance(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
