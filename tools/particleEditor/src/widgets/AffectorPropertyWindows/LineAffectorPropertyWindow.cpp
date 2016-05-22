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

#include "widgets/AffectorPropertyWindows/LineAffectorPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseLineAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	LineAffectorPropertyWindow::LineAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_END = QApplication::tr("End position");
		PRNL_DRIFT = QApplication::tr("Drift");
		PRNL_MAX_DEVIATION = QApplication::tr("Maximum deviation");
		PRNL_TIME_STEP = QApplication::tr("Time step");

		// Line end: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_END, PRNL_END, ParticleUniverse::LineAffector::DEFAULT_END));

		// Maximum Deviation: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_MAX_DEVIATION, PRNL_MAX_DEVIATION, ParticleUniverse::LineAffector::DEFAULT_MAX_DEVIATION));

		// Time Step: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::LineAffector::DEFAULT_TIME_STEP));

		// Drift: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_DRIFT, PRNL_DRIFT, ParticleUniverse::LineAffector::DEFAULT_DRIFT));
	}

	LineAffectorPropertyWindow::~LineAffectorPropertyWindow() {
	}

	void LineAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::LineAffector * lineAffector = static_cast<ParticleUniverse::LineAffector *>(affector);

		// Line end: Ogre::Vector3
		setVector3(PRNL_END, lineAffector->getEnd());

		// Maximum Deviation: ParticleUniverse::Real
		setDouble(PRNL_MAX_DEVIATION, lineAffector->getMaxDeviation());

		// Time Step: ParticleUniverse::Real
		setDouble(PRNL_TIME_STEP, lineAffector->getTimeStep());

		// Drift: ParticleUniverse::Real
		setDouble(PRNL_DRIFT, lineAffector->getDrift());
	}

	void LineAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop)
			return;

		ParticleUniverse::LineAffector * affector = static_cast<ParticleUniverse::LineAffector *>(_owner->getPUElement());
		if (!affector)
			return;

		if (propertyName == PRNL_END) {
			// Line end: Ogre::Vector3
			affector->setEnd(prop->getVector3());
		} else if (propertyName == PRNL_MAX_DEVIATION) {
			// Maximum Deviation: ParticleUniverse::Real
			affector->setMaxDeviation(prop->getDouble());
		} else if (propertyName == PRNL_TIME_STEP) {
			// Time Step: ParticleUniverse::Real
			affector->setTimeStep(prop->getDouble());
		} else if (propertyName == PRNL_DRIFT) {
			// Drift: ParticleUniverse::Real
			affector->setDrift(prop->getDouble());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
