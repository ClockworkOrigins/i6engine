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

#include "widgets/EmitterPropertyWindows/PositionEmitterPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/Vec3ListProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniversePositionEmitter.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	PositionEmitterPropertyWindow::PositionEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_POSITION_EMITTER_POSITION = QApplication::tr("Add particle position");
		PRNL_POSITION_EMITTER_RANDOMIZE = QApplication::tr("Position randomize");
		PRNL_POSITION = QApplication::tr("Position");

		// Add Position: Button, with wich new positions can be added
		append(new properties::Vec3ListProperty(this, PRNL_POSITION_EMITTER_POSITION, PRNL_POSITION_EMITTER_POSITION));

		// Randomize: bool
		append(new properties::BoolProperty(this, PRNL_POSITION_EMITTER_RANDOMIZE, PRNL_POSITION_EMITTER_RANDOMIZE, ParticleUniverse::PositionEmitter::DEFAULT_RANDOMIZE));
	}

	PositionEmitterPropertyWindow::~PositionEmitterPropertyWindow() {
	}

	void PositionEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::PositionEmitter * positionEmitter = static_cast<ParticleUniverse::PositionEmitter *>(emitter);

		// Positions: List
		std::vector<Ogre::Vector3> positions(positionEmitter->getPositions().begin(), positionEmitter->getPositions().end());
		setVector3List(PRNL_POSITION_EMITTER_POSITION, positions);

		// Randomize: bool
		setBool(PRNL_POSITION_EMITTER_RANDOMIZE, positionEmitter->isRandomized());
	}

	void PositionEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::PositionEmitter * positionEmitter = static_cast<ParticleUniverse::PositionEmitter *>(_owner->getPUElement());
		if (!positionEmitter) {
			return;
		}

		if (propertyName == PRNL_POSITION_EMITTER_POSITION) {
			// Update emitter with Positions
			positionEmitter->removeAllPositions();
			for (ParticleUniverse::Vector3 vec : prop->getVector3List()) {
				positionEmitter->addPosition(vec);
			}
		} else if (propertyName == PRNL_POSITION_EMITTER_RANDOMIZE) {
			// Update emitter with Randomize
			positionEmitter->setRandomized(prop->getBool());
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
