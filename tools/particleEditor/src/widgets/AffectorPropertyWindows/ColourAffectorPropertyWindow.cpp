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

#include "widgets/AffectorPropertyWindows/ColourAffectorPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/TimeAndColourListProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseColourAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	ColourAffectorPropertyWindow::ColourAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_TIME_AND_COLOUR = QApplication::tr("Time and Colour");
		PRNL_COLOUR_OPERATION = QApplication::tr("Colour operation");
		COP_SET = "Set";
		COP_MULTIPLY = "Multiply";

		// Time and Colour: List
		append(new properties::TimeAndColourListProperty(this, PRNL_TIME_AND_COLOUR, PRNL_TIME_AND_COLOUR));

		// Colour Operation: List of types
		QStringList colourOperations;
		colourOperations.append(COP_SET);
		colourOperations.append(COP_MULTIPLY);
		append(new properties::EnumProperty(this, PRNL_COLOUR_OPERATION, PRNL_COLOUR_OPERATION, colourOperations));
	}

	ColourAffectorPropertyWindow::~ColourAffectorPropertyWindow() {
	}

	void ColourAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::ColourAffector * colourAffector = static_cast<ParticleUniverse::ColourAffector *>(affector);

		std::vector<std::pair<double, Vec4>> timeAndColourList;
		for (auto & p : colourAffector->getTimeAndColour()) {
			timeAndColourList.push_back(std::make_pair(p.first, Vec4(p.second.a * 255, p.second.r * 255, p.second.g * 255, p.second.b * 255)));
		}
		setTimeAndColourList(PRNL_TIME_AND_COLOUR, timeAndColourList);

		// Colour Operation: List of types
		ParticleUniverse::ColourAffector::ColourOperation colourOperation = colourAffector->getColourOperation();
		QString colourOperationString = COP_SET;
		if (colourOperation == ParticleUniverse::ColourAffector::CAO_MULTIPLY) {
			colourOperationString = COP_MULTIPLY;
		}
		setEnumString(PRNL_COLOUR_OPERATION, colourOperationString);
	}

	void ColourAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ColourAffector * affector = static_cast<ParticleUniverse::ColourAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_TIME_AND_COLOUR) {
			// Colour Operation: List of types
			std::vector<std::pair<double, Vec4>> timeAndColourList = prop->getTimeAndColourList();
			affector->clearColourMap();
			for (auto & p : timeAndColourList) {
				affector->addColour(p.first, ParticleUniverse::ColourValue(p.second.getX() / 255, p.second.getY() / 255, p.second.getZ() / 255, p.second.getW() / 255));
			}
		} else if (propertyName == PRNL_COLOUR_OPERATION) {
			// Colour Operation: List of types
			QString operation = prop->getEnumString();
			if (operation == COP_SET) {
				affector->setColourOperation(ParticleUniverse::ColourAffector::CAO_SET);
			} else if (operation == COP_MULTIPLY) {
				affector->setColourOperation(ParticleUniverse::ColourAffector::CAO_MULTIPLY);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
