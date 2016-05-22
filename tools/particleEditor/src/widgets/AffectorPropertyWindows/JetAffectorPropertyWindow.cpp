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

#include "widgets/AffectorPropertyWindows/JetAffectorPropertyWindow.h"

#include "properties/DynamicAttributeProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseJetAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	JetAffectorPropertyWindow::JetAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ACCELERATION = QApplication::tr("Acceleration");

		// Acceleration: DynamicAttribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::JetAffector::DEFAULT_ACCELERATION);
		append(new properties::DynamicAttributeProperty(this, PRNL_ACCELERATION, PRNL_ACCELERATION, dynAttr));
	}

	JetAffectorPropertyWindow::~JetAffectorPropertyWindow() {
	}

	void JetAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::JetAffector * jetAffector = static_cast<ParticleUniverse::JetAffector *>(affector);

		// Acceleration: DynamicAttribute
		setDynamicAttribute(PRNL_ACCELERATION, jetAffector->getDynAcceleration());
	}

	void JetAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop)
			return;

		ParticleUniverse::JetAffector * affector = static_cast<ParticleUniverse::JetAffector *>(_owner->getPUElement());
		if (!affector)
			return;

		if (propertyName == PRNL_ACCELERATION) {
			// Acceleration: DynamicAttribute
			affector->setDynAcceleration(prop->getDynamicAttribute());
			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
