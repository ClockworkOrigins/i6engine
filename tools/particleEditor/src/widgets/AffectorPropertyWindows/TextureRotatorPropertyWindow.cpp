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

#include "widgets/AffectorPropertyWindows/TextureRotatorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseTextureRotator.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	TextureRotatorPropertyWindow::TextureRotatorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_USE_OWN_SPEED = QApplication::tr("Use own speed");
		PRNL_ROTATION = QApplication::tr("Rotation");
		PRNL_ROTATION_SPEED = QApplication::tr("Rotation speed");

		// Use Own Rotation speed
		append(new properties::BoolProperty(this, PRNL_USE_OWN_SPEED, PRNL_USE_OWN_SPEED, ParticleUniverse::TextureRotator::DEFAULT_USE_OWN_SPEED));

		// Rotation: Dynamic Attribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::TextureRotator::DEFAULT_ROTATION);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION, PRNL_ROTATION, dynAttr));

		// Rotation Speed: Dynamic Attribute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::TextureRotator::DEFAULT_ROTATION_SPEED);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr));
	}

	TextureRotatorPropertyWindow::~TextureRotatorPropertyWindow() {
	}

	void TextureRotatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::TextureRotator * textureRotator = static_cast<ParticleUniverse::TextureRotator *>(affector);

		// Use Own Rotation speed
		setBool(PRNL_USE_OWN_SPEED, textureRotator->useOwnRotationSpeed());

		// Rotation: Dynamic Attribute
		setDynamicAttribute(PRNL_ROTATION, textureRotator->getRotation());

		// Rotation Speed: Dynamic Attribute
		setDynamicAttribute(PRNL_ROTATION_SPEED, textureRotator->getRotationSpeed());
	}

	void TextureRotatorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::TextureRotator * affector = static_cast<ParticleUniverse::TextureRotator *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_USE_OWN_SPEED) {
			// Use own rotation speed: bool
			affector->setUseOwnRotationSpeed(prop->getBool());
		} else if (propertyName == PRNL_ROTATION_SPEED) {
			// Rotation Speed: Dynamic Attribute (Must be put before PRNL_ROTATION)
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				affector->setRotationSpeed(dynAttr);
			}

			if (affector->_isMarkedForEmission()) {
				restartParticle(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
			}
		} else if (propertyName == PRNL_ROTATION) {
			// Rotation: Dynamic Attribute
			ParticleUniverse::DynamicAttribute * dynAttr = prop->getDynamicAttribute();
			if (dynAttr) {
				affector->setRotation(dynAttr);
			}

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
