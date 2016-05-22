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

#include "widgets/AffectorPropertyWindows/InterParticleColliderPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseInterParticleCollider.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	InterParticleColliderPropertyWindow::InterParticleColliderPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT = QApplication::tr("Adjustment");
		PRNL_COLLISON_RESPONSE = QApplication::tr("Collision response");
		CR_AVERAGE_VELOCITY = QApplication::tr("Average velocity");
		CR_ANGLE_BASED_VELOCITY = QApplication::tr("Angle based velocity");

		// Adjustment: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, ParticleUniverse::InterParticleCollider::DEFAULT_ADJUSTMENT));

		// Collision response: List
		QStringList collisionResponse;
		collisionResponse.append(CR_AVERAGE_VELOCITY);
		collisionResponse.append(CR_ANGLE_BASED_VELOCITY);
		append(new properties::EnumProperty(this, PRNL_COLLISON_RESPONSE, PRNL_COLLISON_RESPONSE, collisionResponse));
	}

	InterParticleColliderPropertyWindow::~InterParticleColliderPropertyWindow() {
	}

	void InterParticleColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::InterParticleCollider * interParticleCollider = static_cast<ParticleUniverse::InterParticleCollider *>(affector);

		// Adjustment: ParticleUniverse::Real
		setDouble(PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT, interParticleCollider->getAdjustment());

		// Collision response: List
		ParticleUniverse::InterParticleCollider::InterParticleCollisionResponse collisionResponse = interParticleCollider->getInterParticleCollisionResponse();
		QString collisionResponseString = CR_AVERAGE_VELOCITY;
		if (collisionResponse == ParticleUniverse::InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY) {
			collisionResponseString = CR_ANGLE_BASED_VELOCITY;
		}
		setEnumString(PRNL_COLLISON_RESPONSE, collisionResponseString);
	}

	void InterParticleColliderPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::InterParticleCollider * affector = static_cast<ParticleUniverse::InterParticleCollider *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT) {
			// Adjustment: ParticleUniverse::Real
			affector->setAdjustment(prop->getDouble());
		} else if (propertyName == PRNL_COLLISON_RESPONSE) {
			// Collision response: List
			QString collision = prop->getEnumString();
			if (collision == CR_AVERAGE_VELOCITY) {
				affector->setInterParticleCollisionResponse(ParticleUniverse::InterParticleCollider::IPCR_AVERAGE_VELOCITY);
			} else if (collision == CR_ANGLE_BASED_VELOCITY) {
				affector->setInterParticleCollisionResponse(ParticleUniverse::InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
